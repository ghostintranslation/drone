#ifndef Drone_h
#define Drone_h

#include <Audio.h>

#include "Etc/Etc.h"
#include "Voice.h"
#include "LinearToSpiral.h"

/*
 * Drone
 */
class Drone{
  
  private:
    // Singleton
    static Drone *instance;
    Drone();

    // Inputs
    Input* tune1;
    Input* tune2;
    Input* tune3;
    Input* tune4;
    Input* mix;
    Input* fm;

    // Leds
    Led* led1;
    Led* led2;
    Led* led3;
    Led* led4;

    // Linear to Spiral converter
    LinearToSpiral* linearToSpiral;

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
    static void onFmChange(Input* input);

    // Audio output
    AudioMixer4 * getOutputLeft();
    AudioMixer4 * getOutputRight();
};

// Singleton pre init
Drone * Drone::instance = nullptr;

/**
 * Constructor
 */
inline Drone::Drone(){  

}

/**
 * Singleton instance
 */
inline Drone *Drone::getInstance()    {
  if (!instance)
     instance = new Drone;
  return instance;
}

/**
 * Init
 */
inline void Drone::init(){
//Serial.println("init");

  this->tune1 = new Input(0);
//  this->tune1->setOnChange(onTuneChange);

  this->tune2 = new Input(1);
//  this->tune2->setOnChange(onTuneChange);

  this->tune3 = new Input(2);
//  this->tune3->setOnChange(onTuneChange);

  this->tune4 = new Input(3);
//  this->tune4->setOnChange(onTuneChange);

  this->mix = new Input(4);
//  this->mix->setOnChange(onMixChange);
  this->linearToSpiral = new LinearToSpiral();
  
  this->fm = new Input(5);
  this->fm->setOnChange(onFmChange);

  this->led1 = new Led(0);
  this->led2 = new Led(1);
  this->led3 = new Led(2);
  this->led4 = new Led(3);

  this->outputLeft = new AudioMixer4();
  this->outputLeft->gain(0, 0.25);
  this->outputLeft->gain(1, 0.25);
  this->outputLeft->gain(2, 0.25);
  this->outputLeft->gain(3, 0.25);
  
  this->outputRight = new AudioMixer4();
  this->outputRight->gain(0, 0.25);
  this->outputRight->gain(1, 0.25);
  this->outputRight->gain(2, 0.25);
  this->outputRight->gain(3, 0.25);
  
  // TODO: MAKE VOICE DIRECTLY OUTPUT AUDIO
  for (int i = 0; i < this->voiceCount; i++) {
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
  new AudioConnection(*this->linearToSpiral, 2, *this->voices[2], 1);
  new AudioConnection(*this->linearToSpiral, 3, *this->voices[3], 1);
  new AudioConnection(*this->linearToSpiral, 0, *this->led1, 0);
  new AudioConnection(*this->linearToSpiral, 1, *this->led2, 0);
  new AudioConnection(*this->linearToSpiral, 2, *this->led3, 0);
  new AudioConnection(*this->linearToSpiral, 3, *this->led4, 0);
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
 * Return the audio left output
 */
inline AudioMixer4 * Drone::getOutputLeft(){
  return this->outputLeft;
}

/**
 * Return the audio right output
 */
inline AudioMixer4 * Drone::getOutputRight(){
  return this->outputRight;
}

/**
 * Update
 */
inline void Drone::update(){
  ETC::update();
//  Serial.println("update");
}

/**
 * Set one voice's pan
 */
//inline void Drone::setPan(byte voiceIndex, byte pan){
//  voiceIndex = voiceIndex % 4;
//  this->voices[voiceIndex]->setPan(pan);
//}


//inline void Drone::onMixChange(Input* input){
//  Serial.print("onMixChange ");
//  Serial.println(getInstance()->led1->getValue());
//}

//inline void Drone::onTuneChange(Input* input){
//  Serial.print("onTuneChange ");
//  Serial.print(input->getIndex());
//  Serial.print(" ");
//  Serial.println(input->getValue());
//}

/**
 * On Fm Change
 */
inline void Drone::onFmChange(Input* input){
//  Serial.print("onFmChange ");
//  Serial.print(input->getIndex());
//  Serial.print(" ");
//  Serial.println(input->getValue());

//  for (int i = 0; i < getInstance()->voiceCount ; i++) {
//    getInstance()->voices[i]->setModulatorFrequency(input->getValue());
////    getInstance()->voices[i]->setModulatorFrequency(0);
//  }
//  
//  for (int i = 0; i < getInstance()->voiceCount ; i++) {
//    getInstance()->voices[i]->setModulatorAmplitude(input->getValue());
////    getInstance()->voices[i]->setModulatorAmplitude(0);
//  }
}
#endif
