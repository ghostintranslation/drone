/*
GHOST IN TRANSLATION
DRONE
v2.0.2

https://www.ghostintranslation.com/

Support my work:
https://www.buymeacoffee.com/ghostintranslation
https://ko-fi.com/ghostintranslation
https://liberapay.com/ghostintranslation/
https://github.com/sponsors/ghostintranslation

Stay in touch
https://ghostintranslation.bandcamp.com/
https://www.instagram.com/ghostintranslation/
https://www.youtube.com/ghostintranslation
https://github.com/ghostintranslation
*/

#include <Audio.h>
#include "Drone.h"

AudioOutputI2S i2s;
AudioOutputUSB usb;

// Instanciation of Drone
Drone *drone = Drone::getInstance();

void setup()
{
  Serial.begin(115200);

  while (!Serial && millis() < 2500)
    ; // wait for serial monitor

  // Audio connections require memory to work.
  AudioMemory(500);

  drone->init();
  new AudioConnection(*drone->getOutput(), 0, i2s, 0);
  new AudioConnection(*drone->getOutput(), 0, i2s, 1);
  new AudioConnection(*drone->getOutput(), 0, usb, 0);
  new AudioConnection(*drone->getOutput(), 0, usb, 1);
}

void loop()
{
}
