#ifndef LedManager_h
#define LedManager_h

#include "Led.h"

class LedManager
{
  public:
    static LedManager *getInstance();
    void update();

  private:
    // Singleton
    static LedManager *instance;
    LedManager();

    // Update clock
    elapsedMicros clockUpdate;
    const unsigned int intervalClockUpdate = 100; // 8ms period = 120Hz

    // PWM clock
    elapsedMicros clockPWM;
    const unsigned int intervalPWM = 8000; // 16ms period = 60Hz

    uint8_t ledsData = 0;
    
    void writeLeds();

};

// Singleton pre init
LedManager * LedManager::instance = nullptr;

inline LedManager::LedManager(){
}

/**
 * Singleton instance
 */
inline LedManager *LedManager::getInstance() {
  if (!instance)
     instance = new LedManager;
  return instance;
}

inline void LedManager::update(){
    if(this->clockUpdate > this->intervalClockUpdate){
        this->writeLeds();
        this->clockUpdate = 0;
    }

    if (this->clockPWM > this->intervalPWM)
    {
        this->clockPWM = 0;
    }
}

//elapsedMicros clockTest;

inline void LedManager::writeLeds()
{
    // Preparing the shift register data
    uint8_t ledsData = 0;

    // If no LED we don't need to send anything
    if(Led::getCount() == 0){
        return;
    }
    // Preparing the LEDs data
    for(uint8_t i = 0; i < Led::getCount(); i++){
//      Serial.println((float)this->clockPWM / this->intervalPWM);
        if (Led::get(i)->getValue() == ABSOLUTE_ANALOG_MIN)
        {
            continue;
        }
        else if (Led::get(i)->getValue() == ABSOLUTE_ANALOG_MAX)
        {
            bitSet(ledsData, Led::get(i)->getIndex());
        }
        // The value is a int16_t, offseting it to uint16_t in order to run this comparison
        else if ((float)this->clockPWM / this->intervalPWM < pow(((float)Led::get(i)->getValue() + (float)ABSOLUTE_ANALOG_MAX) / (ABSOLUTE_ANALOG_MAX*2),4))
        {
            bitSet(ledsData, Led::get(i)->getIndex());
        }
    }

//if(this->ledsData == 1 && ledsData == 0){
//  Serial.println(clockTest);
//}
//if(this->ledsData == 0 && ledsData == 1){
//  clockTest = 0;
//}

//    Serial.println(pow(((float)Led::get(0)->getBrightness() + (float)ABSOLUTE_ANALOG_MAX) / (ABSOLUTE_ANALOG_MAX*2),4));

    // Reduce SPI use by not pushing data if not necessary
    if(this->ledsData == ledsData){
        return;
    }
  
//Serial.println(bitRead(ledsData, 0)*5);

    // Set the latch to low (activate the shift registers)
    digitalWrite(REGISTERS_LATCH_PIN, LOW);
    
    // Send the leds byte
    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(ledsData);
    SPI.endTransaction();
    
    // Set the latch to high (shift registers actually set their pins and stop listening)
    digitalWrite(REGISTERS_LATCH_PIN, HIGH);

    this->ledsData = ledsData;
}

#endif
