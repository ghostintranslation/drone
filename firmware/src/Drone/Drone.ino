/*
GHOST IN TRANSLATION
DRONE
v2.0.0

Support my work:
https://www.buymeacoffee.com/ghostintranslation
https://ko-fi.com/ghostintranslation


Music and Merch:
https://ghostintranslation.bandcamp.com/

Social:
https://www.instagram.com/ghostintranslation/
https://www.youtube.com/ghostintranslation

Code:
https://github.com/ghostintranslation

My website:
https://www.ghostintranslation.com/
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
