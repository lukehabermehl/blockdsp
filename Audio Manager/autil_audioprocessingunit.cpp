//
//  AudioProcessingUnit.cpp
//  libblockdsp
//
//  Created by Luke on 5/24/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_audioprocessingunit.hpp"

void AudioProcessingUnit::processAudio(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels)
{
    outputBuffer[0] = inputBuffer[0];
    
    if (numInputChannels == 1 && numOutputChannels == 2)
    {
        outputBuffer[1] = outputBuffer[0];
    }
    
    else if (numInputChannels == 1 && numOutputChannels == 2)
    {
        outputBuffer[1] = inputBuffer[1];
    }
}

AudioProcessingUnit * AudioProcessingUnit::createPassthroughUnit()
{
    return new AudioProcessingUnit;
}

void AudioProcessingUnit::setMaxInputChannels(uint32_t num)
{
}

void AudioProcessingUnit::setMaxOutputChannels(uint32_t num)
{
}

void AudioProcessingUnit::onSampleRateChanged()
{
}

void AudioProcessingUnit::setSampleRate(unsigned long sampleRate)
{
    sampleRate_ = sampleRate;
    onSampleRateChanged();
}

unsigned long AudioProcessingUnit::getSampleRate()
{
    return sampleRate_;
}

const char * AudioProcessingUnit::getName()
{
    return "Default APU";
}
