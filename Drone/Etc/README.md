# M2
An affordable yet powerful Eurorack digital platform for Teensy 4.0 .

## Key Features
- 8 analog inputs
- 6 dimmable LEDS
- MIDI in/out
- Eurorack compatible
- Expandable with I2S and I2C
- Teensy's audio lib compatible
- Open Source
- Beginners friendly

## Premises
Is it possible to build many modules without reinventing the wheel every time, thus saving time and money?

Yes, by building a platform as the base to build modules on top of.

This is made possible by using a microcontroller as the core so that the module’s function is made in software and the module’s interface is fully decoupled from the base.

And that means every module could have the same hardware while showcasing different interfaces and fulfilling different functions. 

Such a platform would simplify maintenance, reduce time of building new modules, remove overhead of dealing with low level hardware, reduce overall cost and allow quick firmware updates to all modules at once. 

That’s what the Motherboard is all about.

While v1 was great and allowed me to create a few modules, it was mainly MIDI oriented,  came with some hardware limitations, and wasn’t easy to use in a Eurorack environment which was limiting creativity in the end.

V2 is fully compatible with Eurorack, and fixes previous limitations.



## Hardware

### Analog inputs
Up to 8 analog inputs are available. They are routed through a multiplexer to only 1 pin on Teensy. This is done so for future boards I can use the other pins with other multiplexers to go up to 32 inputs.

Analog inputs support a voltage range of 0 to 5v, and their sampling rate is about 12.5 kHz so frequencies up to about 6kHz can be read.

Sending voltages above 5v should not damage the Teensy thanks to an op-amp and diodes circuit, but over time I can’t guarantee it.

Sending negative voltages won't damage Teensy either thanks again to the op-amp circuit but it might damage the multiplexer. Additionally, a negative voltage on an input will have the effect to lower the next input’s reading, so readings will be wrong, this is due to the multiplexer being powered by 0-5v.

Note: I tried powering the 74HCT4051E with -5v/5v, because then it should support negative voltages but the readings where still wrong. A CD74HC4051BE might be the solution but this one needs 5v on the control pins, which requires transistors since Teensy’s pins output 3.3v. For now the user should just stick to positive voltages.

### LEDs
Up to 6 LEDs are available. They are connected to a shift register and their intensity is controlled with PWM.

### MIDI
MIDI in and out is available. The MIDI in goes through an optocoupler so Teensy won’t be damaged in case a device use high voltages on the line.
 
### Power
The platform is powered by the standard Eurorack 10 pin connector. The connector can be plugged either way thanks to a bridge rectifier, so no risk to damage anything by plugging the connector the other way.

### I2S and I2C
I2S an I2C pins are available in order to expand the platform capabilities. For example if a module needs audio in/out, an I2S chip like the CS4344 can be placed on the front PCB and connected to the appropriate pins. Or if a module needs analog output, an I2C DAC chip can be placed on the front PCB and connected to the appropriate pins.

### Expandable design
Additionally to connecting a front PCB to the platform’s PCB, a 10 pins header is available on the back in order to build an expander module. I2S, I2C and power is available on this header.

### Resilient design
In order to reduce the risk of not being able to source components (in case of a global crisis for example) most parts are through hole and “jelly beans”. Additionally some ICs have through hole and SMT traces allowing to do one or the other.

## Software

### Getting started
