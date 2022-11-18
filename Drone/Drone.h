#ifndef Drone_h
#define Drone_h

#include <Audio.h>

#include "Etc/Etc.h"
#include "Voice.h"
#include "LinearToSpiral.h"
#include "DC.h"

/*
   Drone
*/
class Drone
{

private:
  // Singleton
  static Drone *instance;
  Drone();

  // Inputs
  Input *tune1;
  Input *tune2;
  Input *tune3;
  Input *tune4;
  Input *mix;
  Input *fm;

  // Leds
  Led *led1;
  Led *led2;
  Led *led3;
  Led *led4;

  // Linear to Spiral converter
  LinearToSpiral *linearToSpiral;

  static const byte voiceCount = 4;
  Voice *voices[voiceCount];

  byte updateMillis = 10;
  elapsedMillis clockUpdate;

  AudioMixer4 *outputLeft;
  AudioMixer4 *outputRight;

public:
  static Drone *getInstance();
  void init();
  void update();
  void setPan(byte voiceIndex, byte pan);

  // Inputs callbacks
  //     static void onTuneChange(Input* input);
  //    static void onMixChange(Input* input); // TODO: CREATE A SPIRAL CONVERTER TO ELIMINATE THIS CALLBACK
  static void onFmChange(Input *input);

  // Audio output
  AudioMixer4 *getOutputLeft();
  AudioMixer4 *getOutputRight();
};

// Singleton pre init
Drone *Drone::instance = nullptr;

/**
   Constructor
*/
inline Drone::Drone()
{
}

/**
   Singleton instance
*/
inline Drone *Drone::getInstance()
{
  if (!instance)
    instance = new Drone;
  return instance;
}

/**
   Init
*/
inline void Drone::init()
{
  this->tune1 = new Input(0);
  this->tune2 = new Input(1);
  this->tune3 = new Input(2);
  this->tune4 = new Input(3);
  this->mix = new Input(4);
  this->fm = new Input(5);

  this->led1 = new Led(0);
  this->led2 = new Led(1);
  this->led3 = new Led(2);
  this->led4 = new Led(3);

  this->linearToSpiral = new LinearToSpiral();

  this->outputLeft = new AudioMixer4();
  this->outputLeft->gain(0, 1);
  this->outputLeft->gain(1, 1);
  this->outputLeft->gain(2, 1);
  this->outputLeft->gain(3, 1);

  this->outputRight = new AudioMixer4();
  this->outputRight->gain(0, 1);
  this->outputRight->gain(1, 1);
  this->outputRight->gain(2, 1);
  this->outputRight->gain(3, 1);

  //  AudioSynthWaveformDc *dc = new AudioSynthWaveformDc();
  //  AudioEffectMultiply *sineModulatorMultiplier = new AudioEffectMultiply();
  //  dc->amplitude(1);
  //  new AudioConnection(*dc, 0, *sineModulatorMultiplier, 0);
  //  new AudioConnection(*this->tune1, 0, *sineModulatorMultiplier, 1);
  //  new AudioConnection(*sineModulatorMultiplier, 0, *this->outputLeft, 0);

  // DC *dc = new DC();
  // new AudioConnection(*dc, 0, *this->outputLeft, 0);

  // new AudioConnection(*this->tune1, 0, *this->outputLeft, 1);;
  // new AudioConnection(*this->tune1, 0, *this->led1, 0);

  PrintSerial *printSerial = new PrintSerial();
  new AudioConnection(*this->tune1, 0, *printSerial, 0);
  // new AudioConnection(*this->tune2, 0, *printSerial, 1);
  // new AudioConnection(*this->tune3, 0, *printSerial, 2);
  // new AudioConnection(*this->tune4, 0, *printSerial, 3);
  // new AudioConnection(*this->mix, 0, *printSerial, 4);
  // new AudioConnection(*this->fm, 0, *printSerial, 5);

  for (int i = 0; i < this->voiceCount; i++)
  {
    this->voices[i] = new Voice();
    new AudioConnection(*this->voices[i], 0, *this->outputLeft, i);
    new AudioConnection(*this->voices[i], 0, *this->outputRight, i);
  }

  new AudioConnection(*this->tune1, 0, *this->voices[0], 0);
  new AudioConnection(*this->tune2, 0, *this->voices[1], 0);
  new AudioConnection(*this->tune3, 0, *this->voices[2], 0);
  new AudioConnection(*this->tune4, 0, *this->voices[3], 0);
  new AudioConnection(*this->mix, 0, *this->linearToSpiral, 0);
  new AudioConnection(*this->linearToSpiral, 0, *this->voices[0], 1);
  new AudioConnection(*this->linearToSpiral, 1, *this->voices[1], 1);
  new AudioConnection(*this->linearToSpiral, 2, *this->voices[3], 1);
  new AudioConnection(*this->linearToSpiral, 3, *this->voices[2], 1);
  new AudioConnection(*this->linearToSpiral, 0, *this->led1, 0);
  new AudioConnection(*this->linearToSpiral, 1, *this->led2, 0);
  new AudioConnection(*this->linearToSpiral, 2, *this->led4, 0);
  new AudioConnection(*this->linearToSpiral, 3, *this->led3, 0);
  new AudioConnection(*this->fm, 0, *this->voices[0], 2);
  new AudioConnection(*this->fm, 0, *this->voices[1], 2);
  new AudioConnection(*this->fm, 0, *this->voices[2], 2);
  new AudioConnection(*this->fm, 0, *this->voices[3], 2);

  // Set the pan programmatically
  //  this->setPan(0, 50);
  //  this->setPan(1, 100);
  //  this->setPan(2, 150);
  //  this->setPan(3, 200);
  ETC::init();
}

/**
   Return the audio left output
*/
inline AudioMixer4 *Drone::getOutputLeft()
{
  return this->outputLeft;
}

/**
   Return the audio right output
*/
inline AudioMixer4 *Drone::getOutputRight()
{
  return this->outputRight;
}

/**
   Update
*/
inline void Drone::update()
{
  ETC::update();
  //  Serial.println("update");
}

/**
   Set one voice's pan
*/
// inline void Drone::setPan(byte voiceIndex, byte pan){
//   voiceIndex = voiceIndex % 4;
//   this->voices[voiceIndex]->setPan(pan);
// }

#endif
