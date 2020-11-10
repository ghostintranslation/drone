# DRONE

![GitHub version](https://img.shields.io/github/v/release/ghostintranslation/drone.svg?include_prereleases)

DRONE is a modular 4 voices drone synthesizer, based on eurorack physical format it has however no patch cables in the front but has rather midi and audio jacks in the back.

It is based on [Motherboard6](https://github.com/ghostintranslation/motherboard6).

## Features

* 4 voices
* FM synthesis
* Mix in spiral
* MIDI over USB
* MIDI over mini jack
* Audio jack output

## Dimensions

Height: 3U / 128.5mm

Width: 8HP / 40mm

## Getting Started

### Prerequisites

What things you need to make it work:

```
1 Teensy 4.0
1 Teensy audio board
1 MOTHERBOARD6A pcb
1 MOTHERBOARD6B pcb
1 DRONE front panel pcb
6 vertical linear 10k potentiometers
3 14 pins male header
5 14 pins female header
1 5 pins female header
4 CD4051 multiplexers
4 DIP16 IC sockets (optional)
1 resistor ~ 22ohm *
4 LED
```
* Regarding the resistor, this resistor is for the leds, you might notice that a 1ohm resistor will do the trick too, this is because the multiplexers have acutally impedance depending on the part number (CD4051 but you could try others with same pins) and the frequency of switching them. 

I personally get these parts from multiple stores: digikey.com, thonk.co.uk, modularaddict.com, amazon, aliexpress...

In order to run any sketch on the Teensy you have to install the Teensyduino add-on to Arduino.
Follow the instructions from the official page:
https://www.pjrc.com/teensy/teensyduino.html

Then open the sketch located in the Drone folder of this repo.

In the Tools -> USB Type menu, choose "Serial + midi".

Then you are ready to compile and upload the sketch.

## How to use

Here is a description of the 6 inputs and what they do:

```
Tune
    - Individual tune potentiometers

Mix
    - Mix the 4 voices in a spiral motion
    - LEDs indicate the presence of each voice

Fm
    - Modulator frequency and amplitude
    - Common to all voices
    - 3 divisions:
        1. 0 to 10Hz, low amplitude
        2. 0 to 40Hz, low amplitude
        3. 0 to 1000Hz, higher amplitude
```

## MIDI

The MIDI input and output jacks are directly connected to the Teensy serial input and output. That means there is not protection against voltage or current. It is primarily ment to connect 2 of these modules, or 2 Teensy together. If you want to connect something else to it make sure to provide a maximum of 3.3v and 250 mA.

Note that this module doesn't do anything MIDI but you could hack it.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

# About me
You can find me on Bandcamp and Instagram:

https://ghostintranslation.bandcamp.com/

https://www.instagram.com/ghostintranslation/

https://www.youtube.com/channel/UCcyUTGTM-hGLIz4194Inxyw

https://www.ghostintranslation.com/