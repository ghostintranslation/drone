#ifndef Drone_h
#define Drone_h

#include <Audio.h>
#include "Motherboard/Motherboard.h"
#include "Motherboard/Input.h"
#include "Motherboard/OutputLed.h"
#include "Motherboard/Setting.h"
#include "Motherboard/MidiInput.h"
#include "Voice.h"
#include "LinearToSpiral.h"
#include "Combine.h"

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
  Input *shape;
  Input *voct1;
  Input *voct2;
  Input *voct3;
  Input *voct4;
  Input *mixMod;
  Input *shapeMod;
  Input *fmRange;

  // Midi inputs
  MidiInput *midiCCTune1;
  MidiInput *midiCCTune2;
  MidiInput *midiCCTune3;
  MidiInput *midiCCTune4;
  MidiInput *midiCCMix;
  MidiInput *midiCCShape;

  // Leds
  OutputLed *led1;
  OutputLed *led2;
  OutputLed *led3;
  OutputLed *led4;
  OutputLed *led5;
  OutputLed *led6;
  OutputLed *led7;
  OutputLed *led8;
  OutputLed *led9;
  OutputLed *led10;
  OutputLed *led11;

  // Linear to Spiral converter
  LinearToSpiral *linearToSpiral;

  static const byte voiceCount = 4;
  Voice *voices[voiceCount];

  Combine *tune1Combine;
  Combine *tune2Combine;
  Combine *tune3Combine;
  Combine *tune4Combine;
  Combine *mixCombine;
  Combine *shapeCombine;

  AudioMixer4 *output;

  // Settings
  // Setting *midiChannelSetting;
  Setting *midiCCTune1Setting;
  Setting *midiCCTune2Setting;
  Setting *midiCCTune3Setting;
  Setting *midiCCTune4Setting;
  Setting *midiCCMixSetting;
  Setting *midiCCShapeSetting;

public:
  static Drone *getInstance();
  void init();

  // Audio output
  AudioMixer4 *getOutput();
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

  // Settings
  // this->midiChannelSetting = new Setting("midi-channel", "Midi Channel", 0, 0, 8, 1);
  this->midiCCTune1Setting = new Setting("midi-cc-tune-1", "Midi CC Voice 1 tune", 1, 1, 127, 1);
  this->midiCCTune2Setting = new Setting("midi-cc-tune-2", "Midi CC Voice 2 tune", 2, 1, 127, 1);
  this->midiCCTune3Setting = new Setting("midi-cc-tune-3", "Midi CC Voice 3 tune", 3, 1, 127, 1);
  this->midiCCTune4Setting = new Setting("midi-cc-tune-4", "Midi CC Voice 4 tune", 4, 1, 127, 1);
  this->midiCCMixSetting = new Setting("midi-cc-mix", "Midi CC Mix", 5, 1, 127, 1);
  this->midiCCShapeSetting = new Setting("midi-cc-shape", "Midi CC Shape", 6, 1, 127, 1);

  Motherboard::init();

  this->midiCCTune1 = new MidiInput(this->midiCCTune1Setting);
  this->midiCCTune2 = new MidiInput(this->midiCCTune2Setting);
  this->midiCCTune3 = new MidiInput(this->midiCCTune3Setting);
  this->midiCCTune4 = new MidiInput(this->midiCCTune4Setting);
  this->midiCCMix = new MidiInput(this->midiCCMixSetting);
  this->midiCCShape = new MidiInput(this->midiCCShapeSetting);

  this->tune1 = new Input(0);
  this->tune2 = new Input(1);
  this->tune3 = new Input(2);
  this->tune4 = new Input(3);
  this->mix = new Input(4);
  // this->mix->setLowPassCoeff(0.0001);
  this->shape = new Input(5);
  this->voct1 = new Input(6);
  this->voct2 = new Input(7);
  this->voct3 = new Input(8);
  this->voct4 = new Input(9);
  this->mixMod = new Input(10);
  this->shapeMod = new Input(11);
  this->fmRange = new Input(12);

  this->led1 = new OutputLed(0);
  this->led2 = new OutputLed(1);
  this->led3 = new OutputLed(2);
  this->led4 = new OutputLed(3);
  this->led5 = new OutputLed(4);
  this->led6 = new OutputLed(5);
  this->led7 = new OutputLed(6);
  this->led8 = new OutputLed(7);
  this->led9 = new OutputLed(8);
  this->led10 = new OutputLed(9);
  this->led11 = new OutputLed(10);

  this->linearToSpiral = new LinearToSpiral();

  this->tune1Combine = new Combine();
  this->tune2Combine = new Combine();
  this->tune3Combine = new Combine();
  this->tune4Combine = new Combine();
  this->mixCombine = new Combine();
  this->shapeCombine = new Combine();

  this->output = new AudioMixer4();
  this->output->gain(0, 0.1); // 0.012, 0.25
  this->output->gain(1, 0.1);
  this->output->gain(2, 0.1);
  this->output->gain(3, 0.1);

  for (int i = 0; i < this->voiceCount; i++)
  {
    this->voices[i] = new Voice();
    new AudioConnection(*this->voices[i], 0, *this->output, i);
  }

  // Tune 1
  new AudioConnection(*this->tune1, 0, *this->tune1Combine, 0);
  new AudioConnection(*this->midiCCTune1, 0, *this->tune1Combine, 1);
  new AudioConnection(*this->tune1Combine, 0, *this->voices[0], 0);
  // Tune 2
  new AudioConnection(*this->tune2, 0, *this->tune2Combine, 0);
  new AudioConnection(*this->midiCCTune2, 0, *this->tune2Combine, 1);
  new AudioConnection(*this->tune2Combine, 0, *this->voices[1], 0);
  // Tune 3
  new AudioConnection(*this->tune3, 0, *this->tune3Combine, 0);
  new AudioConnection(*this->midiCCTune3, 0, *this->tune3Combine, 1);
  new AudioConnection(*this->tune3Combine, 0, *this->voices[2], 0);
  // Tune 4
  new AudioConnection(*this->tune4, 0, *this->tune4Combine, 0);
  new AudioConnection(*this->midiCCTune4, 0, *this->tune4Combine, 1);
  new AudioConnection(*this->tune4Combine, 0, *this->voices[3], 0);

  new AudioConnection(*this->voct1, 0, *this->voices[0], 1);
  new AudioConnection(*this->voct2, 0, *this->voices[1], 1);
  new AudioConnection(*this->voct3, 0, *this->voices[2], 1);
  new AudioConnection(*this->voct4, 0, *this->voices[3], 1);
  new AudioConnection(*this->mix, 0, *this->mixCombine, 0);
  new AudioConnection(*this->mixMod, 0, *this->mixCombine, 1);
  new AudioConnection(*this->midiCCMix, 0, *this->mixCombine, 2);
  new AudioConnection(*this->mixCombine, 0, *this->linearToSpiral, 0);
  new AudioConnection(*this->linearToSpiral, 0, *this->voices[0], 2);
  new AudioConnection(*this->linearToSpiral, 1, *this->voices[1], 2);
  new AudioConnection(*this->linearToSpiral, 2, *this->voices[3], 2);
  new AudioConnection(*this->linearToSpiral, 3, *this->voices[2], 2);
  new AudioConnection(*this->linearToSpiral, 0, *this->led1, 0);
  new AudioConnection(*this->linearToSpiral, 1, *this->led2, 0);
  new AudioConnection(*this->linearToSpiral, 2, *this->led4, 0);
  new AudioConnection(*this->linearToSpiral, 3, *this->led3, 0);
  new AudioConnection(*this->shape, 0, *this->shapeCombine, 0);
  new AudioConnection(*this->shapeMod, 0, *this->shapeCombine, 1);
  new AudioConnection(*this->midiCCShape, 0, *this->shapeCombine, 2);
  new AudioConnection(*this->shapeCombine, 0, *this->voices[0], 3);
  new AudioConnection(*this->shapeCombine, 0, *this->voices[1], 3);
  new AudioConnection(*this->shapeCombine, 0, *this->voices[2], 3);
  new AudioConnection(*this->shapeCombine, 0, *this->voices[3], 3);
  new AudioConnection(*this->voct1, 0, *this->led5, 0);
  new AudioConnection(*this->voct2, 0, *this->led6, 0);
  new AudioConnection(*this->voct3, 0, *this->led7, 0);
  new AudioConnection(*this->voct4, 0, *this->led8, 0);
  new AudioConnection(*this->mixMod, 0, *this->led9, 0);
  new AudioConnection(*this->shapeMod, 0, *this->led10, 0);
  new AudioConnection(*this->fmRange, 0, *this->led11, 0);
  new AudioConnection(*this->fmRange, 0, *this->voices[0], 4);
  new AudioConnection(*this->fmRange, 0, *this->voices[1], 4);
  new AudioConnection(*this->fmRange, 0, *this->voices[3], 4);
  new AudioConnection(*this->fmRange, 0, *this->voices[2], 4);
}

/**
   Return the audio left output
*/
inline AudioMixer4 *Drone::getOutput()
{
  return this->output;
}
#endif
