#ifndef LinearToSpiral_h
#define LinearToSpiral_h

#include <Audio.h>

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
    audio_block_t *block, *outBlock1, *outBlock2, *outBlock3, *outBlock4;
    block = receiveReadOnly(0);
    
    outBlock1 = allocate();
    outBlock2 = allocate();
    outBlock3 = allocate();
    outBlock4 = allocate();

    if(!block || !outBlock1 || !outBlock2 || !outBlock3 || !outBlock4){
        return;
    }

    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
        int16_t x = freqBlock->data[i];
      
        // Cutting off negative values
        if(x < 0){
           x = 0;
        }

        float out1 = constrain(cos(input->getValue()) + pow(input->getValue(),2), 0, 1);
        outBlock1->data[i] = 32767 * out1;

        float out2 = constrain(cos(input->getValue() + 1.5 * PI) + pow(input->getValue(),2), 0, 1);
        outBlock2->data[i] = 32767 * out2;

        float out3 = constrain(cos(input->getValue() + 0.5 * PI) + pow(input->getValue(),2), 0, 1);
        outBlock3->data[i] = 32767 * out3;

        float out4 = constrain(cos(input->getValue() + PI) + pow(input->getValue(),2), 0, 1);
        outBlock4->data[i] = 32767 * out4;
    }

    transmit(outBlock1);
    transmit(outBlock2);
    transmit(outBlock3);
    transmit(outBlock4);

    release(block);
    release(outBlock1);
    release(outBlock2);
    release(outBlock3);
    release(outBlock4);
}


#endif