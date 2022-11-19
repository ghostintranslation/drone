/*
GHOST IN TRANSLATION
DRONE
v2.0.0

Support my work:
https://patreon.com/ghostintranslation
https://www.paypal.com/paypalme/ghostintranslation

Music and Merch:
https://ghostintranslation.bandcamp.com/

Social:
https://www.instagram.com/ghostintranslation/
https://www.youtube.com/c/ghostintranslation

Code:
https://github.com/ghostintranslation

My website:
https://www.ghostintranslation.com/
*/

#include <Audio.h>
#include "Drone.h"

// Instanciation of Drone
Drone * drone = Drone::getInstance();

AudioOutputI2S i2s;
AudioControlSGTL5000 audioBoard;

void setup() {
  Serial.begin(115200);
  
  while (!Serial && millis() < 2500); // wait for serial monitor

  // Audio connections require memory to work.
  AudioMemory(60);
  
  audioBoard.enable();
  audioBoard.volume(0.2);

  drone->init();
  new AudioConnection(*drone->getOutputLeft(), 0, i2s, 0);
  new AudioConnection (*drone->getOutputRight(), 0, i2s, 1);

  // Ready!
//  Serial.println("Ready!");
}

void loop() {
  
  // Drone update
  drone->update();
}
