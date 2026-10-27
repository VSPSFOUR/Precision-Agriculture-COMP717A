#ifndef DASHBOARD_HTML_H
#define DASHBOARD_HTML_H

#include <pgmspace.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Precision Agriculture • Smart Irrigation Dashboard</title>
  <script src="https://www.gstatic.com/antigravity/web/dev/tailwindcss.min.js"></script>
  <style>
    /* 1. Organic Light Neumorphic Variables */
    :root {
      --bg: #ebf0ea;
      --card-bg: #ebf0ea;
      --surface-warm: #f1f5f0;
      --text-main: #233329;
      --text-muted: #5e7064;
      --border-subtle: rgba(255, 255, 255, 0.75);

      --nm-flat: 8px 8px 18px #cfd6cc, -8px -8px 18px #ffffff;
      --nm-flat-sm: 4px 4px 10px #d2d9cf, -4px -4px 10px #ffffff;
      --nm-inset: inset 4px 4px 9px #cbd3c7, inset -4px -4px 9px #ffffff;
      --nm-inset-sm: inset 3px 3px 6px #cfd7cb, inset -3px -3px 6px #ffffff;
      --nm-btn-pressed: inset 4px 4px 8px #c5cec1, inset -4px -4px 8px #ffffff;

      --sage-deep: #2d5a3f;
      --sage-primary: #3d7051;
      --sage-light: #52966e;
      --emerald-glow: #10b981;
      --earthen-terracotta: #c9653b;
      --solar-amber: #e59837;
      --dew-blue: #258fc4;
      --danger-coral: #d94545;
    }

    /* 2. Self-Contained Offline Utility Fallbacks */
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      background-color: var(--bg);
      color: var(--text-main);
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
      user-select: none;
      -webkit-font-smoothing: antialiased;
      min-height: 100vh;
      display: flex;
      justify-content: center;
      padding: 16px;
    }

    .container-max { width: 100%; max-width: 1152px; display: flex; flex-direction: column; gap: 24px; }
    .flex { display: flex; }
    .flex-col { flex-direction: column; }
    .items-center { align-items: center; }
    .justify-between { justify-content: space-between; }
    .justify-center { justify-content: center; }
    .gap-2 { gap: 8px; }
    .gap-3 { gap: 12px; }
    .gap-4 { gap: 16px; }
    .gap-6 { gap: 24px; }
    .w-full { width: 100%; }
    .hidden { display: none !important; }
    .font-mono { font-family: ui-monospace, SFMono-Regular, Menlo, Monaco, Consolas, monospace; }

    /* 3. Tactile Neumorphic Elements */
    .nm-pebble {
      background: var(--card-bg);
      box-shadow: var(--nm-flat);
      border-radius: 28px;
      border: 1px solid var(--border-subtle);
    }
    .nm-card {
      background: var(--card-bg);
      box-shadow: var(--nm-flat);
      border-radius: 22px;
      border: 1px solid var(--border-subtle);
      padding: 22px;
    }
    .nm-card-sm {
      background: var(--card-bg);
      box-shadow: var(--nm-flat-sm);
      border-radius: 16px;
      border: 1px solid var(--border-subtle);
    }
    .nm-sunken {
      background: var(--card-bg);
      box-shadow: var(--nm-inset);
      border-radius: 18px;
    }
    .nm-sunken-sm {
      background: var(--card-bg);
      box-shadow: var(--nm-inset-sm);
      border-radius: 12px;
    }
    .nm-press-btn {
      background: var(--card-bg);
      box-shadow: var(--nm-flat-sm);
      border-radius: 14px;
      border: 1px solid var(--border-subtle);
      transition: all 0.18s cubic-bezier(0.34, 1.56, 0.64, 1);
      cursor: pointer;
      font-weight: 700;
    }
    .nm-press-btn:hover {
      transform: translateY(-2px);
      filter: brightness(1.02);
    }
    .nm-press-btn:active, .nm-press-btn.active {
      box-shadow: var(--nm-btn-pressed) !important;
      transform: translateY(1px);
    }

    /* Rocker & Debug Toggles */
    .nm-leaf-toggle {
      width: 54px;
      height: 28px;
      background: var(--card-bg);
      box-shadow: var(--nm-inset-sm);
      border-radius: 9999px;
      position: relative;
      cursor: pointer;
      transition: all 0.25s ease;
    }
    .nm-leaf-thumb {
      width: 22px;
      height: 22px;
      border-radius: 50%;
      background: var(--card-bg);
      box-shadow: var(--nm-flat-sm);
      position: absolute;
      top: 3px;
      left: 3px;
      display: flex;
      align-items: center;
      justify-content: center;
      transition: transform 0.25s cubic-bezier(0.34, 1.56, 0.64, 1), background 0.2s;
    }
    .nm-leaf-toggle.active {
      background: rgba(61, 112, 81, 0.15);
    }
    .nm-leaf-toggle.active .nm-leaf-thumb {
      transform: translateX(26px);
      background: #3d7051;
      color: #ffffff;
      box-shadow: 0 0 10px rgba(61, 112, 81, 0.5);
    }

    .nm-debug-toggle {
      width: 42px;
      height: 22px;
      background: var(--card-bg);
      box-shadow: var(--nm-inset-sm);
      border-radius: 9999px;
      position: relative;
      cursor: pointer;
      transition: all 0.2s ease;
    }
    .nm-debug-thumb {
      width: 16px;
      height: 16px;
      border-radius: 50%;
      background: var(--card-bg);
      box-shadow: var(--nm-flat-sm);
      position: absolute;
      top: 3px;
      left: 3px;
      transition: transform 0.2s ease, background 0.2s;
    }
    .nm-debug-toggle.active {
      background: rgba(2, 132, 199, 0.15);
    }
    .nm-debug-toggle.active .nm-debug-thumb {
      transform: translateX(20px);
      background: #0284c7;
      box-shadow: 0 0 8px rgba(2, 132, 199, 0.6);
    }

    /* Sliders */
    input[type=range].nm-organic-slider {
      -webkit-appearance: none;
      width: 100%;
      height: 10px;
      background: var(--card-bg);
      box-shadow: var(--nm-inset-sm);
      border-radius: 9999px;
      outline: none;
    }
    input[type=range].nm-organic-slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      width: 24px;
      height: 24px;
      border-radius: 50%;
      background: var(--card-bg);
      box-shadow: var(--nm-flat-sm);
      border: 2px solid #3d7051;
      cursor: pointer;
      transition: transform 0.15s;
    }
    input[type=range].nm-organic-slider::-webkit-slider-thumb:hover {
      transform: scale(1.18);
    }

    /* Hardware Indicator Bezels */
    .pebble-bezel {
      width: 34px;
      height: 34px;
      border-radius: 50%;
      box-shadow: var(--nm-inset-sm);
      display: flex;
      align-items: center;
      justify-content: center;
      background: var(--card-bg);
    }
    .led-bulb {
      width: 16px;
      height: 16px;
      border-radius: 50%;
      transition: all 0.3s ease;
      background: #94a3b8;
    }
    .led-sprout-on {
      background: #10b981;
      box-shadow: 0 0 16px rgba(16, 185, 129, 0.9), 0 0 4px #10b981;
    }
    .led-alert-on {
      background: #ef4444;
      box-shadow: 0 0 16px rgba(239, 68, 68, 0.9), 0 0 4px #ef4444;
      animation: alertPulse 1.2s infinite ease-in-out;
    }
    @keyframes alertPulse {
      0%, 100% { opacity: 0.85; transform: scale(1); }
      50% { opacity: 1; transform: scale(1.1); filter: brightness(1.2); }
    }

    @keyframes gentleBreeze {
      0%, 100% { transform: rotate(0deg); }
      35% { transform: rotate(1.8deg) translateY(-0.5px); }
      70% { transform: rotate(-1.5deg); }
    }
    .plant-sway {
      transform-origin: bottom center;
      animation: gentleBreeze 4.5s ease-in-out infinite;
    }

    @keyframes dewFloat {
      0%, 100% { transform: translateY(0px) scale(1); opacity: 0.8; }
      50% { transform: translateY(-3px) scale(1.08); opacity: 1; }
    }
    .dew-float {
      animation: dewFloat 3s ease-in-out infinite;
    }

    .terminal-scroll::-webkit-scrollbar {
      width: 6px;
    }
    .terminal-scroll::-webkit-scrollbar-track {
      background: transparent;
    }
    .terminal-scroll::-webkit-scrollbar-thumb {
      background: rgba(94, 112, 100, 0.3);
      border-radius: 9999px;
    }
  </style>
</head>
<body>

  <div class="container-max">

    <!-- HEADER: RENAMED TO PRECISION AGRICULTURE (IP REMOVED) -->
    <header class="nm-pebble p-5 sm:p-6 flex flex-col md:flex-row items-center justify-between gap-5 relative overflow-hidden">
      <!-- Decorative Leaf Watermark -->
      <svg class="absolute -right-6 -bottom-8 w-44 h-44 text-emerald-900/[0.04] pointer-events-none transform rotate-12" viewBox="0 0 24 24" fill="currentColor">
        <path d="M12 2C6.5 2 2 6.5 2 12c0 5 3.5 9.1 8.2 9.8-.1-.6-.2-1.3-.2-2 0-3.3 2.7-6 6-6 .7 0 1.4.1 2 .2.7-4.7-3.4-8.2-6-8.2zm6 10c-2.2 0-4 1.8-4 4 0 1.2.5 2.2 1.3 3 .7.5 1.7.9 2.7 1 2.2 0 4-1.8 4-4s-1.8-4-4-4z"/>
      </svg>

      <div class="flex items-center gap-4 w-full md:w-auto relative z-10">
        <!-- Pebble Sprout Icon -->
        <div class="w-16 h-16 rounded-3xl nm-sunken flex items-center justify-center text-[#3d7051] shrink-0">
          <svg class="w-9 h-9 plant-sway" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <path d="M7 20h10"/>
            <path d="M10 20c0-4.5 1.5-8 5-11"/>
            <path d="M15 9c-3 0-5.5 1.5-6 4.5"/>
            <path d="M14 9c0-3.5 2.5-6 6-6 0 3.5-2.5 6-6 6z"/>
            <path d="M9 14c-3.5 0-6-2.5-6-6 3.5 0 6 2.5 6 6z"/>
          </svg>
        </div>

        <div>
          <div class="flex items-center gap-2.5 flex-wrap">
            <h1 class="text-2xl font-bold tracking-tight text-[#233329]">
              Precision Agriculture
            </h1>
            <span class="text-xs px-3 py-0.5 rounded-full font-semibold nm-sunken text-[#3d7051] flex items-center gap-1.5">
              <span class="w-2 h-2 rounded-full bg-[#3d7051]"></span>
              ORGANIC SOFT UI
            </span>
          </div>
          <p class="text-xs sm:text-sm text-[#5e7064] mt-0.5">
            Closed-Loop Hydration & Environmental Phytotelemetry
          </p>
        </div>
      </div>

      <!-- Action & Network Status Badges (No IP Address) -->
      <div class="flex flex-wrap items-center justify-end gap-2.5 w-full md:w-auto relative z-10">
        <!-- Live vs Simulation Toggle -->
        <button id="btnMode" onclick="toggleEngineMode()" class="nm-press-btn px-3.5 py-2 text-xs font-semibold flex items-center gap-2 text-[#3d7051]">
          <span class="w-2 h-2 rounded-full bg-emerald-500 animate-ping"></span>
          <span id="txtMode">LIVE ESP32 CONNECTED</span>
        </button>

        <!-- Sound Buzzer Synthesis -->
        <button id="btnSound" onclick="toggleAudioBuzzer()" class="nm-press-btn p-2 text-xs text-[#5e7064] hover:text-[#c9653b]" title="Toggle Safety Buzzer Audio Feedback">
          <svg id="iconSound" class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
            <polygon points="11 5 6 9 2 9 2 15 6 15 11 19 11 5"></polygon>
            <path d="M19.07 4.93a10 10 0 0 1 0 14.14M15.54 8.46a5 5 0 0 1 0 7.07"></path>
          </svg>
        </button>

        <!-- Network Online Status (No IP Address) -->
        <div class="nm-sunken-sm px-3.5 py-1.5 flex items-center gap-2 text-xs text-[#5e7064]">
          <span id="statusDot" class="w-2 h-2 rounded-full bg-emerald-500 shadow-sm"></span>
          <span id="badgeWifi">Network Online</span>
        </div>
      </div>
    </header>

    <!-- LIVING PLANT VITALITY HERO CARD -->
    <section class="nm-card p-6">
      <div class="grid grid-cols-1 lg:grid-cols-12 gap-6 items-center">
        
        <!-- Left: Interactive Animated Botanical Avatar -->
        <div class="lg:col-span-6 flex items-center gap-5">
          <div class="w-28 h-28 sm:w-32 sm:h-32 rounded-3xl nm-sunken flex items-center justify-center relative p-3 shrink-0">
            <div class="absolute top-2 right-2 dew-float text-sky-500">
              <svg class="w-5 h-5 fill-current" viewBox="0 0 24 24">
                <path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z"/>
              </svg>
            </div>

            <!-- Dynamic Plant Graphic -->
            <svg id="plantAvatarSVG" class="w-20 h-20 plant-sway transition-all duration-700" viewBox="0 0 100 100">
              <ellipse cx="50" cy="85" rx="30" ry="8" fill="#d2dad0"/>
              <path d="M26 68 L32 85 C34 88, 66 88, 68 85 L74 68 Z" fill="#c9653b" opacity="0.85"/>
              <ellipse cx="50" cy="68" rx="24" ry="5" fill="#a44e28"/>
              <ellipse cx="50" cy="68" rx="21" ry="3.5" fill="#4d3224"/>
              
              <path id="plantStem" d="M50 67 Q50 45 50 25" stroke="#3d7051" stroke-width="4.5" stroke-linecap="round" fill="none"/>
              <path id="leafLeft" d="M50 48 C32 46, 22 32, 28 20 C40 22, 48 38, 50 48 Z" fill="#52966e" class="transition-all duration-700 origin-bottom-right"/>
              <path id="leafRight" d="M50 40 C68 38, 78 24, 72 12 C60 14, 52 30, 50 40 Z" fill="#3d7051" class="transition-all duration-700 origin-bottom-left"/>
              <circle cx="50" cy="22" r="4.5" fill="#84c99b"/>
            </svg>
          </div>

          <!-- Vitality Metric Details with Filled Progress Bar -->
          <div class="space-y-1.5 w-full">
            <div class="flex justify-between items-center">
              <span class="text-xs uppercase font-bold tracking-wider text-[#5e7064]">Plant Health Index</span>
              <span id="vitalityPctLabel" class="text-sm font-extrabold text-[#3d7051]">94%</span>
            </div>
            <div id="plantHealthTitle" class="text-xl sm:text-2xl font-extrabold text-[#233329]">
              Flourishing & Healthy
            </div>
            
            <div class="nm-sunken-sm h-3 p-0.5 overflow-hidden">
              <div id="vitalityFillBar" class="h-full rounded-full bg-gradient-to-r from-[#3d7051] to-[#10b981] transition-all duration-500" style="width: 94%;"></div>
            </div>

            <p id="plantHealthDesc" class="text-xs text-[#5e7064] leading-relaxed pt-0.5">
              Root moisture is well-balanced within the optimal 30%–70% absorption range.
            </p>
          </div>
        </div>

        <!-- Right: Status Indicators Dock (Clean Natural Labels, GPIOs hidden behind debug) -->
        <div class="lg:col-span-6 nm-sunken p-4 sm:p-5 rounded-2xl flex flex-col sm:flex-row items-center justify-around gap-4">
          <!-- GOOD STATUS LED -->
          <div class="flex items-center gap-3">
            <div class="pebble-bezel">
              <div id="ledGood" class="led-bulb led-sprout-on"></div>
            </div>
            <div>
              <div class="text-xs font-bold text-[#233329]">HEALTHY STATUS</div>
              <div class="text-xs text-[#5e7064]">
                <span>Hydration Balanced</span>
                <span class="debug-tag hidden font-mono text-[10px] text-sky-600 ml-1">(GPIO 1)</span>
              </div>
            </div>
          </div>

          <div class="hidden sm:block w-px h-8 bg-black/10"></div>

          <!-- ALERT STATUS LED -->
          <div class="flex items-center gap-3">
            <div class="pebble-bezel">
              <div id="ledBad" class="led-bulb"></div>
            </div>
            <div>
              <div class="text-xs font-bold text-[#233329]">MOISTURE ALERT</div>
              <div class="text-xs text-[#5e7064]">
                <span>Low Water Attention</span>
                <span class="debug-tag hidden font-mono text-[10px] text-sky-600 ml-1">(GPIO 22)</span>
              </div>
            </div>
          </div>

          <div class="hidden sm:block w-px h-8 bg-black/10"></div>

          <!-- BUZZER ALARM -->
          <div class="flex items-center gap-3">
            <div class="pebble-bezel">
              <svg id="buzzerIcon" class="w-4 h-4 text-slate-400 transition-colors" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <path d="M11 5 6 9H2v6h4l5 4V5z"/>
                <path d="M15.54 8.46a5 5 0 0 1 0 7.07"/>
                <path d="M19.07 4.93a10 10 0 0 1 0 14.14"/>
              </svg>
            </div>
            <div>
              <div id="buzzerTitle" class="text-xs font-bold text-[#233329]">BUZZER IDLE</div>
              <div class="text-xs text-[#5e7064]">
                <span>Audible Safety</span>
                <span class="debug-tag hidden font-mono text-[10px] text-sky-600 ml-1">(GPIO 23)</span>
              </div>
            </div>
          </div>
        </div>

      </div>
    </section>

    <!-- 4 SENSOR RESERVOIR BOXES (Strictly contained, no color bleeding out) -->
    <section class="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-4 gap-6">

      <!-- 1. SOIL HYDRATION BOX -->
      <div class="nm-card p-5 flex flex-col items-center justify-between space-y-4">
        <div class="w-full flex justify-between items-start">
          <div>
            <span class="text-xs font-bold uppercase tracking-wider text-[#5e7064]">Soil Moisture</span>
            <div class="text-xs text-[#3d7051] font-semibold mt-0.5">
              <span>Root Hydration</span>
              <span class="debug-tag hidden font-mono text-[10px] text-sky-600 ml-1">• HW-038 (GPIO 34)</span>
            </div>
          </div>
          <div class="w-8 h-8 rounded-2xl nm-sunken flex items-center justify-center text-[#3d7051]">
            <svg class="w-4 h-4 fill-current" viewBox="0 0 24 24">
              <path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z"/>
            </svg>
          </div>
        </div>

        <!-- Debossed Fluid Reservoir Box -->
        <div class="relative w-36 h-36 rounded-2xl nm-sunken overflow-hidden flex flex-col justify-end">
          <div id="boxFillMoisture" 
               class="w-full transition-all duration-700 ease-out bg-gradient-to-t from-[#3d7051]/55 to-[#52966e]/35 border-t-2 border-[#3d7051]/70" 
               style="height: 48%;">
          </div>

          <div class="absolute inset-0 flex flex-col items-center justify-center p-2 z-10 pointer-events-none">
            <div class="nm-card-sm px-3.5 py-1.5 rounded-xl bg-white/70 backdrop-blur-xs border border-white/80 shadow-xs flex flex-col items-center">
              <div class="flex items-baseline justify-center">
                <span id="valMoisture" class="text-3xl font-black text-[#233329]">48</span>
                <span class="text-base font-extrabold text-[#3d7051] ml-0.5">%</span>
              </div>
              <span class="text-xs text-[#5e7064] font-bold uppercase tracking-wider -mt-0.5">HYDRATION</span>
            </div>
          </div>
        </div>

        <div class="w-full nm-sunken-sm p-2 text-center">
          <div class="text-xs text-[#5e7064] flex justify-between px-1">
            <span>Soil State</span>
            <span id="badgeMoistureState" class="font-bold text-[#3d7051]">ADEQUATE</span>
          </div>
        </div>
      </div>

      <!-- 2. CANOPY TEMPERATURE BOX -->
      <div class="nm-card p-5 flex flex-col items-center justify-between space-y-4">
        <div class="w-full flex justify-between items-start">
          <div>
            <span class="text-xs font-bold uppercase tracking-wider text-[#5e7064]">Canopy Temp</span>
            <div class="text-xs text-[#c9653b] font-semibold mt-0.5">
              <span>Ambient Thermal</span>
              <span class="debug-tag hidden font-mono text-[10px] text-sky-600 ml-1">• DHT11 (GPIO 4)</span>
            </div>
          </div>
          <div class="w-8 h-8 rounded-2xl nm-sunken flex items-center justify-center text-[#c9653b]">
            <svg class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z"/>
            </svg>
          </div>
        </div>

        <!-- Debossed Fluid Reservoir Box -->
        <div class="relative w-36 h-36 rounded-2xl nm-sunken overflow-hidden flex flex-col justify-end">
          <div id="boxFillTemp" 
               class="w-full transition-all duration-700 ease-out bg-gradient-to-t from-[#c9653b]/55 to-[#e59837]/35 border-t-2 border-[#c9653b]/70" 
               style="height: 49.6%;">
          </div>

          <div class="absolute inset-0 flex flex-col items-center justify-center p-2 z-10 pointer-events-none">
            <div class="nm-card-sm px-3.5 py-1.5 rounded-xl bg-white/70 backdrop-blur-xs border border-white/80 shadow-xs flex flex-col items-center">
              <div class="flex items-baseline justify-center">
                <span id="valTemp" class="text-3xl font-black text-[#233329]">24.8</span>
                <span class="text-base font-extrabold text-[#c9653b] ml-0.5">°C</span>
              </div>
              <span class="text-xs text-[#5e7064] font-bold uppercase tracking-wider -mt-0.5">TEMPERATURE</span>
            </div>
          </div>
        </div>

        <div class="w-full nm-sunken-sm p-2 text-center">
          <div class="text-xs text-[#5e7064] flex justify-between px-1">
            <span>Thermal Zone</span>
            <span id="badgeTempState" class="font-bold text-[#c9653b]">TEMPERATE</span>
          </div>
        </div>
      </div>

      <!-- 3. RELATIVE HUMIDITY BOX -->
      <div class="nm-card p-5 flex flex-col items-center justify-between space-y-4">
        <div class="w-full flex justify-between items-start">
          <div>
            <span class="text-xs font-bold uppercase tracking-wider text-[#5e7064]">Leaf Humidity</span>
            <div class="text-xs text-sky-600 font-semibold mt-0.5">
              <span>Vapor Density</span>
              <span class="debug-tag hidden font-mono text-[10px] text-sky-600 ml-1">• DHT11 (GPIO 4)</span>
            </div>
          </div>
          <div class="w-8 h-8 rounded-2xl nm-sunken flex items-center justify-center text-sky-600">
            <svg class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M17.5 19H9a7 7 0 1 1 6.71-9h1.79a4.5 4.5 0 1 1 0 9Z"/>
            </svg>
          </div>
        </div>

        <!-- Debossed Fluid Reservoir Box -->
        <div class="relative w-36 h-36 rounded-2xl nm-sunken overflow-hidden flex flex-col justify-end">
          <div id="boxFillHumidity" 
               class="w-full transition-all duration-700 ease-out bg-gradient-to-t from-[#258fc4]/55 to-[#38bdf8]/35 border-t-2 border-[#258fc4]/70" 
               style="height: 64%;">
          </div>

          <div class="absolute inset-0 flex flex-col items-center justify-center p-2 z-10 pointer-events-none">
            <div class="nm-card-sm px-3.5 py-1.5 rounded-xl bg-white/70 backdrop-blur-xs border border-white/80 shadow-xs flex flex-col items-center">
              <div class="flex items-baseline justify-center">
                <span id="valHumidity" class="text-3xl font-black text-[#233329]">64</span>
                <span class="text-base font-extrabold text-sky-600 ml-0.5">%</span>
              </div>
              <span class="text-xs text-[#5e7064] font-bold uppercase tracking-wider -mt-0.5">HUMIDITY</span>
            </div>
          </div>
        </div>

        <div class="w-full nm-sunken-sm p-2 text-center">
          <div class="text-xs text-[#5e7064] flex justify-between px-1">
            <span>Dew Point</span>
            <span class="font-bold text-sky-600 font-mono">16.4°C</span>
          </div>
        </div>
      </div>

      <!-- 4. SOLAR ILLUMINATION BOX -->
      <div class="nm-card p-5 flex flex-col items-center justify-between space-y-4">
        <div class="w-full flex justify-between items-start">
          <div>
            <span class="text-xs font-bold uppercase tracking-wider text-[#5e7064]">Daylight Flux</span>
            <div class="text-xs text-amber-600 font-semibold mt-0.5">
              <span>Solar Intensity</span>
              <span class="debug-tag hidden font-mono text-[10px] text-sky-600 ml-1">• LDR (GPIO 35)</span>
            </div>
          </div>
          <div class="w-8 h-8 rounded-2xl nm-sunken flex items-center justify-center text-amber-500">
            <svg class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <circle cx="12" cy="12" r="4"/><path d="M12 2v2"/><path d="M12 20v2"/><path d="m4.93 4.93 1.41 1.41"/><path d="m17.66 17.66 1.41 1.41"/><path d="M2 12h2"/><path d="M20 12h2"/><path d="m6.34 17.66-1.41 1.41"/><path d="m19.07 4.93-1.41 1.41"/>
            </svg>
          </div>
        </div>

        <!-- Debossed Fluid Reservoir Box -->
        <div class="relative w-36 h-36 rounded-2xl nm-sunken overflow-hidden flex flex-col justify-end">
          <div id="boxFillLight" 
               class="w-full transition-all duration-700 ease-out bg-gradient-to-t from-[#e59837]/55 to-[#f59e0b]/35 border-t-2 border-[#e59837]/70" 
               style="height: 74%;">
          </div>

          <div class="absolute inset-0 flex flex-col items-center justify-center p-2 z-10 pointer-events-none">
            <div class="nm-card-sm px-3.5 py-1.5 rounded-xl bg-white/70 backdrop-blur-xs border border-white/80 shadow-xs flex flex-col items-center">
              <div class="flex items-baseline justify-center">
                <span id="valLight" class="text-3xl font-black text-[#233329]">74</span>
                <span class="text-base font-extrabold text-amber-600 ml-0.5">%</span>
              </div>
              <span class="text-xs text-[#5e7064] font-bold uppercase tracking-wider -mt-0.5">SUNLIGHT</span>
            </div>
          </div>
        </div>

        <div class="w-full nm-sunken-sm p-2 text-center">
          <div class="text-xs text-[#5e7064] flex justify-between px-1">
            <span>Sun Condition</span>
            <span class="font-bold text-amber-600">BRIGHT</span>
          </div>
        </div>
      </div>

    </section>

    <!-- ACTUATION & CLOSED-LOOP CONTROLS -->
    <section class="grid grid-cols-1 lg:grid-cols-12 gap-6">

      <!-- LEFT 7 COLS: IRRIGATION CONTROLLER -->
      <div class="lg:col-span-7 nm-card p-6 flex flex-col justify-between space-y-6">
        <div>
          <!-- Header -->
          <div class="flex items-center justify-between mb-4">
            <div class="flex items-center gap-3">
              <div id="pumpIconBox" class="w-12 h-12 rounded-2xl nm-sunken flex items-center justify-center text-[#3d7051]">
                <svg class="w-6 h-6" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                  <path d="M12 2v20M17 5H9.5a3.5 3.5 0 0 0 0 7h5a3.5 3.5 0 0 1 0 7H6"/>
                </svg>
              </div>
              <div>
                <h2 class="text-base font-bold text-[#233329]">
                  Water Irrigation Pump
                </h2>
                <span class="text-xs text-[#5e7064]">
                  <span>Submersible Irrigation Actuator</span>
                  <span class="debug-tag hidden font-mono text-[10px] text-sky-600 ml-1">• Relay (GPIO 26)</span>
                </span>
              </div>
            </div>

            <div id="pumpStateBadge" class="nm-sunken-sm px-3.5 py-1.5 text-xs font-bold tracking-wider text-[#5e7064]">
              STATUS: IDLE
            </div>
          </div>

          <!-- Leaf Auto Rocker -->
          <div class="nm-sunken p-4 rounded-2xl flex items-center justify-between mb-5">
            <div>
              <div class="text-sm font-bold text-[#233329]">Auto-Hydration Cycle</div>
              <div class="text-xs text-[#5e7064]">Triggers automatic gentle watering when soil moisture drops below threshold</div>
            </div>

            <!-- Organic Leaf Rocker -->
            <div id="toggleAuto" onclick="toggleAutoMode()" class="nm-leaf-toggle active">
              <div class="nm-leaf-thumb">
                <svg class="w-3.5 h-3.5 fill-current" viewBox="0 0 24 24">
                  <path d="M12 2C6.5 2 2 6.5 2 12c0 5 3.5 9.1 8.2 9.8-.1-.6-.2-1.3-.2-2 0-3.3 2.7-6 6-6 .7 0 1.4.1 2 .2.7-4.7-3.4-8.2-6-8.2z"/>
                </svg>
              </div>
            </div>
          </div>

          <!-- Hydration Progress Inset Track -->
          <div class="space-y-2">
            <div class="flex justify-between text-xs text-[#5e7064]">
              <span id="pumpDetailText">Hydration Loop Resting • Soil Stable</span>
              <span id="countdownText" class="font-mono font-bold text-[#233329]">0s Remaining</span>
            </div>
            <div class="nm-sunken-sm h-3.5 p-0.5 overflow-hidden">
              <div id="pumpProgressBar" class="h-full rounded-full bg-gradient-to-r from-[#52966e] to-[#258fc4] transition-all duration-300" style="width: 0%;"></div>
            </div>
          </div>
        </div>

        <!-- Manual Controls Row -->
        <div class="grid grid-cols-1 sm:grid-cols-12 gap-3 pt-2">
          <!-- Duration Stepper -->
          <div class="sm:col-span-4 nm-sunken-sm px-3 py-2 flex items-center justify-between">
            <span class="text-xs text-[#5e7064]">Cycle:</span>
            <div class="flex items-center gap-1.5">
              <input id="inpManualDuration" type="number" min="1" max="60" value="10" 
                     class="w-12 bg-transparent text-center font-bold text-sm text-[#233329] outline-none">
              <span class="text-xs text-[#5e7064]">sec</span>
            </div>
          </div>

          <!-- Irrigate Garden Button -->
          <button id="btnRunPump" onclick="triggerManualIrrigation()" 
                  class="sm:col-span-5 nm-press-btn py-3 px-4 flex items-center justify-center gap-2 text-sm font-bold text-[#3d7051]">
            <svg class="w-4 h-4 fill-current" viewBox="0 0 24 24"><polygon points="5 3 19 12 5 21 5 3"/></svg>
            IRRIGATE GARDEN
          </button>

          <!-- Emergency Cutoff Button -->
          <button id="btnEmergencyStop" onclick="emergencyStopPump()" 
                  class="sm:col-span-3 nm-press-btn py-3 px-3 flex items-center justify-center gap-1.5 text-xs font-bold text-rose-600 hover:text-rose-700">
            <svg class="w-4 h-4" viewBox="0 0 24 24" fill="currentColor"><rect x="6" y="6" width="12" height="12"/></svg>
            HALT
          </button>
        </div>
      </div>

      <!-- RIGHT 5 COLS: BOTANICAL THRESHOLD LIMITS -->
      <div class="lg:col-span-5 nm-card p-6 flex flex-col justify-between space-y-4">
        <div>
          <div class="flex items-center justify-between mb-3">
            <div>
              <h2 class="text-base font-bold text-[#233329]">Target Thresholds</h2>
              <span class="text-xs text-[#5e7064]">Persistent System Parameters (NVS)</span>
            </div>
            <div class="w-9 h-9 rounded-2xl nm-sunken flex items-center justify-center text-[#3d7051]">
              <svg class="w-5 h-5" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <circle cx="12" cy="12" r="3"/>
                <path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"/>
              </svg>
            </div>
          </div>

          <!-- Sliders -->
          <div class="space-y-4 pt-1">
            <!-- Min Moisture Trigger -->
            <div>
              <div class="flex justify-between text-xs mb-1.5">
                <span class="text-[#5e7064] font-medium">Hydrate Trigger (Min Soil Moisture)</span>
                <span class="font-bold text-[#3d7051]"><span id="lblMinMoist">30</span>%</span>
              </div>
              <input id="sliderMinMoist" type="range" min="10" max="60" value="30" class="nm-organic-slider" oninput="updateThresholdSlider('minMoist', this.value)">
            </div>

            <!-- Max Moisture Cutoff -->
            <div>
              <div class="flex justify-between text-xs mb-1.5">
                <span class="text-[#5e7064] font-medium">Target Saturation Cut-off</span>
                <span class="font-bold text-sky-600"><span id="lblMaxMoist">70</span>%</span>
              </div>
              <input id="sliderMaxMoist" type="range" min="50" max="95" value="70" class="nm-organic-slider" oninput="updateThresholdSlider('maxMoist', this.value)">
            </div>

            <!-- High Temp Alarm -->
            <div>
              <div class="flex justify-between text-xs mb-1.5">
                <span class="text-[#5e7064] font-medium">High Temp Warning Limit</span>
                <span class="font-bold text-[#c9653b]"><span id="lblHighTemp">35</span>°C</span>
              </div>
              <input id="sliderHighTemp" type="range" min="25" max="50" value="35" class="nm-organic-slider" oninput="updateThresholdSlider('highTemp', this.value)">
            </div>

            <!-- Cooldown Delay -->
            <div>
              <div class="flex justify-between text-xs mb-1.5">
                <span class="text-[#5e7064] font-medium">Resting Cooldown (Soak Duration)</span>
                <span class="font-bold text-[#233329]"><span id="lblCooldown">30</span>s</span>
              </div>
              <input id="sliderCooldown" type="range" min="10" max="180" step="5" value="30" class="nm-organic-slider" oninput="updateThresholdSlider('cooldown', this.value)">
            </div>
          </div>
        </div>

        <button onclick="commitThresholds()" class="nm-press-btn w-full py-2.5 text-xs font-bold text-[#3d7051] flex items-center justify-center gap-2">
          <svg class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"/><polyline points="17 21 17 13 7 13 7 21"/><polyline points="7 3 7 8 15 8"/></svg>
          SAVE PARAMETERS TO FLASH
        </button>
      </div>
    </section>

    <!-- DEBUG SECTION: HIDDEN BY DEFAULT BEHIND THE DEBUG TOGGLE -->
    <section id="debugPanel" class="nm-card p-6 hidden transition-all duration-300 border-2 border-sky-400/20">
      <div class="flex items-center justify-between mb-4">
        <div class="flex items-center gap-2.5">
          <div class="w-3 h-3 rounded-full bg-sky-500 shadow-sm animate-pulse"></div>
          <h3 class="text-xs sm:text-sm font-bold uppercase tracking-wider text-[#233329]">
            Debug Telemetry & Pin Diagnostics
          </h3>
        </div>
        <div class="flex items-center gap-2">
          <button onclick="clearEventLogs()" class="nm-press-btn px-2.5 py-1 text-xs text-[#5e7064] hover:text-rose-600">
            Clear
          </button>
          <span id="logCounter" class="nm-sunken-sm px-2.5 py-1 text-xs font-mono text-[#5e7064]">0 entries</span>
        </div>
      </div>

      <!-- Quick Hardware Pin Reference Tray -->
      <div class="grid grid-cols-2 sm:grid-cols-4 gap-3 mb-4 text-xs font-mono">
        <div class="nm-sunken-sm p-2 text-center">
          <div class="text-[#5e7064] text-[10px]">SOIL ADC</div>
          <div class="font-bold text-[#3d7051]">HW-038 • GPIO 34</div>
          <div id="debugMoistRaw" class="text-[10px] text-slate-500">ADC: --</div>
        </div>
        <div class="nm-sunken-sm p-2 text-center">
          <div class="text-[#5e7064] text-[10px]">TEMP/HUMID</div>
          <div class="font-bold text-[#c9653b]">DHT11 • GPIO 4</div>
          <div class="text-[10px] text-slate-500">Sampling: 2s</div>
        </div>
        <div class="nm-sunken-sm p-2 text-center">
          <div class="text-[#5e7064] text-[10px]">LIGHT SENSOR</div>
          <div class="font-bold text-[#e59837]">LDR • GPIO 35</div>
          <div id="debugLightVolt" class="text-[10px] text-slate-500">0.00V</div>
        </div>
        <div class="nm-sunken-sm p-2 text-center">
          <div class="text-[#5e7064] text-[10px]">PUMP RELAY</div>
          <div class="font-bold text-sky-600">RELAY • GPIO 26</div>
          <div class="text-[10px] text-slate-500">Active-LOW</div>
        </div>
      </div>

      <!-- Sunken Garden Terminal -->
      <div class="nm-sunken p-4 font-mono text-xs max-h-48 overflow-y-auto terminal-scroll space-y-2 rounded-2xl" id="eventListContainer">
        <!-- Log items injected via JS -->
      </div>
    </section>

    <!-- FOOTER WITH DEBUG TOGGLE (BOTTOM OF PAGE) -->
    <footer class="flex flex-col sm:flex-row items-center justify-between gap-4 px-2 text-xs text-[#5e7064]">
      <div>
        Uptime: <span id="footUptime" class="font-mono font-bold text-[#233329]">00:00:00</span> | Free Heap: <span id="footHeap" class="font-mono font-bold text-[#233329]">-- KB</span>
      </div>

      <!-- DEBUG TOGGLE -->
      <div class="flex items-center gap-2.5 nm-sunken-sm px-3.5 py-1.5">
        <span class="text-xs font-bold text-[#5e7064]">Debug</span>
        <div id="toggleDebugSwitch" onclick="toggleDebugMode()" class="nm-debug-toggle">
          <div class="nm-debug-thumb"></div>
        </div>
      </div>

      <div>
        Precision Agriculture • Closed-Loop Phytotelemetry
      </div>
    </footer>

  </div>

  <!-- TOAST NOTIFICATION -->
  <div id="toast" class="fixed bottom-6 right-6 pointer-events-none transition-all duration-300 opacity-0 transform translate-y-4 nm-card p-4 flex items-center gap-3 text-xs font-semibold z-50">
    <div id="toastDot" class="w-2.5 h-2.5 rounded-full bg-emerald-600 shadow-sm"></div>
    <span id="toastMessage">Parameters successfully saved</span>
  </div>

  <!-- SCRIPT ENGINE -->
  <script>
    let isConnectedToHardware = true;
    let failCount = 0;
    let debugMode = false;
    let audioEnabled = false;
    let audioCtx = null;
    let autoMode = true;
    let pumpActive = false;
    let pumpRemaining = 0;
    let pumpTotal = 10;
    let cooldownRemaining = 0;
    let cooldownTotal = 30;
    let uptimeSeconds = 0;
    let pollTimer = null;

    let thresholds = {
      minMoisture: 30,
      maxMoisture: 70,
      highTemp: 35.0,
      duration: 10,
      cooldown: 30
    };

    let env = {
      moisture: 48,
      temperature: 24.8,
      humidity: 64.0,
      lightPercent: 74,
      lightVoltage: 2.44,
      goodLed: true,
      badLed: false,
      buzzer: false
    };

    let eventLogs = [];

    function toggleDebugMode() {
      debugMode = !debugMode;
      const sw = document.getElementById('toggleDebugSwitch');
      const panel = document.getElementById('debugPanel');
      const tags = document.querySelectorAll('.debug-tag');

      if (debugMode) {
        sw.classList.add('active');
        panel.classList.remove('hidden');
        tags.forEach(t => t.classList.remove('hidden'));
        showToast('Debug Diagnostics Enabled');
      } else {
        sw.classList.remove('active');
        panel.classList.add('hidden');
        tags.forEach(t => t.classList.add('hidden'));
        showToast('Debug Diagnostics Hidden');
      }
    }

    function toggleAudioBuzzer() {
      if (!audioCtx) {
        audioCtx = new (window.AudioContext || window.webkitAudioContext)();
      }
      audioEnabled = !audioEnabled;
      const btn = document.getElementById('btnSound');
      if (audioEnabled) {
        btn.classList.add('text-[#c9653b]');
        showToast('Audible Safety Alarm Enabled');
        playTone(880, 0.1);
      } else {
        btn.classList.remove('text-[#c9653b]');
        showToast('Safety Alarm Muted');
      }
    }

    function playTone(freq = 1200, duration = 0.15) {
      if (!audioEnabled || !audioCtx) return;
      try {
        if (audioCtx.state === 'suspended') audioCtx.resume();
        const osc = audioCtx.createOscillator();
        const gain = audioCtx.createGain();
        osc.type = 'triangle';
        osc.frequency.setValueAtTime(freq, audioCtx.currentTime);
        gain.gain.setValueAtTime(0.06, audioCtx.currentTime);
        gain.gain.exponentialRampToValueAtTime(0.0001, audioCtx.currentTime + duration);
        osc.connect(gain);
        gain.connect(audioCtx.destination);
        osc.start();
        osc.stop(audioCtx.currentTime + duration);
      } catch (e) {
        console.error(e);
      }
    }

    function toggleEngineMode() {
      isConnectedToHardware = !isConnectedToHardware;
      const btn = document.getElementById('btnMode');
      const txt = document.getElementById('txtMode');
      if (isConnectedToHardware) {
        txt.innerText = 'LIVE ESP32 CONNECTED';
        btn.className = 'nm-press-btn px-3.5 py-2 text-xs font-semibold flex items-center gap-2 text-[#3d7051]';
        showToast('Switched to Hardware Polling Mode');
        fetchData();
      } else {
        txt.innerText = 'SIMULATION PREVIEW';
        btn.className = 'nm-press-btn px-3.5 py-2 text-xs font-semibold flex items-center gap-2 text-sky-600';
        showToast('Switched to Local Simulation Preview');
      }
    }

    function renderEventLogs() {
      const container = document.getElementById('eventListContainer');
      const counter = document.getElementById('logCounter');
      counter.innerText = `${eventLogs.length} entries`;
      
      if (eventLogs.length === 0) {
        container.innerHTML = '<div class="text-[#5e7064] text-center py-4">No events logged yet.</div>';
        return;
      }

      container.innerHTML = eventLogs.map(log => `
        <div class="flex items-start gap-2.5 py-1.5 border-b border-black/[0.06]">
          <span class="text-[#5e7064] font-mono shrink-0">${log.time}</span>
          <span class="nm-sunken-sm px-1.5 py-0.5 text-[10px] font-bold text-[#3d7051] shrink-0">${log.source}</span>
          <span class="text-[#233329]">${log.text}</span>
        </div>
      `).join('');
      container.scrollTop = container.scrollHeight;
    }

    function addLog(source, text) {
      const h = Math.floor(uptimeSeconds / 3600);
      const m = Math.floor((uptimeSeconds % 3600) / 60);
      const s = uptimeSeconds % 60;
      const timeStr = `${String(h).padStart(2,'0')}:${String(m).padStart(2,'0')}:${String(s).padStart(2,'0')}`;
      eventLogs.push({ time: timeStr, source, text });
      if (eventLogs.length > 30) eventLogs.shift();
      renderEventLogs();
    }

    function clearEventLogs() {
      eventLogs = [];
      renderEventLogs();
      showToast('Event History Cleared');
    }

    function showToast(msg, type = 'success') {
      const toast = document.getElementById('toast');
      const toastMsg = document.getElementById('toastMessage');
      const toastDot = document.getElementById('toastDot');
      toastMsg.innerText = msg;
      toastDot.className = `w-2.5 h-2.5 rounded-full shadow-sm ${type === 'danger' ? 'bg-rose-600' : 'bg-emerald-600'}`;
      toast.classList.remove('opacity-0', 'translate-y-4');
      setTimeout(() => {
        toast.classList.add('opacity-0', 'translate-y-4');
      }, 3000);
    }

    function updateThresholdSlider(key, val) {
      if (key === 'minMoist') {
        thresholds.minMoisture = parseInt(val);
        document.getElementById('lblMinMoist').innerText = val;
      } else if (key === 'maxMoist') {
        thresholds.maxMoisture = parseInt(val);
        document.getElementById('lblMaxMoist').innerText = val;
      } else if (key === 'highTemp') {
        thresholds.highTemp = parseFloat(val);
        document.getElementById('lblHighTemp').innerText = val;
      } else if (key === 'cooldown') {
        thresholds.cooldown = parseInt(val);
        document.getElementById('lblCooldown').innerText = val;
      }
    }

    async function commitThresholds() {
      if (thresholds.minMoisture >= thresholds.maxMoisture) {
        showToast('Min Moisture must be strictly less than Max Moisture!', 'danger');
        return;
      }

      try {
        const res = await fetch('/api/thresholds', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({
            minMoisture: thresholds.minMoisture,
            maxMoisture: thresholds.maxMoisture,
            highTemp: thresholds.highTemp,
            duration: thresholds.duration,
            cooldown: thresholds.cooldown,
            autoMode: autoMode
          })
        });
        const data = await res.json();
        showToast(data.message || 'Saved to Persistent Storage');
        fetchData();
      } catch (e) {
        if (!isConnectedToHardware) {
          addLog('STORAGE', `Saved parameters: Min=${thresholds.minMoisture}%, Max=${thresholds.maxMoisture}%, Temp=${thresholds.highTemp}°C`);
          showToast('Parameters saved (Simulation)');
        } else {
          showToast('Failed to reach Device: ' + e.message, 'danger');
        }
      }
    }

    async function toggleAutoMode() {
      autoMode = !autoMode;
      const sw = document.getElementById('toggleAuto');
      if (autoMode) {
        sw.classList.add('active');
        showToast('Auto-Hydration Enabled');
      } else {
        sw.classList.remove('active');
        showToast('Auto-Hydration Paused');
      }

      try {
        await fetch('/api/thresholds', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ autoMode })
        });
        fetchData();
      } catch (err) {
        if (!isConnectedToHardware) {
          addLog('AUTO_LOOP', autoMode ? 'Closed-loop automation ENGAGED' : 'Closed-loop automation PAUSED');
        }
      }
    }

    async function triggerManualIrrigation() {
      const dur = parseInt(document.getElementById('inpManualDuration').value) || 10;
      if (dur < 1 || dur > 60) {
        showToast('Duration must be between 1 and 60 seconds', 'danger');
        return;
      }

      try {
        const res = await fetch('/api/irrigate', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ duration: dur })
        });
        const data = await res.json();
        showToast(data.message || 'Pump cycle started');
        fetchData();
      } catch (e) {
        if (!isConnectedToHardware) {
          startPumpSimulation(dur, 'MANUAL_DASHBOARD');
        } else {
          showToast('Command failed: ' + e.message, 'danger');
        }
      }
    }

    async function emergencyStopPump() {
      try {
        const res = await fetch('/api/irrigate/stop', { method: 'POST' });
        const data = await res.json();
        showToast(data.message || 'Pump Halted', 'danger');
        fetchData();
      } catch (e) {
        if (!isConnectedToHardware) {
          pumpActive = false;
          pumpRemaining = 0;
          cooldownRemaining = thresholds.cooldown;
          addLog('SAFETY_CUTOFF', 'EMERGENCY HALT: Actuator stopped');
          showToast('Pump Stopped Immediately', 'danger');
        } else {
          showToast('Emergency cutoff failed: ' + e.message, 'danger');
        }
      }
    }

    function startPumpSimulation(duration, source = 'AUTO_CLOSED_LOOP') {
      if (pumpActive) return;
      pumpActive = true;
      pumpTotal = duration;
      pumpRemaining = duration;
      cooldownRemaining = 0;
      addLog('ACTUATOR_ON', `Pump active. Hydrating for ${duration}s via ${source}`);
      showToast(`Irrigation Active (${duration}s cycle)`);
    }

    // REAL HARDWARE FETCH FUNCTION
    async function fetchData() {
      if (!isConnectedToHardware) {
        simulationTick();
        return;
      }

      try {
        const res = await fetch('/api/data', { cache: 'no-store' });
        if (!res.ok) throw new Error('HTTP ' + res.status);
        const data = await res.json();
        failCount = 0;

        document.getElementById('statusDot').className = 'w-2 h-2 rounded-full bg-emerald-500 shadow-sm';
        document.getElementById('badgeWifi').innerText = data.system.wifiMode || 'Network Online';
        document.getElementById('footHeap').innerText = Math.round(data.system.freeHeap / 1024) + ' KB';
        uptimeSeconds = data.system.uptime || uptimeSeconds;
        const h = Math.floor(uptimeSeconds / 3600);
        const m = Math.floor((uptimeSeconds % 3600) / 60);
        const s = uptimeSeconds % 60;
        document.getElementById('footUptime').innerText = `${String(h).padStart(2,'0')}:${String(m).padStart(2,'0')}:${String(s).padStart(2,'0')}`;

        // Irrigation state
        const irr = data.irrigation;
        autoMode = irr.autoMode;
        document.getElementById('toggleAuto').classList.toggle('active', autoMode);

        pumpActive = (irr.state === 'Irrigating');
        pumpRemaining = irr.remainingSeconds || 0;
        pumpTotal = irr.totalDuration || 10;
        cooldownRemaining = irr.cooldownRemaining || 0;
        cooldownTotal = irr.totalCooldown || 30;

        if (pumpActive) {
          const prog = ((pumpTotal - pumpRemaining) / pumpTotal) * 100;
          document.getElementById('pumpProgressBar').style.width = Math.max(5, prog) + '%';
          document.getElementById('countdownText').innerText = `${pumpRemaining}s Remaining`;
          document.getElementById('pumpDetailText').innerText = 'Actuator Active • Hydrating soil roots...';
        } else if (cooldownRemaining > 0) {
          const cProg = ((cooldownTotal - cooldownRemaining) / cooldownTotal) * 100;
          document.getElementById('pumpProgressBar').style.width = Math.max(5, cProg) + '%';
          document.getElementById('countdownText').innerText = `${cooldownRemaining}s Soak Delay`;
          document.getElementById('pumpDetailText').innerText = 'Soaking & Motor Rest (Cooldown)...';
        } else {
          document.getElementById('pumpProgressBar').style.width = '0%';
          document.getElementById('countdownText').innerText = '0s Remaining';
          document.getElementById('pumpDetailText').innerText = 'Hydration Loop Resting • Soil Stable';
        }

        // Threshold inputs sync
        thresholds.minMoisture = data.thresholds.minMoisture;
        thresholds.maxMoisture = data.thresholds.maxMoisture;
        thresholds.highTemp = data.thresholds.highTemp;
        thresholds.duration = data.thresholds.duration;
        thresholds.cooldown = data.thresholds.cooldown;

        document.getElementById('sliderMinMoist').value = thresholds.minMoisture;
        document.getElementById('lblMinMoist').innerText = thresholds.minMoisture;
        document.getElementById('sliderMaxMoist').value = thresholds.maxMoisture;
        document.getElementById('lblMaxMoist').innerText = thresholds.maxMoisture;
        document.getElementById('sliderHighTemp').value = thresholds.highTemp;
        document.getElementById('lblHighTemp').innerText = thresholds.highTemp;
        document.getElementById('sliderCooldown').value = thresholds.cooldown;
        document.getElementById('lblCooldown').innerText = thresholds.cooldown;

        // Events sync
        if (data.events && data.events.length > 0) {
          eventLogs = data.events;
          renderEventLogs();
        }

        updateDashboardUI({
          soilMoisture: data.soilMoisture,
          temperature: data.temperature,
          humidity: data.humidity,
          light: data.light,
          indicators: data.indicators || {
            goodState: (data.soilMoisture.percent >= thresholds.minMoisture),
            buzzerActive: (data.soilMoisture.percent < thresholds.minMoisture)
          },
          irrigation: data.irrigation
        });

      } catch (err) {
        failCount++;
        if (failCount >= 3) {
          document.getElementById('statusDot').className = 'w-2 h-2 rounded-full bg-amber-500 shadow-sm';
          document.getElementById('badgeWifi').innerText = 'Reconnecting...';
        }
        // Fallback simulation tick to keep visuals alive
        simulationTick();
      }
    }

    // FALLBACK SIMULATION TICK
    function simulationTick() {
      uptimeSeconds++;
      const h = Math.floor(uptimeSeconds / 3600);
      const m = Math.floor((uptimeSeconds % 3600) / 60);
      const s = uptimeSeconds % 60;
      document.getElementById('footUptime').innerText = `${String(h).padStart(2,'0')}:${String(m).padStart(2,'0')}:${String(s).padStart(2,'0')}`;

      env.temperature += (Math.random() - 0.5) * 0.12;
      env.temperature = Math.max(18, Math.min(42, env.temperature));
      
      env.humidity += (Math.random() - 0.5) * 0.25;
      env.humidity = Math.max(30, Math.min(95, env.humidity));

      env.lightPercent += Math.floor((Math.random() - 0.5) * 3);
      env.lightPercent = Math.max(10, Math.min(98, env.lightPercent));
      env.lightVoltage = 3.3 * (env.lightPercent / 100);

      if (pumpActive) {
        pumpRemaining--;
        env.moisture = Math.min(thresholds.maxMoisture + 4, env.moisture + 3.8);
        const progress = ((pumpTotal - pumpRemaining) / pumpTotal) * 100;
        document.getElementById('pumpProgressBar').style.width = `${progress}%`;
        document.getElementById('countdownText').innerText = `${pumpRemaining}s Remaining`;
        document.getElementById('pumpDetailText').innerText = 'Actuator Active • Hydrating soil roots...';

        if (pumpRemaining <= 0) {
          pumpActive = false;
          cooldownRemaining = thresholds.cooldown;
          cooldownTotal = thresholds.cooldown;
          addLog('ACTUATOR_OFF', `Irrigation cycle complete. Moisture: ${Math.round(env.moisture)}%`);
          showToast('Irrigation Finished. Soil soaking.');
        }
      } else if (cooldownRemaining > 0) {
        cooldownRemaining--;
        const cProg = ((cooldownTotal - cooldownRemaining) / cooldownTotal) * 100;
        document.getElementById('pumpProgressBar').style.width = `${cProg}%`;
        document.getElementById('countdownText').innerText = `${cooldownRemaining}s Soak Delay`;
        document.getElementById('pumpDetailText').innerText = 'Soaking & Motor Rest (Cooldown)...';
      } else {
        env.moisture -= 0.22;
        env.moisture = Math.max(14, env.moisture);

        document.getElementById('pumpProgressBar').style.width = '0%';
        document.getElementById('countdownText').innerText = '0s Remaining';
        document.getElementById('pumpDetailText').innerText = 'Hydration Loop Resting • Soil Stable';

        if (autoMode && env.moisture < thresholds.minMoisture) {
          addLog('CLOSED_LOOP', `Moisture (${Math.round(env.moisture)}%) < ${thresholds.minMoisture}%. Starting auto-irrigation.`);
          startPumpSimulation(thresholds.duration, 'AUTO_CLOSED_LOOP');
        }
      }

      if (env.moisture < thresholds.minMoisture) {
        env.goodLed = false;
        env.badLed = true;
        env.buzzer = true;
        playTone(1350, 0.12);
      } else {
        env.goodLed = true;
        env.badLed = false;
        env.buzzer = false;
      }

      updateDashboardUI({
        soilMoisture: {
          percent: Math.round(env.moisture),
          raw: Math.round((env.moisture / 100) * 1800)
        },
        temperature: env.temperature,
        humidity: env.humidity,
        light: {
          percent: env.lightPercent,
          voltage: env.lightVoltage
        },
        indicators: {
          goodState: env.goodLed,
          buzzerActive: env.buzzer
        },
        irrigation: {
          active: pumpActive,
          remaining: pumpRemaining,
          cooldown: cooldownRemaining
        }
      });
    }

    // UPDATE UI WITH DIRECT BOX COLOR FILLS
    function updateDashboardUI(data) {
      // 1. Soil Moisture
      const mVal = data.soilMoisture.percent;
      document.getElementById('valMoisture').innerText = mVal;
      document.getElementById('debugMoistRaw').innerText = `ADC: ${data.soilMoisture.raw}`;
      
      const boxM = document.getElementById('boxFillMoisture');
      boxM.style.height = `${mVal}%`;

      const mState = document.getElementById('badgeMoistureState');
      const healthTitle = document.getElementById('plantHealthTitle');
      const healthDesc = document.getElementById('plantHealthDesc');
      const vitalityPct = document.getElementById('vitalityPctLabel');
      const vitalityBar = document.getElementById('vitalityFillBar');
      const leafLeft = document.getElementById('leafLeft');
      const leafRight = document.getElementById('leafRight');

      if (mVal < thresholds.minMoisture) {
        mState.innerText = 'PARCHED / LOW';
        mState.className = 'font-bold text-rose-600';
        healthTitle.innerText = 'Soil Parched (Thirsty)';
        healthTitle.className = 'text-xl sm:text-2xl font-extrabold text-rose-700';
        healthDesc.innerText = 'Moisture has dropped below trigger limits. Roots require immediate hydration.';
        
        vitalityPct.innerText = '52%';
        vitalityPct.className = 'text-sm font-extrabold text-rose-600';
        vitalityBar.style.width = '52%';
        vitalityBar.className = 'h-full rounded-full bg-gradient-to-r from-rose-500 to-amber-500 transition-all duration-500';
        
        boxM.className = 'w-full transition-all duration-700 ease-out bg-gradient-to-t from-rose-500/55 to-amber-500/35 border-t-2 border-rose-500/70';

        leafLeft.setAttribute('transform', 'rotate(15 50 48)');
        leafRight.setAttribute('transform', 'rotate(-15 50 40)');
        leafLeft.setAttribute('fill', '#9a8d5b');
      } else if (mVal > thresholds.maxMoisture) {
        mState.innerText = 'SATURATED';
        mState.className = 'font-bold text-sky-600';
        healthTitle.innerText = 'Soil Saturated';
        healthTitle.className = 'text-xl sm:text-2xl font-extrabold text-sky-700';
        healthDesc.innerText = 'Hydration is abundant. Actuator cutoff engaged to prevent overwatering.';

        vitalityPct.innerText = '88%';
        vitalityPct.className = 'text-sm font-extrabold text-sky-600';
        vitalityBar.style.width = '88%';
        vitalityBar.className = 'h-full rounded-full bg-gradient-to-r from-sky-500 to-emerald-500 transition-all duration-500';

        boxM.className = 'w-full transition-all duration-700 ease-out bg-gradient-to-t from-sky-500/55 to-emerald-500/35 border-t-2 border-sky-500/70';

        leafLeft.removeAttribute('transform');
        leafRight.removeAttribute('transform');
        leafLeft.setAttribute('fill', '#52966e');
      } else {
        mState.innerText = 'ADEQUATE';
        mState.className = 'font-bold text-[#3d7051]';
        healthTitle.innerText = 'Flourishing & Healthy';
        healthTitle.className = 'text-xl sm:text-2xl font-extrabold text-[#233329]';
        healthDesc.innerText = 'Root moisture is well-balanced within the optimal 30%–70% absorption range.';

        vitalityPct.innerText = '96%';
        vitalityPct.className = 'text-sm font-extrabold text-[#3d7051]';
        vitalityBar.style.width = '96%';
        vitalityBar.className = 'h-full rounded-full bg-gradient-to-r from-[#3d7051] to-[#10b981] transition-all duration-500';

        boxM.className = 'w-full transition-all duration-700 ease-out bg-gradient-to-t from-[#3d7051]/55 to-[#52966e]/35 border-t-2 border-[#3d7051]/70';

        leafLeft.removeAttribute('transform');
        leafRight.removeAttribute('transform');
        leafLeft.setAttribute('fill', '#52966e');
      }

      // 2. Temperature
      const tVal = data.temperature;
      document.getElementById('valTemp').innerText = tVal.toFixed(1);
      const tPercent = Math.min(100, Math.max(0, (tVal / 50) * 100));
      
      const boxT = document.getElementById('boxFillTemp');
      boxT.style.height = `${tPercent}%`;

      const tState = document.getElementById('badgeTempState');
      if (tVal >= thresholds.highTemp) {
        tState.innerText = 'HEAT ALERT';
        tState.className = 'font-bold text-rose-600';
        boxT.className = 'w-full transition-all duration-700 ease-out bg-gradient-to-t from-rose-500/55 to-amber-500/35 border-t-2 border-rose-500/70';
      } else {
        tState.innerText = 'TEMPERATE';
        tState.className = 'font-bold text-[#c9653b]';
        boxT.className = 'w-full transition-all duration-700 ease-out bg-gradient-to-t from-[#c9653b]/55 to-[#e59837]/35 border-t-2 border-[#c9653b]/70';
      }

      // 3. Humidity
      const hVal = Math.round(data.humidity);
      document.getElementById('valHumidity').innerText = hVal;
      document.getElementById('boxFillHumidity').style.height = `${hVal}%`;

      // 4. Sunlight
      const lVal = Math.round(data.light.percent);
      document.getElementById('valLight').innerText = lVal;
      document.getElementById('boxFillLight').style.height = `${lVal}%`;
      document.getElementById('debugLightVolt').innerText = data.light.voltage.toFixed(2) + 'V';

      // Hardware LEDs
      const ledG = document.getElementById('ledGood');
      const ledB = document.getElementById('ledBad');
      const buzIcon = document.getElementById('buzzerIcon');
      const buzTitle = document.getElementById('buzzerTitle');

      if (data.indicators.goodState) {
        ledG.className = 'led-bulb led-sprout-on';
        ledB.className = 'led-bulb';
      } else {
        ledG.className = 'led-bulb';
        ledB.className = 'led-bulb led-alert-on';
      }

      if (data.indicators.buzzerActive) {
        buzIcon.classList.add('text-amber-500', 'animate-bounce');
        buzTitle.innerText = 'ALARM BEEPING';
        buzTitle.classList.add('text-amber-600');
      } else {
        buzIcon.classList.remove('text-amber-500', 'animate-bounce');
        buzTitle.innerText = 'BUZZER IDLE';
        buzTitle.classList.remove('text-amber-600');
      }

      // Pump Buttons & State Badge
      const badge = document.getElementById('pumpStateBadge');
      const pumpIcon = document.getElementById('pumpIconBox');
      const runBtn = document.getElementById('btnRunPump');
      const stopBtn = document.getElementById('btnEmergencyStop');

      if (pumpActive) {
        badge.innerText = 'STATUS: IRRIGATING';
        badge.className = 'nm-sunken-sm px-3.5 py-1.5 text-xs font-bold tracking-wider text-sky-600';
        pumpIcon.classList.add('text-sky-600', 'animate-pulse');
        runBtn.disabled = true;
        runBtn.classList.add('opacity-50');
        stopBtn.disabled = false;
        stopBtn.classList.remove('opacity-50');
      } else if (cooldownRemaining > 0) {
        badge.innerText = 'STATUS: SOAKING';
        badge.className = 'nm-sunken-sm px-3.5 py-1.5 text-xs font-bold tracking-wider text-[#c9653b]';
        pumpIcon.classList.remove('text-sky-600', 'animate-pulse');
        runBtn.disabled = false;
        runBtn.classList.remove('opacity-50');
        stopBtn.disabled = true;
        stopBtn.classList.add('opacity-50');
      } else {
        badge.innerText = 'STATUS: IDLE';
        badge.className = 'nm-sunken-sm px-3.5 py-1.5 text-xs font-bold tracking-wider text-[#5e7064]';
        pumpIcon.classList.remove('text-sky-600', 'animate-pulse');
        runBtn.disabled = false;
        runBtn.classList.remove('opacity-50');
        stopBtn.disabled = true;
        stopBtn.classList.add('opacity-50');
      }
    }

    window.addEventListener('DOMContentLoaded', () => {
      fetchData();
      pollTimer = setInterval(fetchData, 1800);
    });
  </script>
</body>
</html>
)rawliteral";

#endif // DASHBOARD_HTML_H
