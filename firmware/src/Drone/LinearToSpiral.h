#ifndef LinearToSpiral_h
#define LinearToSpiral_h

#include <Audio.h>

/**
 * This object converts 1 input into 4 outputs following a spiral pattern
 */
class LinearToSpiral : public AudioStream{
  private:
    audio_block_t *inputQueueArray[1];

  public:
    LinearToSpiral();
    void update(void);
};


/**
 * Constructor
 */
inline LinearToSpiral::LinearToSpiral() : AudioStream(1, inputQueueArray){
}


inline void LinearToSpiral::update(void) {
    // Receive input data
    audio_block_t *block = NULL, *outBlock1 = NULL, *outBlock2 = NULL, *outBlock3 = NULL, *outBlock4 = NULL;
    block = receiveReadOnly(0);

    if(!block){
      return;
    }
    
    outBlock1 = allocate();
    outBlock2 = allocate();
    outBlock3 = allocate();
    outBlock4 = allocate();

    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
        float x = map((float)block->data[i], INT16_MIN, INT16_MAX, 0, 4.5*PI);

        if(outBlock1){
          float out1 = constrain(100 * cos(x) + pow(x,2), 0, 100) / 100;
          outBlock1->data[i] = 65535 * out1 - 32768;
        }

        if(outBlock2){
          float out2 = constrain(100 * cos(x + 1.25 * PI) + pow(x,2), 0, 100) / 100;
          outBlock2->data[i] = 65535 * out2 - 32768;
        }

        if(outBlock3){
          float out3 = constrain(100 * cos(x + 0.75 * PI) + pow(x,2), 0, 100) / 100;
          outBlock3->data[i] = 65535 * out3 - 32768;
        }
        
        if(outBlock4){
          float out4 = constrain(100 * cos(x + 0.5 * PI) + pow(x,2), 0, 100) / 100;
          outBlock4->data[i] = 65535 * out4 - 32768;
        }
    }

    if(outBlock1){
      transmit(outBlock1, 0);
      release(outBlock1);
    }
    if(outBlock2){
      transmit(outBlock2, 1);
      release(outBlock2);
    }
    if(outBlock3){
      transmit(outBlock3, 2);
      release(outBlock3);
    }
    if(outBlock4){
      transmit(outBlock4, 3);
      release(outBlock4);
    }

    release(block);
}


#endif
