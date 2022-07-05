#ifndef Test_h
#define Test_h

#include "ETC.h"

class Test
{
  public:
    static Test* getInstance();
    void init();
    void update();
    static void onInputChange(Input* input);

  private:
    static Test *instance;
    Test();
    
    Input* input;
    Led* led;
    AudioSynthWaveformSineModulated* waveform1;
};

// Singleton pre init
Test * Test::instance = nullptr;


/**
 * Singleton instance
 */
inline Test *Test::getInstance() {
  if (!instance)
     instance = new Test;
  return instance;
}

inline Test::Test() {
}

inline void Test::init() {
  this->input = new Input(0);
  this->input->setOnChange(onInputChange);
  this->led = new Led(0);
  this->waveform1 = new AudioSynthWaveformSineModulated();
  new AudioConnection(*this->waveform1, 0, *this->led, 0);
  
  ETC::init();
}

inline void Test::update() {
  ETC::update();
}

inline void Test::onInputChange(Input* input){
  Serial.println(input->getValue());
}
#endif
