#ifndef InputsManager_h
#define InputsManager_h

#include <DMAChannel.h>

//typedef struct audio_block{
//  int16_t data[AUDIO_BLOCK_SAMPLES*2];
//  uint16_t count = 0;
//  uint16_t readIndex = 0;
//  uint16_t period = 0;
//} audio_block;

/**
 * The manager for the inputs.
 * Handles the multiplexer and DMA in order to read one value and then switch the multiplexer
 * at 44100 * 8 Hz. Fills all 8 inputs buffer that are read by the Input objects in 2.9ms as
 * required by the audio library.
 */
class InputsManager
{
public:
  static InputsManager *getInstance();
  void init();
  void update();
  static void dmaInterrupt();
  //    volatile audio_block* getData(byte index);
  // audio_buffer *getBuffer(byte index);
  int16_t *readInput(byte index);

private:
  // Singleton
  static InputsManager *instance;
  InputsManager();

  void iterate();

  static DMAChannel dmachannel;
  static uint32_t dmaValue;
  static const byte inputsCount = 8;
  byte muxIndex = 0;
  static const uint32_t buffSize = AUDIO_BLOCK_SAMPLES;
  uint32_t data[inputsCount][buffSize * 2];
  uint32_t count[inputsCount];
  //    volatile audio_block* readyData[inputsCount];

  // typedef struct audio_buffer
  // {
  //   int16_t data[10][AUDIO_BLOCK_SAMPLES * 2];
  //   bool isFree[10] = {true, true, true, true, true, true, true, true, true, true};
  //   volatile uint8_t head, tail, headSamplesCount;

  //   void write(int16_t data)
  //   {
  //     if (this->isFree[this->head])
  //     {
  //       this->data[this->head][this->headSamplesCount++] = data;

  //       if (this->headSamplesCount >= AUDIO_BLOCK_SAMPLES)
  //       {
  //         // Block is complete with AUDIO_BLOCK_SAMPLES samples
  //         this->isFree[this->head] = false;
  //         this->head++;
  //         this->head = this->head % 10;
  //         this->headSamplesCount = 0;
  //       }
  //     }
  //   }
  //
  // int16_t * read()
  // {
  //   if (!this->isFree[this->tail])
  //   {
  //     int16_t *block = this->data[this->tail];
  //     this->isFree[this->tail] = true;
  //     this->tail++;
  //     this->tail = this->tail % 10;
  //     return block;
  //   }

  //   return NULL;
  // }

  // } audio_buffer;

  typedef struct audio_buffer
  {
    int16_t data[AUDIO_BLOCK_SAMPLES * 2];
    uint16_t head;

    audio_buffer()
    {
      for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
      {
        this->data[i] = 0;
      }
    }

    void write(int16_t data)
    {
      if (this->head < AUDIO_BLOCK_SAMPLES)
      {
        this->data[this->head] = data;
        this->head++;
      }
    }

    int16_t *read()
    {
      if (this->head >= AUDIO_BLOCK_SAMPLES - 1)
      {
        this->head = 0;
        return this->data;
      }
      else
      {
        for (int i = this->head; i < AUDIO_BLOCK_SAMPLES; i++)
        {
          this->data[i] = this->data[this->head];
        }
        // this->head = AUDIO_BLOCK_SAMPLES;
        this->head = 0;
        return this->data;
        // Serial.println(this->head);
      }
      // return NULL;
    }
  } audio_buffer;

  audio_buffer *buffers[inputsCount];
};

PROGMEM static const uint8_t adc2_pin_to_channel[] = {
    7,   // 0/A0  AD_B1_02
    8,   // 1/A1  AD_B1_03
    12,  // 2/A2  AD_B1_07
    11,  // 3/A3  AD_B1_06
    6,   // 4/A4  AD_B1_01
    5,   // 5/A5  AD_B1_00
    15,  // 6/A6  AD_B1_10
    0,   // 7/A7  AD_B1_11
    13,  // 8/A8  AD_B1_08
    14,  // 9/A9  AD_B1_09
    255, // 10/A10 AD_B0_12 - only on ADC1, 1 - can't use for audio
    255, // 11/A11 AD_B0_13 - only on ADC1, 2 - can't use for audio
    3,   // 12/A12 AD_B1_14
    4,   // 13/A13 AD_B1_15
    7,   // 14/A0  AD_B1_02
    8,   // 15/A1  AD_B1_03
    12,  // 16/A2  AD_B1_07
    11,  // 17/A3  AD_B1_06
    6,   // 18/A4  AD_B1_01
    5,   // 19/A5  AD_B1_00
    15,  // 20/A6  AD_B1_10
    0,   // 21/A7  AD_B1_11
    13,  // 22/A8  AD_B1_08
    14,  // 23/A9  AD_B1_09
    255, // 24/A10 AD_B0_12 - only on ADC1, 1 - can't use for audio
    255, // 25/A11 AD_B0_13 - only on ADC1, 2 - can't use for audio
    3,   // 26/A12 AD_B1_14 - only on ADC2, do not use analogRead()
    4,   // 27/A13 AD_B1_15 - only on ADC2, do not use analogRead()
#ifdef ARDUINO_TEENSY41
    255, // 28
    255, // 29
    255, // 30
    255, // 31
    255, // 32
    255, // 33
    255, // 34
    255, // 35
    255, // 368

    255, // 37
    1,   // 38/A14 AD_B1_12 - only on ADC2, do not use analogRead()
    2,   // 39/A15 AD_B1_13 - only on ADC2, do not use analogRead()
    9,   // 40/A16 AD_B1_04
    10,  // 41/A17 AD_B1_05
#endif
};

extern "C" void xbar_connect(unsigned int input, unsigned int output);

DMAChannel InputsManager::dmachannel(false);
uint32_t InputsManager::dmaValue = 0;

// Singleton pre init
InputsManager *InputsManager::instance = nullptr;

inline InputsManager::InputsManager()
{
}

/**
 * Singleton instance
 */
inline InputsManager *InputsManager::getInstance()
{
  if (!instance)
    instance = new InputsManager;
  return instance;
}

inline void InputsManager::init()
{
  pinMode(A0, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  for (int i = 0; i < inputsCount; i++)
  {
    buffers[i] = new audio_buffer();
  }

  // configure a timer to trigger ADC
  // sample rate should be very close to 4X AUDIO_SAMPLE_RATE_EXACT
  const int comp1 = ((float)F_BUS_ACTUAL) / (AUDIO_SAMPLE_RATE_EXACT * (float)(inputsCount)) / 2.0f + 0.5f;
  TMR4_ENBL &= ~(1 << 3);
  TMR4_SCTRL3 = TMR_SCTRL_OEN | TMR_SCTRL_FORCE;
  TMR4_CSCTRL3 = TMR_CSCTRL_CL1(1) | TMR_CSCTRL_TCF1EN;
  TMR4_CNTR3 = 0;
  TMR4_LOAD3 = 0;
  TMR4_COMP13 = comp1;
  TMR4_CMPLD13 = comp1;
  TMR4_CTRL3 = TMR_CTRL_CM(1) | TMR_CTRL_PCS(8) | TMR_CTRL_LENGTH | TMR_CTRL_OUTMODE(3);
  TMR4_DMA3 = TMR_DMA_CMPLD1DE;
  TMR4_CNTR3 = 0;
  TMR4_ENBL |= (1 << 3);

  // connect the timer output the ADC_ETC input
  const int trigger = 4; // 0-3 for ADC1, 4-7 for ADC2
  CCM_CCGR2 |= CCM_CCGR2_XBAR1(CCM_CCGR_ON);
  xbar_connect(XBARA1_IN_QTIMER4_TIMER3, XBARA1_OUT_ADC_ETC_TRIG00 + trigger);

  // turn on ADC_ETC and configure to receive trigger
  if (ADC_ETC_CTRL & (ADC_ETC_CTRL_SOFTRST | ADC_ETC_CTRL_TSC_BYPASS))
  {
    ADC_ETC_CTRL = 0; // clears SOFTRST only
    ADC_ETC_CTRL = 0; // clears TSC_BYPASS
  }
  ADC_ETC_CTRL |= ADC_ETC_CTRL_TRIG_ENABLE(1 << trigger) | ADC_ETC_CTRL_DMA_MODE_SEL;
  ADC_ETC_DMA_CTRL |= ADC_ETC_DMA_CTRL_TRIQ_ENABLE(trigger);

  // configure ADC_ETC trigger4 to make one ADC2 measurement on pin A2
  const int len = 1;
  IMXRT_ADC_ETC.TRIG[trigger].CTRL = ADC_ETC_TRIG_CTRL_TRIG_CHAIN(len - 1) |
                                     ADC_ETC_TRIG_CTRL_TRIG_PRIORITY(7);
  IMXRT_ADC_ETC.TRIG[trigger].CHAIN_1_0 = ADC_ETC_TRIG_CHAIN_HWTS0(1) |
                                          ADC_ETC_TRIG_CHAIN_CSEL0(adc2_pin_to_channel[A0]) | ADC_ETC_TRIG_CHAIN_B2B0;

  uint32_t cfg = ADC_CFG_ADTRG;
  cfg |= ADC_CFG_MODE(2); // 2 = 12 bits
  cfg |= ADC_CFG_AVGS(0); // number of samples to average
                          //  cfg |= ADC_CFG_ADSTS(3); // sampling time, 0-3
  cfg |= ADC_CFG_ADLSMP;  // long sample time
  cfg |= ADC_CFG_ADHSC;   // high speed conversion
  //cfg |= ADC_CFG_ADLPC;    // low power
  cfg |= ADC_CFG_ADICLK(0); // 0:ipg, 1=ipg/2, 3=adack (10 or 20 MHz)
  cfg |= ADC_CFG_ADIV(1);   // 0:div1, 1=div2, 2=div4, 3=div8
  ADC2_CFG = cfg;
  //  ADC2_GC &= ~ADC_GC_AVGE; // single sample, no averaging
  ADC2_GC |= ADC_GC_AVGE;     // use averaging
  ADC2_HC0 = ADC_HC_ADCH(16); // 16 = controlled by ADC_ETC

  // configure DMA channels
  //  dmachannel.begin();
  ////  dmachannel.source( GPIO1_DR );
  //
  //  dmachannel.TCD->SADDR = &(IMXRT_ADC_ETC.TRIG[4].RESULT_1_0);
  ////  dmachannel.destinationBuffer( dmaBuffer, DMABUFFER_SIZE * 4 );
  //  dmachannel.destination(dmaValue);
  //  dmachannel.interruptAtCompletion();
  //  dmachannel.attachInterrupt( dmaInterrupt );
  //  dmachannel.transferSize(4);
  //  dmachannel.transferCount(1);
  //
  //  dmachannel.TCD->SOFF = 0;
  //  dmachannel.TCD->ATTR = DMA_TCD_ATTR_SSIZE(1) | DMA_TCD_ATTR_DSIZE(1);
  //  dmachannel.TCD->NBYTES_MLNO = 2;
  //  dmachannel.TCD->SLAST = 0;
  ////  dmachannel.TCD->DOFF = 2;
  ////  dmachannel.TCD->CSR = 0;
  //
  //  dmachannel.enable();
  // use a DMA channel to capture ADC_ETC output
  dmachannel.begin();
  dmachannel.TCD->SADDR = &(IMXRT_ADC_ETC.TRIG[4].RESULT_1_0);
  dmachannel.TCD->SOFF = 0;
  dmachannel.TCD->ATTR = DMA_TCD_ATTR_SSIZE(1) | DMA_TCD_ATTR_DSIZE(1);
  dmachannel.TCD->NBYTES_MLNO = 2;
  dmachannel.TCD->SLAST = 0;
  //  dmachannel.TCD->DADDR = adc_buffer;
  dmachannel.TCD->DOFF = 2;
  //  dmachannel.TCD->CITER_ELINKNO = sizeof(adc_buffer) / 2;
  //  dmachannel.TCD->DLASTSGA = -sizeof(adc_buffer);
  //  dmachannel.TCD->BITER_ELINKNO = sizeof(adc_buffer) / 2;
  dmachannel.TCD->CSR = 0;
  dmachannel.triggerAtHardwareEvent(DMAMUX_SOURCE_ADC_ETC);
  dmachannel.destination(this->dmaValue);
  dmachannel.interruptAtCompletion();
  dmachannel.transferSize(2);
  dmachannel.transferCount(1);
  dmachannel.attachInterrupt(this->dmaInterrupt);
  dmachannel.enable();
}

inline void InputsManager::update()
{
}

//inline volatile audio_block* InputsManager::getData(byte index){
//  return this->readyData[index];
//}

inline void InputsManager::iterate()
{
  this->muxIndex++;
  this->muxIndex = this->muxIndex % this->inputsCount;
  bool bit0 = this->muxIndex & 0x01;

  digitalWriteFast(2, bit0);
  digitalWriteFast(3, bitRead(this->muxIndex, 1));
  digitalWriteFast(4, bitRead(this->muxIndex, 2));
}

inline void InputsManager::dmaInterrupt()
{
  dmachannel.clearInterrupt(); // tell system we processed it.
  asm("DSB");                  // this is a memory barrier

  // Storing the value in the right buffer
  // This value is converted from the ADC range of unsigned 12 bits to a signed 16 bits range to match
  // the audio library's blocks samples range. So a GND value is now -32768.
  getInstance()->buffers[getInstance()->muxIndex]->write(dmaValue * 16 - 32768);

  // Switching to the next input
  getInstance()->iterate();
}

// inline audio_buffer *InputsManager::getBuffer(byte index)
// {
//   index = index % inputsCount;
//   return this->buffers[index];
// }

inline int16_t *InputsManager::readInput(byte index)
{
  index = index % inputsCount;
  return this->buffers[index]->read();
}

#endif
