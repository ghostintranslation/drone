#ifndef Drone_h
#define Drone_h

#include <Audio.h>
#include "Motherboard/Motherboard.h"
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

  // MIDI
  MidiCCInput *tune1MidiCCInput;
  MidiCCInput *tune2MidiCCInput;
  MidiCCInput *tune3MidiCCInput;
  MidiCCInput *tune4MidiCCInput;
  MidiCCInput *mixMidiCCInput;
  MidiCCInput *shapeMidiCCInput;

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

  Combine<2> *tune1Combine;
  Combine<2> *tune2Combine;
  Combine<2> *tune3Combine;
  Combine<2> *tune4Combine;
  Combine<3> *mixCombine;
  Combine<3> *shapeCombine;


  // AudioEffectCompressor *compressor;
  AudioMixer4 *output;

public:
  static Drone *getInstance();
  void init();

  // Audio output
  AudioStream *getOutput();
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
  this->shape = new Input(5);
  this->voct1 = new Input(6);
  this->voct1->setLowPassCoeff(0.01);
  this->voct2 = new Input(7);
  this->voct2->setLowPassCoeff(0.01);
  this->voct3 = new Input(8);
  this->voct3->setLowPassCoeff(0.01);
  this->voct4 = new Input(9);
  this->voct4->setLowPassCoeff(0.01);
  this->mixMod = new Input(10);
  this->mixMod->setLowPassCoeff(0.01);
  this->shapeMod = new Input(11);
  this->shapeMod->setLowPassCoeff(0.01);
  this->fmRange = new Input(12);
  this->fmRange->setLowPassCoeff(0.2);


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

  
  this->tune1Combine = new Combine<2>();
  this->tune2Combine = new Combine<2>();
  this->tune3Combine = new Combine<2>();
  this->tune4Combine = new Combine<2>();
  this->mixCombine = new Combine<3>();
  this->shapeCombine = new Combine<3>();

  // MIDI
  this->tune1MidiCCInput = new MidiCCInput(1);
  this->tune2MidiCCInput = new MidiCCInput(2);
  this->tune3MidiCCInput = new MidiCCInput(3);
  this->tune4MidiCCInput = new MidiCCInput(4);
  this->mixMidiCCInput = new MidiCCInput(5);
  this->shapeMidiCCInput = new MidiCCInput(6);

  this->output = new AudioMixer4();
  this->output->gain(0, 0.02);
  this->output->gain(1, 0.02);
  this->output->gain(2, 0.02);
  this->output->gain(3, 0.02);

  for (int i = 0; i < this->voiceCount; i++)
  {
    this->voices[i] = new Voice();
    new AudioConnection(*this->voices[i], 0, *this->output, i);
  }

  // Tune 1
  new AudioConnection(*this->tune1, 0, *this->tune1Combine, 0);
  new AudioConnection(*this->tune1MidiCCInput, 0, *this->tune1Combine, 1);
  new AudioConnection(*this->tune1Combine, 0, *this->voices[0], 0);
  // Tune 2
  new AudioConnection(*this->tune2, 0, *this->tune2Combine, 0);
  new AudioConnection(*this->tune2MidiCCInput, 0, *this->tune2Combine, 1);
  new AudioConnection(*this->tune2Combine, 0, *this->voices[1], 0);
  // Tune 3
  new AudioConnection(*this->tune3, 0, *this->tune3Combine, 0);
  new AudioConnection(*this->tune3MidiCCInput, 0, *this->tune3Combine, 1);
  new AudioConnection(*this->tune3Combine, 0, *this->voices[2], 0);
  // Tune 4
  new AudioConnection(*this->tune4, 0, *this->tune4Combine, 0);
  new AudioConnection(*this->tune4MidiCCInput, 0, *this->tune4Combine, 1);
  new AudioConnection(*this->tune4Combine, 0, *this->voices[3], 0);
  // v/Oct inputs
  new AudioConnection(*this->voct1, 0, *this->voices[0], 1);
  new AudioConnection(*this->voct1, 0, *this->led5, 0);
  new AudioConnection(*this->voct2, 0, *this->voices[1], 1);
  new AudioConnection(*this->voct2, 0, *this->led6, 0);
  new AudioConnection(*this->voct3, 0, *this->voices[2], 1);
  new AudioConnection(*this->voct3, 0, *this->led7, 0);
  new AudioConnection(*this->voct4, 0, *this->voices[3], 1);
  new AudioConnection(*this->voct4, 0, *this->led8, 0);
  // Mix 
  new AudioConnection(*this->mix, 0, *this->mixCombine, 0);
  new AudioConnection(*this->mixMod, 0, *this->mixCombine, 1);
  new AudioConnection(*this->mixMod, 0, *this->led9, 0);
  new AudioConnection(*this->mixMidiCCInput, 0, *this->mixCombine, 2);
  new AudioConnection(*this->mixCombine, 0, *this->linearToSpiral, 0);
  // Spiral
  new AudioConnection(*this->linearToSpiral, 0, *this->voices[0], 2);
  new AudioConnection(*this->linearToSpiral, 1, *this->voices[1], 2);
  new AudioConnection(*this->linearToSpiral, 2, *this->voices[3], 2);
  new AudioConnection(*this->linearToSpiral, 3, *this->voices[2], 2);
  new AudioConnection(*this->linearToSpiral, 0, *this->led1, 0);
  new AudioConnection(*this->linearToSpiral, 1, *this->led2, 0);
  new AudioConnection(*this->linearToSpiral, 2, *this->led4, 0);
  new AudioConnection(*this->linearToSpiral, 3, *this->led3, 0);
  // Shape
  new AudioConnection(*this->shape, 0, *this->shapeCombine, 0);
  new AudioConnection(*this->shapeMod, 0, *this->shapeCombine, 1);
  new AudioConnection(*this->shapeMod, 0, *this->led10, 0);
  new AudioConnection(*this->shapeMidiCCInput, 0, *this->shapeCombine, 2);
  new AudioConnection(*this->shapeCombine, 0, *this->voices[0], 3);
  new AudioConnection(*this->shapeCombine, 0, *this->voices[1], 3);
  new AudioConnection(*this->shapeCombine, 0, *this->voices[2], 3);
  new AudioConnection(*this->shapeCombine, 0, *this->voices[3], 3);
  // FM // TODO: rename fmRange -> FM
  new AudioConnection(*this->fmRange, 0, *this->led11, 0);
  new AudioConnection(*this->fmRange, 0, *this->voices[0], 4);
  new AudioConnection(*this->fmRange, 0, *this->voices[1], 4);
  new AudioConnection(*this->fmRange, 0, *this->voices[3], 4);
  new AudioConnection(*this->fmRange, 0, *this->voices[2], 4);
}

/**
   The audio output
*/
inline AudioStream *Drone::getOutput()
{
  // return this->compressor; // Quite noisy...
  return this->output;
}
#endif
