/**
 * COMP717A - Smart Garden & Irrigation Controller
 * ESP32 firmware with web dashboard, closed-loop irrigation, and hardware telemetry.
 *
 * Hardware Pin Connections:
 *   GPIO 34 - Soil Moisture Sensor (Analog ADC1)
 *   GPIO 35 - LDR Ambient Light Sensor (Analog ADC1)
 *   GPIO 4  - DHT11 Temp/Humidity Sensor
 *   GPIO 26 - Relay / Submersible Pump (Active-LOW)
 *   GPIO 23 - Active Buzzer
 *   GPIO 22 - Alert LED (Red - low moisture / warning)
 *   GPIO 1  - Status LED (Green - normal operation / shared with UART TX0)
 */

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include <DHT.h>
#include "dashboard_html.h"

// ---------------------------------------------------------------------------
// Hardware Pin Mappings
// ---------------------------------------------------------------------------
constexpr uint8_t PIN_SOIL_MOISTURE = 34;
constexpr uint8_t PIN_LDR           = 35;
constexpr uint8_t PIN_DHT           = 4;
constexpr uint8_t PIN_RELAY         = 26;
constexpr uint8_t PIN_BUZZER        = 23;
constexpr uint8_t PIN_LED_ALERT     = 22; // Red alert LED
constexpr uint8_t PIN_LED_OK        = 1;  // Green status LED (UART TX0)

#define DHTTYPE DHT11

// ---------------------------------------------------------------------------
// Network & Calibration Constants
// ---------------------------------------------------------------------------
const char* WIFI_SSID               = "ESP32_IMU";
const char* WIFI_PASSWORD           = "abcdefg";
constexpr uint32_t WIFI_TIMEOUT_MS  = 15000;

const char* AP_SSID                 = "ESP32-Irrigation";
const char* AP_PASSWORD             = "12345678";

constexpr bool RELAY_ACTIVE_LOW     = true;

constexpr uint16_t ADC_MAX          = 4095;
constexpr float    ADC_REF_V        = 3.3f;
constexpr int      DRY_VALUE        = 0;
constexpr int      WET_VALUE        = 1800;

constexpr uint32_t MAX_PUMP_RUN_MS    = 60000; // 60s hard safety cutoff
constexpr uint32_t SENSOR_INTERVAL_MS = 2000;  // 2s DHT11 sampling interval

// ---------------------------------------------------------------------------
// Irrigation State Machine
// ---------------------------------------------------------------------------
enum IrrigationState : uint8_t {
  STATE_IDLE,
  STATE_IRRIGATING,
  STATE_COOLDOWN
};

IrrigationState irrigationState = STATE_IDLE;
uint32_t pumpStartTime         = 0;
uint32_t currentDurationMs     = 10000;
uint32_t cooldownStartTime     = 0;
uint32_t currentCooldownMs     = 30000;
const char* triggerSource      = "None";

// Physical indicator state
bool     isMoistureGood = true;
bool     buzzerActive   = false;
uint32_t buzzerEndTime  = 0;

// Configurable thresholds (stored in NVS flash)
int   minMoistureThreshold = 30;
int   maxMoistureThreshold = 70;
float highTempLimit        = 35.0f;
int   defaultDurationSec   = 10;
int   cooldownPeriodSec    = 30;
bool  autoModeEnabled      = true;

// Sensor cache
int      soilMoistureRaw     = 0;
int      soilMoisturePercent = 0;
int      lightRaw            = 0;
int      lightPercent        = 0;
float    lightVoltage        = 0.0f;
float    cachedTemperature   = 24.0f;
float    cachedHumidity      = 50.0f;
bool     dhtValid            = false;
uint32_t lastSensorRead      = 0;

bool isStationMode = false;

// ---------------------------------------------------------------------------
// Event Log (Circular Ring Buffer)
// ---------------------------------------------------------------------------
struct LogEntry {
  uint32_t uptime;
  char     source[12];
  char     message[64];
};

constexpr uint8_t MAX_LOG_ENTRIES = 8;
LogEntry eventLog[MAX_LOG_ENTRIES];
uint8_t  logHead  = 0;
uint8_t  logCount = 0;

// ---------------------------------------------------------------------------
// Peripherals & Server Instances
// ---------------------------------------------------------------------------
WebServer   server(80);
DHT         dht(PIN_DHT, DHTTYPE);
Preferences prefs;

// ---------------------------------------------------------------------------
// Hardware Actuators & Indicators
// ---------------------------------------------------------------------------

void setPump(bool turnOn) {
  uint8_t level = turnOn ? (RELAY_ACTIVE_LOW ? LOW : HIGH)
                         : (RELAY_ACTIVE_LOW ? HIGH : LOW);
  digitalWrite(PIN_RELAY, level);
}

void triggerBuzzer(uint32_t durationMs) {
  digitalWrite(PIN_BUZZER, HIGH);
  buzzerActive  = true;
  buzzerEndTime = millis() + durationMs;
}

void updatePhysicalIndicators() {
  bool currentGood = (soilMoisturePercent >= minMoistureThreshold);

  // Trigger audio feedback and log entry on state transition
  if (currentGood != isMoistureGood) {
    isMoistureGood = currentGood;
    if (!isMoistureGood) {
      triggerBuzzer(500); // 500ms warning alert
      logEvent("Alert", "Soil moisture below threshold");
    } else {
      triggerBuzzer(200); // 200ms confirmation chirp
      logEvent("Status", "Soil moisture restored to normal");
    }
  }

  // Update physical LED states
  digitalWrite(PIN_LED_ALERT, isMoistureGood ? LOW : HIGH);
  digitalWrite(PIN_LED_OK,    isMoistureGood ? HIGH : LOW);

  // Turn off buzzer non-blockingly when active duration expires
  if (buzzerActive && millis() >= buzzerEndTime) {
    digitalWrite(PIN_BUZZER, LOW);
    buzzerActive = false;
  }
}

// ---------------------------------------------------------------------------
// Logging & Time Utilities
// ---------------------------------------------------------------------------

void logEvent(const char* source, const char* message) {
  uint32_t uptime = millis() / 1000;

  if (logCount < MAX_LOG_ENTRIES) {
    logHead = logCount++;
  } else {
    logHead = (logHead + 1) % MAX_LOG_ENTRIES;
  }

  eventLog[logHead].uptime = uptime;
  strncpy(eventLog[logHead].source, source, sizeof(eventLog[logHead].source) - 1);
  eventLog[logHead].source[sizeof(eventLog[logHead].source) - 1] = '\0';
  strncpy(eventLog[logHead].message, message, sizeof(eventLog[logHead].message) - 1);
  eventLog[logHead].message[sizeof(eventLog[logHead].message) - 1] = '\0';

  Serial.printf("[%lus] [%s] %s\n", (unsigned long)uptime, source, message);
}

String formatUptime(uint32_t totalSec) {
  char buf[16];
  snprintf(buf, sizeof(buf), "%02lu:%02lu:%02lu",
           (unsigned long)(totalSec / 3600),
           (unsigned long)((totalSec % 3600) / 60),
           (unsigned long)(totalSec % 60));
  return String(buf);
}

// ---------------------------------------------------------------------------
// JSON Parsing & Parameter Helpers
// ---------------------------------------------------------------------------

static bool extractJsonValue(const String& body, const char* key, String& valueOut) {
  String searchKey = "\"";
  searchKey += key;
  searchKey += "\"";

  int keyIdx = body.indexOf(searchKey);
  if (keyIdx == -1) return false;

  int colonIdx = body.indexOf(':', keyIdx + searchKey.length());
  if (colonIdx == -1) return false;

  int start = colonIdx + 1;
  while (start < (int)body.length() && (body[start] == ' ' || body[start] == '\t' || body[start] == '"')) {
    start++;
  }

  int end = start;
  while (end < (int)body.length() && body[end] != ',' && body[end] != '}' && body[end] != '"' && body[end] != '\r' && body[end] != '\n') {
    end++;
  }

  if (end > start) {
    valueOut = body.substring(start, end);
    valueOut.trim();
    return true;
  }
  return false;
}

static bool parseJsonInt(const String& body, const char* key, int& out) {
  String val;
  if (extractJsonValue(body, key, val)) {
    out = val.toInt();
    return true;
  }
  return false;
}

static bool parseJsonFloat(const String& body, const char* key, float& out) {
  String val;
  if (extractJsonValue(body, key, val)) {
    out = val.toFloat();
    return true;
  }
  return false;
}

static bool parseJsonBool(const String& body, const char* key, bool& out) {
  String val;
  if (extractJsonValue(body, key, val)) {
    val.toLowerCase();
    out = (val == "true" || val == "1");
    return true;
  }
  return false;
}

// Read parameter from either URL/form argument or JSON request body
static void readParamInt(const String& body, const char* key, int& target) {
  if (server.hasArg(key)) {
    target = server.arg(key).toInt();
  } else {
    parseJsonInt(body, key, target);
  }
}

static void readParamFloat(const String& body, const char* key, float& target) {
  if (server.hasArg(key)) {
    target = server.arg(key).toFloat();
  } else {
    parseJsonFloat(body, key, target);
  }
}

static void readParamBool(const String& body, const char* key, bool& target) {
  if (server.hasArg(key)) {
    String arg = server.arg(key);
    target = (arg == "true" || arg == "1");
  } else {
    parseJsonBool(body, key, target);
  }
}

// ---------------------------------------------------------------------------
// NVS Flash Persistence
// ---------------------------------------------------------------------------

void loadSettings() {
  prefs.begin("irrigation", true);
  minMoistureThreshold = prefs.getInt("minM", 30);
  maxMoistureThreshold = prefs.getInt("maxM", 70);
  highTempLimit        = prefs.getFloat("highT", 35.0f);
  defaultDurationSec   = prefs.getInt("dur", 10);
  cooldownPeriodSec    = prefs.getInt("cd", 30);
  autoModeEnabled      = prefs.getBool("auto", true);
  prefs.end();

  // Validate bounds on loaded settings
  if (minMoistureThreshold >= maxMoistureThreshold) {
    minMoistureThreshold = 30;
    maxMoistureThreshold = 70;
  }
  if (defaultDurationSec <= 0 || defaultDurationSec > 60) {
    defaultDurationSec = 10;
  }
  if (cooldownPeriodSec < 5 || cooldownPeriodSec > 600) {
    cooldownPeriodSec = 30;
  }

  Serial.printf("[NVS] Loaded: MinM=%d%%, MaxM=%d%%, HighT=%.1fC, Dur=%ds, Cooldown=%ds, Auto=%s\n",
                minMoistureThreshold, maxMoistureThreshold, highTempLimit,
                defaultDurationSec, cooldownPeriodSec, autoModeEnabled ? "YES" : "NO");
}

void saveSettings() {
  prefs.begin("irrigation", false);
  prefs.putInt("minM",    minMoistureThreshold);
  prefs.putInt("maxM",    maxMoistureThreshold);
  prefs.putFloat("highT", highTempLimit);
  prefs.putInt("dur",     defaultDurationSec);
  prefs.putInt("cd",      cooldownPeriodSec);
  prefs.putBool("auto",   autoModeEnabled);
  prefs.end();
  Serial.println("[NVS] Settings saved.");
}

// ---------------------------------------------------------------------------
// Irrigation Control Logic
// ---------------------------------------------------------------------------

void startIrrigation(uint32_t durationSec, const char* source) {
  if (durationSec == 0) durationSec = defaultDurationSec;
  if (durationSec * 1000 > MAX_PUMP_RUN_MS) {
    durationSec = MAX_PUMP_RUN_MS / 1000;
  }

  currentDurationMs = durationSec * 1000;
  pumpStartTime     = millis();
  irrigationState   = STATE_IRRIGATING;
  triggerSource     = source;

  setPump(true);
  triggerBuzzer(500);

  char msg[64];
  snprintf(msg, sizeof(msg), "Pump started for %lu s (%s)", (unsigned long)durationSec, source);
  logEvent(source, msg);
}

void stopIrrigation(const char* reason) {
  setPump(false);
  triggerBuzzer(250);

  char msg[64];
  snprintf(msg, sizeof(msg), "Pump stopped: %s", reason);
  logEvent("System", msg);

  irrigationState   = STATE_COOLDOWN;
  cooldownStartTime = millis();
  currentCooldownMs = (uint32_t)cooldownPeriodSec * 1000;
}

void processIrrigationStateMachine() {
  uint32_t now = millis();

  switch (irrigationState) {
    case STATE_IDLE:
      if (autoModeEnabled && soilMoisturePercent < minMoistureThreshold) {
        char reason[64];
        snprintf(reason, sizeof(reason), "Soil %d%% < Min %d%%", soilMoisturePercent, minMoistureThreshold);
        logEvent("Auto", reason);
        startIrrigation(defaultDurationSec, "Auto");
      }
      break;

    case STATE_IRRIGATING:
      if (now - pumpStartTime >= MAX_PUMP_RUN_MS) {
        stopIrrigation("Safety timeout reached");
      } else if (now - pumpStartTime >= currentDurationMs) {
        stopIrrigation("Scheduled duration completed");
      } else if (soilMoisturePercent >= maxMoistureThreshold) {
        stopIrrigation("Target moisture reached");
      }
      break;

    case STATE_COOLDOWN:
      if (now - cooldownStartTime >= currentCooldownMs) {
        irrigationState = STATE_IDLE;
        logEvent("System", "Cooldown complete, returning to IDLE");
      }
      break;
  }
}

// ---------------------------------------------------------------------------
// Sensor Acquisition
// ---------------------------------------------------------------------------

void readSensors() {
  uint32_t now = millis();
  if (now - lastSensorRead < SENSOR_INTERVAL_MS) {
    return;
  }
  lastSensorRead = now;

  // 1. Soil moisture (HW-038)
  soilMoistureRaw     = analogRead(PIN_SOIL_MOISTURE);
  int mappedMoisture  = map(soilMoistureRaw, DRY_VALUE, WET_VALUE, 0, 100);
  soilMoisturePercent = constrain(mappedMoisture, 0, 100);

  // 2. Ambient light (LDR)
  lightRaw     = analogRead(PIN_LDR);
  lightVoltage = lightRaw * (ADC_REF_V / (float)ADC_MAX);
  lightPercent = constrain(map(lightRaw, 0, ADC_MAX, 0, 100), 0, 100);

  // 3. Ambient temperature & humidity (DHT11)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("[DHT] Sensor read failed; serving cached values");
    dhtValid = false;
  } else {
    cachedHumidity    = h;
    cachedTemperature = t;
    dhtValid          = true;
  }
}

// ---------------------------------------------------------------------------
// Web Server & REST API Handlers
// ---------------------------------------------------------------------------

void setCorsHeaders() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

void handleRoot() {
  setCorsHeaders();
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleGetData() {
  setCorsHeaders();

  uint32_t now = millis();
  uint32_t remainingSec = 0;
  uint32_t cooldownSec  = 0;

  if (irrigationState == STATE_IRRIGATING) {
    uint32_t elapsed = now - pumpStartTime;
    if (elapsed < currentDurationMs) {
      remainingSec = (currentDurationMs - elapsed) / 1000;
    }
  } else if (irrigationState == STATE_COOLDOWN) {
    uint32_t elapsed = now - cooldownStartTime;
    if (elapsed < currentCooldownMs) {
      cooldownSec = (currentCooldownMs - elapsed) / 1000;
    }
  }

  const char* stateStr = (irrigationState == STATE_IRRIGATING) ? "Irrigating" :
                         (irrigationState == STATE_COOLDOWN)   ? "Cooldown"   : "Idle";

  String json;
  json.reserve(2048);

  json += "{";

  // Indicators
  json += "\"indicators\":{";
  json += "\"goodState\":"    + String(isMoistureGood ? "true" : "false") + ",";
  json += "\"buzzerActive\":" + String(buzzerActive ? "true" : "false");
  json += "},";

  // Soil moisture
  json += "\"soilMoisture\":{";
  json += "\"percent\":" + String(soilMoisturePercent) + ",";
  json += "\"raw\":"     + String(soilMoistureRaw)     + ",";
  json += "\"dryVal\":"  + String(DRY_VALUE)           + ",";
  json += "\"wetVal\":"  + String(WET_VALUE);
  json += "},";

  // Temperature & humidity (DHT11)
  json += "\"temperature\":" + String(cachedTemperature, 1)        + ",";
  json += "\"humidity\":"    + String(cachedHumidity, 1)           + ",";
  json += "\"dhtValid\":"    + String(dhtValid ? "true" : "false") + ",";

  // Ambient light (LDR)
  json += "\"light\":{";
  json += "\"percent\":" + String(lightPercent)    + ",";
  json += "\"raw\":"     + String(lightRaw)        + ",";
  json += "\"voltage\":" + String(lightVoltage, 2);
  json += "},";

  // Irrigation actuator status
  json += "\"irrigation\":{";
  json += "\"state\":\""           + String(stateStr) + "\",";
  json += "\"pumpActive\":"        + String((irrigationState == STATE_IRRIGATING) ? "true" : "false") + ",";
  json += "\"autoMode\":"          + String(autoModeEnabled ? "true" : "false")                        + ",";
  json += "\"remainingSeconds\":"  + String(remainingSec)                                              + ",";
  json += "\"totalDuration\":"     + String(currentDurationMs / 1000)                                  + ",";
  json += "\"cooldownRemaining\":" + String(cooldownSec)                                               + ",";
  json += "\"totalCooldown\":"     + String(currentCooldownMs / 1000);
  json += "},";

  // Configured thresholds
  json += "\"thresholds\":{";
  json += "\"minMoisture\":" + String(minMoistureThreshold) + ",";
  json += "\"maxMoisture\":" + String(maxMoistureThreshold) + ",";
  json += "\"highTemp\":"    + String(highTempLimit, 1)     + ",";
  json += "\"duration\":"    + String(defaultDurationSec)   + ",";
  json += "\"cooldown\":"    + String(cooldownPeriodSec);
  json += "},";

  // Dynamic alerts
  json += "\"alerts\":[";
  bool hasAlert = false;
  if (soilMoisturePercent < minMoistureThreshold) {
    json += "{\"type\":\"danger\",\"message\":\"Soil moisture critically low (" 
         + String(soilMoisturePercent) + "% < " + String(minMoistureThreshold) + "%)\"}";
    hasAlert = true;
  }
  if (cachedTemperature >= highTempLimit) {
    if (hasAlert) json += ",";
    json += "{\"type\":\"warning\",\"message\":\"High temperature alert (" 
         + String(cachedTemperature, 1) + " C >= " + String(highTempLimit, 1) + " C)\"}";
    hasAlert = true;
  }
  if (irrigationState == STATE_IRRIGATING) {
    if (hasAlert) json += ",";
    json += "{\"type\":\"warning\",\"message\":\"Irrigation cycle active\"}";
    hasAlert = true;
  }
  json += "],";

  // Recent events (ordered newest to oldest)
  json += "\"events\":[";
  for (uint8_t i = 0; i < logCount; i++) {
    int idx = (logHead - i + MAX_LOG_ENTRIES) % MAX_LOG_ENTRIES;
    if (i > 0) json += ",";
    json += "{";
    json += "\"time\":\""   + formatUptime(eventLog[idx].uptime) + "\",";
    json += "\"source\":\"" + String(eventLog[idx].source)       + "\",";
    json += "\"text\":\""   + String(eventLog[idx].message)      + "\"";
    json += "}";
  }
  json += "],";

  // System diagnostics
  json += "\"system\":{";
  json += "\"uptime\":"    + String(now / 1000)                                                        + ",";
  json += "\"wifiMode\":\"" + String(isStationMode ? "Station (STA)" : "Soft-AP")                       + "\",";
  json += "\"ip\":\""       + (isStationMode ? WiFi.localIP().toString() : WiFi.softAPIP().toString()) + "\",";
  json += "\"freeHeap\":"   + String(ESP.getFreeHeap());
  json += "}";

  json += "}";

  server.send(200, "application/json", json);
}

void handleSetThresholds() {
  setCorsHeaders();
  String body = server.arg("plain");

  int   newMin   = minMoistureThreshold;
  int   newMax   = maxMoistureThreshold;
  float newHighT = highTempLimit;
  int   newDur   = defaultDurationSec;
  int   newCd    = cooldownPeriodSec;
  bool  newAuto  = autoModeEnabled;

  readParamInt(body, "minMoisture", newMin);
  readParamInt(body, "maxMoisture", newMax);
  readParamFloat(body, "highTemp",  newHighT);
  readParamInt(body, "duration",    newDur);
  readParamInt(body, "cooldown",    newCd);
  readParamBool(body, "autoMode",   newAuto);

  // Validation
  if (newMin >= newMax || newMin < 0 || newMax > 100) {
    server.send(400, "application/json", "{\"success\":false,\"error\":\"Min moisture must be strictly less than Max.\"}");
    return;
  }
  if (newDur < 1 || newDur > 60) {
    server.send(400, "application/json", "{\"success\":false,\"error\":\"Duration must be 1-60s.\"}");
    return;
  }
  if (newCd < 5 || newCd > 600) {
    server.send(400, "application/json", "{\"success\":false,\"error\":\"Cooldown must be 5-600s.\"}");
    return;
  }

  minMoistureThreshold = newMin;
  maxMoistureThreshold = newMax;
  highTempLimit        = newHighT;
  defaultDurationSec   = newDur;
  cooldownPeriodSec    = newCd;
  autoModeEnabled      = newAuto;

  saveSettings();
  logEvent("User", "Thresholds updated via web dashboard");

  server.send(200, "application/json", "{\"success\":true,\"message\":\"Settings saved to NVS successfully.\"}");
}

void handleIrrigate() {
  setCorsHeaders();
  String body = server.arg("plain");
  int duration = defaultDurationSec;

  readParamInt(body, "duration", duration);

  if (duration < 1 || duration > 60) {
    server.send(400, "application/json", "{\"success\":false,\"error\":\"Duration must be 1-60s.\"}");
    return;
  }

  startIrrigation(duration, "Manual");
  server.send(200, "application/json", "{\"success\":true,\"message\":\"Irrigation cycle started.\"}");
}

void handleStopIrrigate() {
  setCorsHeaders();
  stopIrrigation("Manual stop requested");
  server.send(200, "application/json", "{\"success\":true,\"message\":\"Irrigation stopped.\"}");
}

void handleNotFound() {
  setCorsHeaders();
  if (server.method() == HTTP_OPTIONS) {
    server.send(204);
    return;
  }
  server.send(404, "text/plain", "Not Found");
}

// ---------------------------------------------------------------------------
// Setup & Main Loop
// ---------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== COMP717A Smart Garden Controller ===");

  // Output pins: pre-set initial inactive states before setting OUTPUT mode
  digitalWrite(PIN_RELAY, RELAY_ACTIVE_LOW ? HIGH : LOW);
  pinMode(PIN_RELAY, OUTPUT);

  pinMode(PIN_BUZZER,    OUTPUT);
  pinMode(PIN_LED_ALERT, OUTPUT);
  pinMode(PIN_LED_OK,    OUTPUT);
  digitalWrite(PIN_BUZZER,    LOW);
  digitalWrite(PIN_LED_ALERT, LOW);
  digitalWrite(PIN_LED_OK,    HIGH);

  // ADC resolution & attenuation (12-bit, ~3.3V range)
  analogReadResolution(12);
  analogSetPinAttenuation(PIN_SOIL_MOISTURE, ADC_11db);
  analogSetPinAttenuation(PIN_LDR,           ADC_11db);

  dht.begin();
  loadSettings();

  // WiFi connection with automatic fallback to Soft-AP
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.printf("[WiFi] Connecting to '%s'...\n", WIFI_SSID);

  uint32_t startWifi = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - startWifi < WIFI_TIMEOUT_MS)) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    isStationMode = true;
    Serial.printf("\n[WiFi] Connected. IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    isStationMode = false;
    Serial.println("\n[WiFi] Connection timed out. Starting fallback Soft-AP...");
    WiFi.mode(WIFI_AP);
    if (strlen(AP_PASSWORD) >= 8) {
      WiFi.softAP(AP_SSID, AP_PASSWORD);
    } else {
      WiFi.softAP(AP_SSID);
    }
    Serial.printf("[WiFi] Soft-AP '%s' active. IP: %s\n", AP_SSID, WiFi.softAPIP().toString().c_str());
  }

  if (MDNS.begin("irrigation")) {
    Serial.println("[mDNS] Responder started at http://irrigation.local");
  }

  // Register HTTP routes
  server.on("/",                  HTTP_GET,  handleRoot);
  server.on("/api/data",          HTTP_GET,  handleGetData);
  server.on("/api/thresholds",    HTTP_POST, handleSetThresholds);
  server.on("/api/irrigate",      HTTP_POST, handleIrrigate);
  server.on("/api/irrigate/stop", HTTP_POST, handleStopIrrigate);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("[HTTP] Server started on port 80");

  logEvent("System", "System booted");
}

void loop() {
  server.handleClient();
  readSensors();
  processIrrigationStateMachine();
  updatePhysicalIndicators();
}
