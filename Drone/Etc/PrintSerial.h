#ifndef PrintSerial_h
#define PrintSerial_h

#include "AudioStream.h"

class PrintSerial : public AudioStream
{
public:
    PrintSerial();
    virtual void update(void);

private:
    audio_block_t *inputQueueArray[8];
    int16_t data[8][AUDIO_BLOCK_SAMPLES] = {{0}};
};

inline PrintSerial::PrintSerial() : AudioStream(8, inputQueueArray)
{
    this->active = true;
}

inline void PrintSerial::update(void)
{
    audio_block_t *block1, *block2, *block3, *block4, *block5, *block6, *block7, *block8;
    block1 = receiveReadOnly(0);
    // block2 = receiveReadOnly(1);
    // block3 = receiveReadOnly(2);
    // block4 = receiveReadOnly(3);
    // block5 = receiveReadOnly(4);
    // block6 = receiveReadOnly(5);
    // block7 = receiveReadOnly(6);
    // block8 = receiveReadOnly(7);

    for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    {
        this->data[0][i] = 0;
        // this->data[1][i] = 0;
        // this->data[2][i] = 0;
        // this->data[3][i] = 0;
        // this->data[4][i] = 0;
        // this->data[5][i] = 0;
        // this->data[6][i] = 0;
        // this->data[7][i] = 0;
    }

    if (block1)
    {
        for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
        {
            this->data[0][i] = block1->data[i];
            Serial.println(this->data[0][i]);
        }
        release(block1);
    }

    // if (block2)
    // {
    //     for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    //     {
    //         this->data[1][i] = block2->data[i];
    //     }
    //     release(block2);
    // }

    // if (block3)
    // {
    //     for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    //     {
    //         this->data[2][i] = block3->data[i];
    //     }
    //     release(block3);
    // }

    // if (block4)
    // {
    //     for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    //     {
    //         this->data[3][i] = block4->data[i];
    //     }
    //     release(block4);
    // }

    // if (block5)
    // {
    //     for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    //     {
    //         this->data[4][i] = block5->data[i];
    //     }
    //     release(block5);
    // }

    // if (block6)
    // {
    //     for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    //     {
    //         this->data[5][i] = block6->data[i];
    //     }
    //     release(block6);
    // }

    // if (block7)
    // {
    //     for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    //     {
    //         this->data[6][i] = block7->data[i];
    //     }
    //     release(block7);
    // }

    // if (block8)
    // {
    //     for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    //     {
    //         this->data[7][i] = block8->data[i];
    //     }
    //     release(block8);
    // }

    //  if(this->block1 && this->block2 && this->block3 && this->block4 && this->block5 && this->block6 && this->block7 && this->block8){
    for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    {
        // Serial.printf(
        //     "%d,%d,%d,%d,%d,%d,%d,%d",
        //     this->data[0][i],
        //     this->data[1][i],
        //     this->data[2][i],
        //     this->data[3][i],
        //     this->data[4][i],
        //     this->data[5][i],
        //     this->data[6][i],
        //     this->data[7][i]);
        // Serial.println("");
        // Serial.println(this->data[0][i]);
    }
    // }
}
#endif