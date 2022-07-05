#include <Audio.h>

#include "Test.h"

//Input* input1; 
//Led* led1;

//AudioSynthWaveformSineModulated       waveform1;
AudioOutputI2S           i2s1;
AudioControlSGTL5000     sgtl5000_1;     //xy=930,518

Test* test = Test::getInstance();

void setup() {
  AudioMemory(20);
    
  Serial.begin(9600);
  while (!Serial && millis() < 5000) ;

//  input1 = new Input(1);
//  ETC::init();

//  input1 = new Input(1);
//  input1->setSmoothing(0);
//  led1 = new Led(0);
//  led1->setSmoothing(0);

  test->init();
  
  AudioNoInterrupts();

//  input1->setOnChange(onInputChange);
//  input1.setOnGateOpen(onInputGateOpen);
//  input1.setOnGateClose(onInputGateClose);

//  led1->setStatus(Led::Status::On);
//  led1->setSmoothing(100);
  
//  waveform1.frequency(1);
//  waveform1.amplitude(1);
//  waveform1.begin(WAVEFORM_TRIANGLE);


  
//  new AudioConnection(input1, 0, i2s1, 0);
//  new AudioConnection(input2, 0, waveform1, 0);
//  new AudioConnection(waveform1, 0, i2s1, 0);
//  new AudioConnection(waveform1, 0, i2s1, 1);

//  new AudioConnection(*input1, 0, *led1, 0);
//  new AudioConnection(waveform1, 0, *led1, 0);

  
  sgtl5000_1.enable();
  sgtl5000_1.volume(1);
//
//  pinMode(REGISTERS_LATCH_PIN, OUTPUT);
//  pinMode(SPI_CLOCK_PIN, OUTPUT);
//  pinMode(SPI_MOSI_PIN, OUTPUT);
//  SPI.setBitOrder(MSBFIRST);
//  SPI.setDataMode(SPI_MODE0);
//  SPI.setClockDivider(SPI_CLOCK_DIV2);
//  SPI.setSCK(SPI_CLOCK_PIN);
//  SPI.setMOSI(SPI_MOSI_PIN);
//  SPI.begin();
  
  AudioInterrupts();
}

elapsedMillis consoleClock;
byte data595 = 0;
void loop() {
  test->update();

//  if(consoleClock >= 20){
//    Serial.println(led1->getValue());
//    consoleClock = 0;
//  }
  
  
//  if(consoleClock >= 2){
//    Serial.printf("%d,%d,%d,%d,%d,%d,%d,%d", input1.getValue(), input2.getValue(), input3.getValue(), input4.getValue(), input5.getValue(), input6.getValue(), input7.getValue(), input8.getValue()); 
//    Serial.println("");
//    consoleClock = 0;
//  }
  
//  if(input2.getValue() >= 600){
//    // Set the latch to low (activate the shift registers)
//    digitalWrite(REGISTERS_LATCH_PIN, LOW);
//      
//    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
//    // Send the byte to set the MUX and select the DAC
//    SPI.transfer(1);
//    SPI.endTransaction();
//    
//    // Set the latch to high (shift registers actually set their pins and stop listening)
//    digitalWrite(REGISTERS_LATCH_PIN, HIGH);
//
////    if(data595 == 0){
////      data595 = 1;
////    }else{
////      data595 = 0;
////    }
////    clock595 = 0;
//  }else{
//       // Set the latch to low (activate the shift registers)
//    digitalWrite(REGISTERS_LATCH_PIN, LOW);
//      
//    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
//    // Send the byte to set the MUX and select the DAC
//    SPI.transfer(0);
//    SPI.endTransaction();
//    
//    // Set the latch to high (shift registers actually set their pins and stop listening)
//    digitalWrite(REGISTERS_LATCH_PIN, HIGH); 
//  }
}

void onInputChange(Input* input){
//  Serial.println(input->getValue());
}

void onInputGateOpen(Input* input){
  Serial.println("Gate open");
}

void onInputGateClose(Input* input){
  Serial.println("Gate close");
}
