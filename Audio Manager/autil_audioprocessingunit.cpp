//
//  AudioProcessingUnit.cpp
//  libblockdsp
//
//  Created by Luke on 5/24/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_audioprocessingunit.hpp"
#include "autil_audioprocessingunit_private.hpp"
#include "bdsp_logger.hpp"

AudioProcessingUnit::AudioProcessingUnit()
{
    _pimpl = new Pimpl();
}

AudioProcessingUnit::~AudioProcessingUnit()
{
    delete _pimpl;
}

void AudioProcessingUnit::setupInitialState()
{
}

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
    _pimpl->sampleRate = sampleRate;

    for (APUParameterMap::Iterator it = _pimpl->parameterMap.begin(); it.valid(); ++it) {
        it.second()->setSampleRate(sampleRate);
    }

    onSampleRateChanged();
}

unsigned long AudioProcessingUnit::getSampleRate()
{
    return _pimpl->sampleRate;
}

const char * AudioProcessingUnit::getName()
{
    return "Default APU";
}

size_t AudioProcessingUnit::getNumParameters()
{
    return _pimpl->parameterMap.size();
}

APUParameter * AudioProcessingUnit::getParameterWithName(const char *name)
{
    return _pimpl->parameterMap.parameterWithName(name);
}

const APUParameterMap AudioProcessingUnit::getParameterMap()
{
    return _pimpl->parameterMap;
}

bool AudioProcessingUnit::addParameter(APUParameter *param)
{
    if (getParameterWithName(param->getName()) != NULL) {
        return false;
    }

    _pimpl->parameterMap.addParameter(param);
    return true;
}

