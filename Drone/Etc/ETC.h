#ifndef ETC_h
#define ETC_h

#define REGISTERS_LATCH_PIN 9
#define SPI_CLOCK_PIN 13
#define SPI_MOSI_PIN 11
#define ABSOLUTE_ANALOG_MIN -32768
#define ABSOLUTE_ANALOG_MAX 32767
#define ANALOG_RANGE 65536

#include <SPI.h>
#include "Input.h"
#include "MidiManager.h"
#include "Led.h"
#include "LedManager.h"
#include "InputsManager.h"

class ETC
{
  public:
    static ETC *getInstance();
    static void init();
    static void update();

  private:
    // Singleton
    static ETC *instance;
    ETC();

    elapsedMicros updateClock;
    // IntervalTimer updateTimer;
};

// Singleton pre init
ETC * ETC::instance = nullptr;

inline ETC::ETC(){
  // updateTimer.begin(ETC::update, 4000);
  // updateTimer.priority(255);
}

/**
 * Singleton instance
 */
inline ETC *ETC::getInstance() {
  if (!instance)
     instance = new ETC;
  return instance;
}

inline void ETC::init(){
//  Serial.println("init");
  pinMode(REGISTERS_LATCH_PIN, OUTPUT);
  pinMode(SPI_CLOCK_PIN, OUTPUT);
  pinMode(SPI_MOSI_PIN, OUTPUT);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setSCK(SPI_CLOCK_PIN);
  SPI.setMOSI(SPI_MOSI_PIN);
  SPI.begin();

  InputsManager::getInstance()->init();
}

inline void ETC::update(){
//  Serial.println("update");

  if(getInstance()->updateClock >= 22){
    for(Input* input : Input::getAll()){
      input->realTimeUpdate();
    }
    getInstance()->updateClock = 0;
  }
  
  LedManager::getInstance()->update();
}


ETC &ETC = *ETC::getInstance();

#endif
