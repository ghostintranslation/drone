#ifndef Voice_h
#define Voice_h

#include <Audio.h>
#include "Motherboard/Vca.h"
#include "Motherboard/Vcc.h"

/*
 * Voice
 */
class Voice : public AudioStream
{
private:
  audio_block_t *inputQueueArray[5];

  // TODO: Add sawtooth and square?
  AudioSynthWaveformModulated *sine;

  AudioSynthNoisePink *noise;        // xy=174,124
  AudioFilterStateVariable *filter1; // xy=366,135
  AudioFilterStateVariable *filter2; // xy=366,135
  AudioFilterStateVariable *filter3; // xy=366,135
  AudioFilterStateVariable *filter4; // xy=366,135
  AudioFilterStateVariable *filter5; // xy=366,135
  AudioFilterStateVariable *filter6; // xy=366,135
  float noiseFilterResonance = 1;

  Vcc *shapeCrossfader;

  AudioRecordQueue *audioOutQueue1;
  AudioPlayQueue *filtersFrequencyQueue;
  AudioPlayQueue *sineFrequencyQueue;
  AudioPlayQueue *gainQueue;
  AudioPlayQueue *shapeCrossfaderQueue;
  Vca *vca1;

  int16_t frequencyData[AUDIO_BLOCK_SAMPLES] = {0};
  int16_t sineFrequencyData[AUDIO_BLOCK_SAMPLES] = {0};
  int16_t filtersFrequencyData[AUDIO_BLOCK_SAMPLES] = {0};
  int16_t gainData[AUDIO_BLOCK_SAMPLES] = {0};
  int16_t shapeCrossfaderData[AUDIO_BLOCK_SAMPLES] = {0};

  int16_t fmRange = INT16_MAX;

  float frequencyCenter = 220.0;
  float vOct = 0;
  float frequency = frequencyCenter;

public:
  Voice();
  void update(void);
};

/**
 * Constructor
 */
inline Voice::Voice()
    : AudioStream(5, inputQueueArray)
{
  // Sine and Triangle sources
  this->sine = new AudioSynthWaveformModulated();
  this->sine->begin(WAVEFORM_SINE);
  this->sine->frequencyModulation(1);
  this->sine->frequency(this->frequency);
  this->sine->amplitude(1);

  this->shapeCrossfader = new Vcc();

  // Noise source
  this->noise = new AudioSynthNoisePink();
  this->noise->amplitude(0.05);

  this->filter1 = new AudioFilterStateVariable();
  this->filter1->frequency(this->frequency);
  this->filter1->resonance(7);
  this->filter1->octaveControl(1);

  this->filter2 = new AudioFilterStateVariable();
  this->filter2->frequency(this->frequency);
  this->filter2->resonance(7);
  this->filter2->octaveControl(1);

  this->filter3 = new AudioFilterStateVariable();
  this->filter3->frequency(this->frequency);
  this->filter3->resonance(7);
  this->filter3->octaveControl(1);

  this->filter4 = new AudioFilterStateVariable();
  this->filter4->frequency(this->frequency);
  this->filter4->resonance(7);
  this->filter4->octaveControl(1);

  this->filter5 = new AudioFilterStateVariable();
  this->filter5->frequency(this->frequency);
  this->filter5->resonance(7);
  this->filter5->octaveControl(1);

  this->filter6 = new AudioFilterStateVariable();
  this->filter6->frequency(this->frequency);
  this->filter6->resonance(7);
  this->filter6->octaveControl(1);

  this->audioOutQueue1 = new AudioRecordQueue();
  this->audioOutQueue1->begin();

  this->filtersFrequencyQueue = new AudioPlayQueue();

  this->sineFrequencyQueue = new AudioPlayQueue();

  this->gainQueue = new AudioPlayQueue();

  this->shapeCrossfaderQueue = new AudioPlayQueue();

  this->vca1 = new Vca();
  // this->vca2 = new Vca();

  // Audio output 1
  new AudioConnection(*this->sineFrequencyQueue, 0, *this->sine, 0);
  new AudioConnection(*this->sine, 0, *this->shapeCrossfader, 0);
  new AudioConnection(*this->shapeCrossfaderQueue, 0, *this->shapeCrossfader, 2);
  new AudioConnection(*this->shapeCrossfader, 0, *this->vca1, 0);
  new AudioConnection(*this->gainQueue, 0, *this->vca1, 1);
  new AudioConnection(*this->vca1, 0, *this->audioOutQueue1, 0);

  // Audio output 2
  new AudioConnection(*this->noise, 0, *this->filter1, 0);
  new AudioConnection(*this->sineFrequencyQueue, 0, *this->filter1, 1);
  new AudioConnection(*this->filter1, 0, *this->filter2, 0);
  new AudioConnection(*this->sineFrequencyQueue, 0, *this->filter2, 1);
  new AudioConnection(*this->filter2, 0, *this->filter3, 0);
  new AudioConnection(*this->sineFrequencyQueue, 0, *this->filter3, 1);
  new AudioConnection(*this->filter3, 0, *this->filter4, 0);
  new AudioConnection(*this->sineFrequencyQueue, 0, *this->filter4, 1);
  new AudioConnection(*this->filter4, 0, *this->filter5, 0);
  new AudioConnection(*this->sineFrequencyQueue, 0, *this->filter5, 1);
  new AudioConnection(*this->filter5, 0, *this->filter6, 0);
  new AudioConnection(*this->sineFrequencyQueue, 0, *this->filter6, 1);
  new AudioConnection(*this->filter6, 0, *this->shapeCrossfader, 1);
}

inline void Voice::update(void)
{
  // Receive input data
  audio_block_t *freqBlock;
  audio_block_t *fmBlock;
  audio_block_t *gainBlock;
  audio_block_t *shapeBlock;
  audio_block_t *fmRangeBlock;

  // Frequency block
  // Receiving data from input of the block, and passing it to the queue which is connected to the other objects
  freqBlock = receiveReadOnly(0);

  if (freqBlock)
  {
    this->frequencyCenter = 55 + (float)(freqBlock->data[0] + 32668) / 65335.0 * (880 - 55);
    release(freqBlock);
  }

  // FM block
  // Receiving data from input of the block, and passing it to the queue which is connected to the other objects
  fmBlock = receiveReadOnly(1);

  if (fmBlock)
  {
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    {
    }
    this->vOct = (float)(fmBlock->data[0] + 32668) / 65335.0;

    release(fmBlock);
  }

  // Setting the frequency
  this->frequency = this->frequencyCenter * pow(2, this->vOct * 5);
  this->sine->frequency(this->frequency);
  this->filter1->frequency(this->frequency);
  this->filter2->frequency(this->frequency);
  this->filter3->frequency(this->frequency);
  this->filter4->frequency(this->frequency);
  this->filter5->frequency(this->frequency);
  this->filter6->frequency(this->frequency);

  // Gain block
  // Receiving data from input of the block, and passing it to the queue which is connected to the other objects
  gainBlock = receiveReadOnly(2);

  if (gainBlock)
  {
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    {
      this->gainData[i] = gainBlock->data[i];
    }

    release(gainBlock);
  }

  this->gainQueue->play(this->gainData, AUDIO_BLOCK_SAMPLES);

  // Shape block
  shapeBlock = receiveReadOnly(3);

  if (shapeBlock)
  {
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    {
      this->shapeCrossfaderData[i] = shapeBlock->data[i];
      float newVal = (((float)shapeBlock->data[i] + (float)32668) / (float)65335) * 7;
      noiseFilterResonance = noiseFilterResonance * (float)0.5 + newVal * (float)0.5;
    }
    this->filter1->resonance(noiseFilterResonance);
    this->filter2->resonance(noiseFilterResonance);
    this->filter3->resonance(noiseFilterResonance);
    this->filter4->resonance(noiseFilterResonance);
    this->filter5->resonance(noiseFilterResonance);
    this->filter6->resonance(noiseFilterResonance);

    // Volume needs to go way down when resonance goes up
    this->noise->amplitude(0.0009); // vol
    release(shapeBlock);
  }

  this->shapeCrossfaderQueue->play(this->shapeCrossfaderData, AUDIO_BLOCK_SAMPLES);

  // Transmitting the audio queue 1
  if (this->audioOutQueue1->available())
  {
    while (this->audioOutQueue1->available())
    {
      audio_block_t *audioBlock = allocate();
      if (audioBlock)
      {
        int16_t *queueData = this->audioOutQueue1->readBuffer();
        for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
        {
          audioBlock->data[i] = queueData[i];
        }
        transmit(audioBlock, 0);
        release(audioBlock);
        this->audioOutQueue1->freeBuffer();
      }
    }
  }

  // FM range block
  fmRangeBlock = receiveReadOnly(4);
  if (fmRangeBlock)
  {
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    {
      this->sineFrequencyData[i] = fmRangeBlock->data[i];
    }

    release(fmRangeBlock);
  }

  this->sineFrequencyQueue->play(this->sineFrequencyData, AUDIO_BLOCK_SAMPLES);
}

#endif
