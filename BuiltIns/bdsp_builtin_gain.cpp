//
//  bdsp_builtin_gain.cpp
//  libblockdsp
//
//  Created by Luke on 10/24/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "bdsp_builtin_gain.hpp"

BDSPGainEffect::BDSPGainEffect() : AudioProcessingUnit()
{
    gainParameter = new APUParameter("Gain", APUNUM_FLOAT, APUNUM_FLOAT(0), APUNUM_FLOAT(1), APUNUM_FLOAT(2));
    gainParameter->setValue(APUNUM_FLOAT(1));

    addParameter(gainParameter);
}

void BDSPGainEffect::setupInitialState()
{
}

void BDSPGainEffect::processAudio(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels)
{
    float fGain = gainParameter->getCurrentValue().floatValue();
    outputBuffer[0] = inputBuffer[0] * fGain;
    if (numOutputChannels == 2)
    {
        if (numInputChannels == 1)
        {
            outputBuffer[1] = outputBuffer[0];
        }
        else
        {
            outputBuffer[1] = inputBuffer[1] * fGain;
        }
    }
}

const char * BDSPGainEffect::getName()
{
    return "Gain Effect";
}
