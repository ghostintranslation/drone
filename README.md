# DRONE ![GitHub version](https://img.shields.io/github/v/release/ghostintranslation/drone.svg?include_prereleases)

**Drone** is a powerful, 4-voice synthesizer/complex oscillator module. It runs high-quality digital sound algorithms to deliver rich, thick, and evolving drones, soundscapes, and metallic textures.

This module is based on the [Motherboard](https://github.com/ghostintranslation/motherboard) platform which can be used to build any kind of digital modules. Current version uses a Pico 2 (RP2350).

<img src="./hardware/drone.WEBP" width="200px"/>

## Features

* 4 individual voices
* 16 Synthesis engines
* Stereo audio (48kHz, 16-bit)
* MIDI TRS Type A (planned)
* Auto calibration
* Either-way power

## Specifications

* Size: 3U, 8HP
* Depth: 50mm
* Power: +12V 75mA, -12V 3.5mA, +5V 0mA
* Inputs range: 0V to 5V

## How to use

The front panel features controls for adjusting the tuning, timbral shape, and volume mix of the 4 voices. It accepts standard Eurorack signals (CV - Control Voltage) to automate these parameters.

### Knobs
* **A B C D**: Sets the base pitch/tuning for each of the four individual voices. The knobs cover a wide range of 7 octaves.
* **MIX**: Turning this knob morphs the volumes of the four voices in a spiral motion.
* **SHAPE**: Adjusts the main character of the sound (such as morphing wavetables, filter sweeps, or FM intensity).

### CV Inputs
* **CV A, B, C, D**: Pitch CV inputs for each voice. Connect keyboard outputs, sequencers, or LFOs here to control the pitch of each voice (scales up to 5 octaves).
* **CV MIX**: Modulates the Mix parameter.
* **CV SHAPE**: Modulates the Shape parameter.
* **CV FM**: Linear frequency modulation for all four voices. 

### Navigation Button

The Mode button is used to change the active synthesis algorithm:

1. **Enter Menu**: Press and hold the **Mode BUTTON** for **1 second**. All 8 top LEDs will turn solid **Blue**.
2. **Read the Settings**: The active algorithm is represented by two blinking **Red** LEDs:
   * Bank Indicators (First 4 LEDs)
   * Algorithm Indicators (Last 4 LEDs)
3. **Change Algorithm**: Tap the **Mode BUTTON** to advance to the next algorithm.
4. **Save & Exit**: After **5 seconds** of inactivity, the module will save your choice and automatically return to normal play mode.

## Algorithms

The module contains 16 algorithms. Below is an overview of what each algorithm does and what the **SHAPE** knob controls.

### Bank 1
| # | Sound Description | What the **SHAPE** Control Does |
| :--- | :--- | :--- |
| 1 | **Sine-Resonant Noise**: A mix of warm, pure sine waves and soft resonant noise. | Blends from pure sine tones (0%) to wind-like resonant noise (100%). |
| 2 | **Resonant Noise**: Filters white noise into whistle-like pitched tones. | Sets Q-resonance. Turn up to morph from ocean-wave wind sounds to sharp, whistling pitches. |
| 3 | **Sine-Phaser-Ring**: Stereo phase-shifted sines combined with ring modulation. | Sets Ring Modulator speed (60Hz to 160Hz) to add metallic, sci-fi buzzes. |
| 4 | **Sine-FM**: Warm frequency modulation synthesis. | Sets modulator speed and modulation depth, introducing growling sidebands and metallic overtones. |

### Bank 2 
| # | Sound Description | What the **SHAPE** Control Does |
| :--- | :--- | :--- |
| 1 | **Saw-Comparator**: Gated, aggressive digital saw waves. | Sets the clipping threshold on the sum of the 4 saw waves. |
| 2 | **Slink Filter**: 8 parallel resonant bandpass filters swept by a complex LFO. | Adjusts sweep rate, ripple spacing, and filter density for vocal "formant" sweeps. |
| 3 | **FM & Wavefolder**: Aggressive, harmonically rich wavefolded FM voice. | Drives FM depth and folding intensity to add sharp digital bite and buzz. |
| 4 | **Karplus-Strong Plucks**: Simulated plucked and bowed string resonances. | Changes the brightness/filtering of the plucking excitation. Morphs from dark thuds to metallic plucks. |

### Bank 3
| # | Sound Description | What the **SHAPE** Control Does |
| :--- | :--- | :--- |
| 1 | **Gong & Bells**: Inharmonic 4-operator FM chain recreating metallic bells. | Acts as modulation index. Morphs from simple sine tones to complex clangs and gong washes. |
| 2 | **Dust-Triangle**: Warm triangle drone mixed with crackling rain impulses. | Blends between crackling, pitched rain clicks (0%) and a clean, warm triangle wave drone (100%). |
| 3 | **[Unused / Empty]**: `[TODO: USER_INFO]` Add description of custom algorithm. | `[TODO: USER_INFO]` Describe shape function. |
| 4 | **[Unused / Empty]**: `[TODO: USER_INFO]` Add description of custom algorithm. | `[TODO: USER_INFO]` Describe shape function. |

### Bank 4
| # | Sound Description | What the **SHAPE** Control Does |
| :--- | :--- | :--- |
| 1 | **Wavetable 1: "Octopus"**: Complex 3D wavetable morphs with cascading modulation. | Scans smoothly through 128 waves to change the fundamental timbre. |
| 2 | **Wavetable 2: "Bells"**: Metallic bell-like wavetable morphs with cascading modulation. | Scans smoothly through 128 waves to change the fundamental timbre. |
| 3 | **Wavetable 3: "Fly By One"**: Vocal and sci-fi wavetable morphs with cascading modulation. | Scans smoothly through 128 waves to change the fundamental timbre. |
| 4 | **Wavetable 4: "Yin Yang"**: Deep, harmonically complex wavetable morphs. | Scans smoothly through 128 waves to change the fundamental timbre. |


# About me

You can buy Drone on my website:

https://www.ghostintranslation.com/

You can also find me on Bandcamp, Instagram, YouTube:

https://ghostintranslation.bandcamp.com/

https://www.instagram.com/ghostintranslation/

https://www.youtube.com/ghostintranslation

To support my work:

https://ko-fi.com/ghostintranslation

https://www.buymeacoffee.com/ghostintranslation

https://liberapay.com/ghostintranslation

https://github.com/sponsors/ghostintranslation

# License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
