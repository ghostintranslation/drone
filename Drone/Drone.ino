/*
 __    _  _____   ___      ___ _  _     __    _ ______ _    
/__|_|/ \(_  |     | |\|    | |_)|_||\|(_ |  |_| |  | / \|\|
\_|| |\_/__) |    _|_| |    | | \| || |__)|__| | | _|_\_/| |

DRONE
v1.1.3

If you enjoy my work and music please consider donating.

https://www.ghostintranslation.com/
https://ghostintranslation.bandcamp.com/
https://www.instagram.com/ghostintranslation/
https://www.youtube.com/channel/UCcyUTGTM-hGLIz4194Inxyw
https://github.com/ghostintranslation
*/

#include <Audio.h>

#include "Drone.h"

// Instanciation of Drone
Drone * drone = Drone::getInstance();

AudioOutputI2S i2s2;
AudioOutputUSB usb;
AudioConnection patchCord1(*drone->getOutputLeft(), 0, i2s2, 0);
AudioConnection patchCord2(*drone->getOutputRight(), 0, i2s2, 1);
AudioConnection patchCord3(*drone->getOutputLeft(), 0, usb, 0);
AudioConnection patchCord4(*drone->getOutputRight(), 0, usb, 1);
AudioControlSGTL5000 audioBoard;

void setup() {
  Serial.begin(115200);
  
  while (!Serial && millis() < 2500); // wait for serial monitor
  
  drone->init();

  // Audio connections require memory to work.
  AudioMemory(40);
  
  audioBoard.enable();
  audioBoard.volume(0.4);
  

  // Starting sequence
  Serial.println("Ready!");
}

void loop() {
  // Drone update
  drone->update();
}
