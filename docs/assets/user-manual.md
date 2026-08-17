---
pdf_options:
  format: A4
  margin:
    top: 0
    right: 0
    bottom: 0
    left: 0
  printBackground: true
  displayHeaderFooter: false
---

<style>
/* CSS for premium A4 User Manual PDF */

/* Global styles */
body, .markdown-body {
  font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
  color: #1e293b !important;
  background-color: #ffffff !important;
  line-height: 1.6;
  font-size: 11pt;
}

h1, h2, h3, h4 {
  color: #0f172a !important;
  font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
  font-weight: 600;
  margin-top: 1.2em;
  margin-bottom: 0.4em;
}

h1 {
  font-size: 22pt;
  border-bottom: 2px solid #cbd5e1;
  padding-bottom: 0.3em;
  margin-top: 0;
  text-transform: uppercase;
}

h2 {
  font-size: 16pt;
  border-bottom: 1px solid #e2e8f0;
  padding-bottom: 0.2em;
}

h3 {
  font-size: 12pt;
}

p, li {
  color: #334155;
  font-size: 11pt;
}

ul, ol {
  margin-top: 0.4em;
  margin-bottom: 0.8em;
  padding-left: 1.5em;
}

li {
  margin-bottom: 0.3em;
}

a {
  color: #0284c7 !important;
  text-decoration: none;
}

a:hover {
  text-decoration: underline;
}

/* Code and syntax highlighting */
code {
  background-color: #f1f5f9 !important;
  color: #0f172a !important;
  padding: 0.2em 0.4em;
  border-radius: 4px;
  font-family: ui-monospace, SFMono-Regular, SF Mono, Menlo, Consolas, monospace;
  font-size: 9pt;
}

pre {
  background-color: #f8fafc !important;
  border: 1px solid #e2e8f0;
  padding: 12px;
  border-radius: 6px;
  overflow: auto;
  margin: 1em 0;
}

pre code {
  background-color: transparent !important;
  padding: 0;
  font-size: 9pt;
}

/* Table styles */
table {
  border-collapse: collapse;
  width: 100%;
  margin: 1.2em 0;
  font-size: 9.5pt;
}

th, td {
  padding: 8px 10px;
  border: 1px solid #cbd5e1 !important;
  text-align: left;
}

th {
  background-color: #f1f5f9 !important;
  color: #0f172a !important;
  font-weight: 600;
}

tr:nth-child(even) {
  background-color: #f8fafc !important;
}

/* Page structural styles */
.cover-page {
  width: 100vw;
  height: 100vh;
  box-sizing: border-box;
  background-image: linear-gradient(rgba(0, 0, 0, 0.35), rgba(0, 0, 0, 0.35)), url('./drone.jpg');
  background-size: cover;
  background-position: center;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  padding: 0 20mm;
  color: #ffffff !important;
  text-align: center;
  break-after: page;
  page-break-after: always;
}

.cover-brand {
  font-size: 16pt;
  font-weight: 300;
  color: #ffffff !important;
  letter-spacing: 4px;
  text-transform: uppercase;
  margin-bottom: 20px;
  margin-top: 0;
}

.cover-title-group h1.cover-title {
  font-size: 92pt !important;
  color: #ffffff !important;
  font-weight: 800;
  letter-spacing: 2px;
  border-bottom: none;
  margin: 0;
  padding: 0;
}

.cover-subtitle {
  font-size: 20pt;
  font-weight: 300;
  margin-top: 15px;
  color: #e2e8f0;
}

.toc-page, .page-body {
  width: 100vw;
  height: 100vh;
  box-sizing: border-box;
  margin: 0;
  padding: 20mm 30mm 30mm 30mm; /* Extra bottom space for footer */
  position: relative;
  break-after: page;
  page-break-after: always;
  background-color: #ffffff;
}

/* Table of Contents page specific styles */
.toc-heading {
  font-size: 22pt;
  margin-top: 0;
  margin-bottom: 1.5em;
  border-bottom: 2px solid #0f172a;
  padding-bottom: 0.4em;
  text-transform: uppercase;
}

.toc-list {
  list-style: none;
  padding: 0;
  margin: 0;
}

.toc-list li {
  display: flex;
  align-items: baseline;
  margin-bottom: 16px;
  font-size: 12pt;
}

.toc-title {
  font-weight: 500;
  color: #334155;
}

.toc-dots {
  flex-grow: 1;
  border-bottom: 1px dotted #94a3b8;
  margin: 0 12px;
  position: relative;
  top: -4px;
}

.toc-page-num {
  font-weight: 600;
  color: #0f172a;
}

/* Custom footer style */
.page-footer {
  position: absolute;
  bottom: 15mm;
  left: 20mm;
  right: 20mm;
  display: flex;
  justify-content: space-between;
  font-size: 8pt;
  color: #64748b;
  font-family: -apple-system, BlinkMacSystemFont, sans-serif;
  border-top: 1px solid #e2e8f0;
  padding-top: 8px;
}

/* Printing styling to enforce A4 sizing and zero margins */
@media print {
  @page {
    size: A4;
    margin: 0; /* Let HTML containers specify boundaries */
  }

  body, .markdown-body {
    background-color: #ffffff !important;
    padding: 0 !important;
    margin: 0 !important;
  }

  .cover-page, .toc-page, .page-body {
    width: 210mm;
    height: 297mm;
    margin: 0;
    box-sizing: border-box;
  }
}
</style>

<div class="cover-page">
  <div class="cover-title-group">
    <p class="cover-brand">Ghost In Translation</p>
    <h1 class="cover-title">DRONE</h1>
    <p class="cover-subtitle">3.0.X</p>
  </div>
</div>

<div class="toc-page">
  <h2 class="toc-heading">Table of Contents</h2>
  <ul class="toc-list">
    <li>
      <span class="toc-title">1. Introduction</span>
      <span class="toc-dots"></span>
      <span class="toc-page-num">3</span>
    </li>
    <li>
      <span class="toc-title">2. Controls and CV Inputs</span>
      <span class="toc-dots"></span>
      <span class="toc-page-num">4</span>
    </li>
    <li>
      <span class="toc-title">3. Menu & Navigation</span>
      <span class="toc-dots"></span>
      <span class="toc-page-num">5</span>
    </li>
    <li>
      <span class="toc-title">4. The 16 Algorithms</span>
      <span class="toc-dots"></span>
      <span class="toc-page-num">6</span>
    </li>
    <li>
      <span class="toc-title">5. Built-in Looper</span>
      <span class="toc-dots"></span>
      <span class="toc-page-num">8</span>
    </li>
    <li>
      <span class="toc-title">6. MIDI Implementation</span>
      <span class="toc-dots"></span>
      <span class="toc-page-num">9</span>
    </li>
    <li>
      <span class="toc-title">7. Firmware Update</span>
      <span class="toc-dots"></span>
      <span class="toc-page-num">10</span>
    </li>
    <li>
      <span class="toc-title">8. Troubleshooting</span>
      <span class="toc-dots"></span>
      <span class="toc-page-num">11</span>
    </li>
  </ul>
  <div class="page-footer">
    <span>Drone User Manual</span>
    <span>Page 2</span>
  </div>
</div>

<div class="page-body">
  <h1>1. Introduction</h1>

  <p>Drone is a multi-algorithm drone module based on my Motherboard platform.</p>

  <h2>Features</h2>

  <ul>
    <li>4 individual voices</li>
    <li>16 algorithms</li>
    <li>32 bit float DSP</li>
    <li>Stereo audio (48kHz, 32-bit)</li>
    <li>MIDI TRS Type A IN and OUT</li>
    <li>Auto calibration</li>
    <li>Either-way power</li>
  </ul>
  
  <h2>Technical Specifications</h1>

  <ul>
    <li><strong>Module Width</strong>: 8 HP</li>
    <li><strong>Depth</strong>: 50 mm</li>
    <li><strong>Power Consumption</strong>:
      <ul>
        <li><strong>+12V</strong>: 75 mA</li>
        <li><strong>-12V</strong>: 3.5 mA</li>
      </ul>
    </li>
    <li><strong>CV Input Range</strong>: 0V to 5V</li>
  </ul>
  
  <div class="page-footer">
    <span>Drone User Manual</span>
    <span>Page 3</span>
  </div>
</div>

<div class="page-body">
  <h1>2. Controls and CV Inputs</h1>

  <p>The front panel features controls for adjusting the tuning, timbral shape, and volume mix of the 4 voices.</p>

  <p>All CV inputs read voltages between 0 and 5V.</p>

  <h2>Knobs</h2>
  <ul>
    <li><strong>A B C D</strong>: Sets the base pitch/tuning for each of the four individual voices.</li>
    <li><strong>MIX</strong>: Turning this knob morphs the volumes of the four voices in a spiral motion.</li>
    <li><strong>SHAPE</strong>: Adjusts the main character of the sound (such as morphing wavetables, filter sweeps, or FM intensity).</li>
  </ul>

  <h2>CV Inputs</h2>
  <ul>
    <li><strong>CV A, B, C, D</strong>: 1V/Octave pitch CV inputs for each voice.</li>
    <li><strong>CV MIX</strong>: Modulates the Mix parameter.</li>
    <li><strong>CV SHAPE</strong>: Modulates the Shape parameter.</li>
    <li><strong>CV FM</strong>: Linear frequency modulation for all four voices.</li>
  </ul>

  <h2>Navigation Button</h2>
  <ul>
    <li><strong>Mode button</strong>:
      <ul>
        <li><strong>Short Press</strong>: Enters Algorithm Selection Mode (when in normal play mode) or cycles to the next algorithm (when in selection mode).</li>
        <li><strong>Long Press (1 second)</strong>: Toggles Looper Mode (enters or exits).</li>
      </ul>
    </li>
  </ul>
  
  <div class="page-footer">
    <span>Drone User Manual</span>
    <span>Page 4</span>
  </div>
</div>

<div class="page-body">
  <h1>3. Menu & Navigation</h1>

  <h2>Normal Play Mode</h2>
  <p>In normal play mode:</p>
  <ul>
    <li>The <strong>8 LEDs at the top</strong> display real-time levels of the control inputs and the main audio output.</li>
    <li>The <strong>4 LEDs in the middle</strong> show the volume of each of the four voices.</li>
  </ul>

  <h2>Algorithm Selection Mode</h2>
  <p>To change the active sound algorithm:</p>
  <ol>
    <li><strong>Enter Menu</strong>: Short press the <strong>Mode button</strong>. All 8 top LEDs will turn solid <strong>Blue</strong>, except for the active selection.</li>
    <li><strong>Read Active Selection</strong>: The active selection is shown by two blinking <strong>Green</strong> LEDs:
      <ul>
        <li><strong>Bank Indicator</strong> (one of the first 4 LEDs)</li>
        <li><strong>Algorithm Indicator</strong> (one of the last 4 LEDs)</li>
      </ul>
    </li>
    <li><strong>Change Algorithm</strong>: Tap the <strong>Mode button</strong> to cycle to the next algorithm.</li>
    <li><strong>Save & Exit</strong>: After <strong>5 seconds</strong> of inactivity, the module saves your choice and returns to play mode automatically.</li>
  </ol>
  
  <div class="page-footer">
    <span>Drone User Manual</span>
    <span>Page 5</span>
  </div>
</div>

<div class="page-body">
  <h1>4. The 16 Algorithms</h1>

  <p>The module contains 16 algorithms. Below is an overview of what each algorithm does and what the <strong>SHAPE</strong> knob controls.</p>

  <h2>Bank 1</h2>
  <table>
    <thead>
      <tr>
        <th style="width: 8%">#</th>
        <th style="width: 46%">Sound Description</th>
        <th style="width: 46%">What the <strong>SHAPE</strong> Control Does</th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td>1</td>
        <td><strong>Sine-Resonant Noise</strong>: A mix of warm, pure sine waves and soft resonant noise.</td>
        <td>Blends from pure sine tones (0%) to wind-like resonant noise (100%).</td>
      </tr>
      <tr>
        <td>2</td>
        <td><strong>Resonant Noise</strong>: Filters white noise into whistle-like pitched tones.</td>
        <td>Sets Q-resonance. Turn up to morph from ocean-wave wind sounds to sharp, whistling pitches.</td>
      </tr>
      <tr>
        <td>3</td>
        <td><strong>Sine-Phaser-Ring</strong>: Stereo phase-shifted sines combined with ring modulation.</td>
        <td>Sets Ring Modulator speed (60Hz to 160Hz) to add metallic, sci-fi buzzes.</td>
      </tr>
      <tr>
        <td>4</td>
        <td><strong>Sine-FM</strong>: Warm frequency modulation synthesis.</td>
        <td>Sets modulator speed and modulation depth, introducing growling sidebands and metallic overtones.</td>
      </tr>
    </tbody>
  </table>

  <h2>Bank 2</h2>
  <table>
    <thead>
      <tr>
        <th style="width: 8%">#</th>
        <th style="width: 46%">Sound Description</th>
        <th style="width: 46%">What the <strong>SHAPE</strong> Control Does</th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td>1</td>
        <td><strong>Saw-Comparator</strong>: Gated, aggressive digital saw waves.</td>
        <td>Sets the clipping threshold on the sum of the 4 saw waves.</td>
      </tr>
      <tr>
        <td>2</td>
        <td><strong>Slink Filter</strong>: 8 parallel resonant bandpass filters swept by a complex LFO.</td>
        <td>Adjusts sweep rate, ripple spacing, and filter density for vocal "formant" sweeps.</td>
      </tr>
      <tr>
        <td>3</td>
        <td><strong>FM & Wavefolder</strong>: Aggressive, harmonically rich wavefolded FM voice.</td>
        <td>Drives FM depth and folding intensity to add sharp digital bite and buzz.</td>
      </tr>
      <tr>
        <td style="width: 8%">4</td>
        <td style="width: 46%"><strong>Karplus-Strong Plucks</strong>: Simulated plucked and bowed string resonances.</td>
        <td style="width: 46%">Changes the brightness/filtering of the plucking excitation. Morphs from dark thuds to metallic plucks.</td>
      </tr>
    </tbody>
  </table>
  
  <div class="page-footer">
    <span>Drone User Manual</span>
    <span>Page 6</span>
  </div>
</div>

<div class="page-body">
  <h2>Bank 3</h2>
  <table>
    <thead>
      <tr>
        <th style="width: 8%">#</th>
        <th style="width: 46%">Sound Description</th>
        <th style="width: 46%">What the <strong>SHAPE</strong> Control Does</th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td>1</td>
        <td><strong>Gong & Bells</strong>: Inharmonic 4-operator FM chain recreating metallic bells.</td>
        <td>Acts as modulation index. Morphs from simple sine tones to complex clangs and gong washes.</td>
      </tr>
      <tr>
        <td>2</td>
        <td><strong>Dust-Triangle</strong>: Warm triangle drone mixed with crackling rain impulses.</td>
        <td>Blends between crackling, pitched rain clicks (0%) and a clean, warm triangle wave drone (100%).</td>
      </tr>
      <tr>
        <td>3</td>
        <td><strong>PWM (Pulse-Width Modulation)</strong>: Thick, stereo-panned square waves with a custom foldback distortion waveshaper.</td>
        <td>Morphs the pulse width (duty cycle) of the square waves to change the harmonic profile.</td>
      </tr>
      <tr>
        <td>4</td>
        <td><strong>Resonant Saw & Saturation</strong>: Saturated sawtooth waves routed through a resonant lowpass filter.</td>
        <td>Sweeps filter cutoff (200Hz to 6kHz), increases filter resonance, and drives the saturation level.</td>
      </tr>
    </tbody>
  </table>

  <h2>Bank 4</h2>
  <table>
    <thead>
      <tr>
        <th style="width: 8%">#</th>
        <th style="width: 46%">Sound Description</th>
        <th style="width: 46%">What the <strong>SHAPE</strong> Control Does</th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td>1</td>
        <td><strong>Wavetable 1: "Octopus"</strong>: Complex 3D wavetable morphs with cascading modulation.</td>
        <td>Scans smoothly through 128 waves to change the fundamental timbre.</td>
      </tr>
      <tr>
        <td>2</td>
        <td><strong>Wavetable 2: "Bells"</strong>: Metallic bell-like wavetable morphs with cascading modulation.</td>
        <td>Scans smoothly through 128 waves to change the fundamental timbre.</td>
      </tr>
      <tr>
        <td>3</td>
        <td><strong>Wavetable 3: "Fly By One"</strong>: Vocal and sci-fi wavetable morphs with cascading modulation.</td>
        <td>Scans smoothly through 128 waves to change the fundamental timbre.</td>
      </tr>
      <tr>
        <td>4</td>
        <td><strong>Wavetable 4: "Yin Yang"</strong>: Deep, harmonically complex wavetable morphs.</td>
        <td>Scans smoothly through 128 waves to change the fundamental timbre.</td>
      </tr>
    </tbody>
  </table>
  
  <div class="page-footer">
    <span>Drone User Manual</span>
    <span>Page 7</span>
  </div>
</div>

<div class="page-body">
  <h1>5. Built-in Looper</h1>

  <p>Drone features a built-in Looper to record and loop knob movements (TUNE A-D, MIX, SHAPE) up to <strong>1 minute</strong>, or <strong>32 bars</strong> when synchronized to a MIDI clock.</p>

  <ul>
    <li><strong>Enter / Exit Looper</strong>: Hold the <strong>Mode button</strong> for <strong>1 second</strong>.</li>
    <li><strong>Start Recording</strong>: Short press the <strong>Mode button</strong> while in <strong>Idle</strong> (all 8 LEDs solid <strong>Red</strong>).</li>
    <li><strong>Stop & Loop</strong>: Short press the <strong>Mode button</strong> while recording (LEDs fill from <strong>Red to Blue</strong>). If MIDI clock is present, the loop stop is quantized to the next beat.</li>
    <li><strong>Reset / Clear</strong>: Short press the <strong>Mode button</strong> during Playback to erase the loop and return to Idle.</li>
    <li><strong>Overdub</strong>: Move any knob during Playback to record new values.</li>
    <li><strong>MIDI Sync</strong>: recording and Playback automatically sync to external MIDI clock. A MIDI Start message resets Playback to the start of the loop.</li>
  </ul>
  
  <div class="page-footer">
    <span>Drone User Manual</span>
    <span>Page 8</span>
  </div>
</div>

<div class="page-body">
  <h1>6. MIDI Implementation</h1>

  <p>Drone listens to and transmits on <strong>Channel 1</strong>.</p>

  <h2>Control Change (CC) Mapping</h2>
  <ul>
    <li><strong>CC 1</strong>: Voice A Tune</li>
    <li><strong>CC 2</strong>: Voice B Tune</li>
    <li><strong>CC 3</strong>: Voice C Tune</li>
    <li><strong>CC 4</strong>: Voice D Tune</li>
    <li><strong>CC 5</strong>: Mix</li>
    <li><strong>CC 6</strong>: Shape</li>
  </ul>

  <h2>Clock & Transport</h2>
  <ul>
    <li><strong>MIDI Clock</strong>: Synchronizes Looper recording and playback speed.</li>
    <li><strong>MIDI Start</strong>: Resets loop playback to the beginning.</li>
  </ul>
  
  <div class="page-footer">
    <span>Drone User Manual</span>
    <span>Page 9</span>
  </div>
</div>

<div class="page-body">
  <h1>7. Firmware Update</h1>

  <p>The firmware update process is as follows:</p>

  <ol>
    <li><strong>Power Off the Module</strong>: Disconnect the module from power.</li>
    <li><strong>Press and hold BOOTSEL</strong>: Locate the physical <strong>BOOTSEL</strong> button on the microcontroller (Pico 2) on the back of the module, and hold it down.</li>
    <li><strong>Connect to Computer</strong>: While holding the <strong>BOOTSEL</strong> button, connect the Pico 2 to your computer via a USB cable.</li>
    <li><strong>Release BOOTSEL</strong>: Once the USB cable is plugged in, release the button. The Pico 2 will mount on your computer as a USB mass storage drive.</li>
    <li><strong>Transfer the Firmware</strong>: Locate the compiled firmware file <code>firmware/drone.ino.uf2</code>. Drag and drop (or copy) the <code>.uf2</code> file onto the drive.</li>
    <li><strong>Automatic Reboot</strong>: The Pico 2 will write the new firmware to its flash memory, automatically eject/unmount, and reboot with the updated firmware running.</li>
  </ol>

  <div class="page-footer">
    <span>Drone User Manual</span>
    <span>Page 10</span>
  </div>
</div>

<div class="page-body">
  <h1>8. Troubleshooting</h1>

  <h2>No Sound Output</h2>
  <ul>
    <li>Verify that the eurorack power ribbon cable is connected correctly.</li>
    <li>Sweep the <strong>Mix</strong> and <strong>Shape</strong> knobs to see if that makes any difference.</li>
    <li>Make sure your audio cable is patched securely.</li>
    <li>Switch off and switch on the module to restart the microcontroller.</li>
  </ul>
  
  <div class="page-footer">
    <span>Drone User Manual</span>
    <span>Page 11</span>
  </div>
</div>