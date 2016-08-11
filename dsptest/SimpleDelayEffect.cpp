//
//  SimpleDelayEffect.cpp
//  libblockdsp
//
//  Created by Luke on 8/9/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "SimpleDelayEffect.hpp"
#include <cmath>

static const char * kLogPrefix = "[SimpleDelayEffect]";

SimpleDelayEffect::SimpleDelayEffect()
: BlockDSPAPU(new BlockDSPSystem())
{
    maxDelaySamples = 132300; // 3 seconds at 44.1 kHz
    wetDryParam = NULL;
    wetMultiplier = NULL;
    dryMultiplier = NULL;
    delayLine = NULL;
    delayLineNode = NULL;
    msDelay = 100; //15 ms delay default
    
    configureSystem();
}

unsigned long SimpleDelayEffect::getMaxDelaySamples()
{
    return maxDelaySamples;
}

void SimpleDelayEffect::configureSystem()
{
    BlockDSPSystem *system = getSystem();
    
    delayLine = system->createDelayLine(system->mainInputNode);
    delayLine->setSize(maxDelaySamples);
    
    dryMultiplier = system->createMultiplierNode();
    dryMultiplier->connectInput(system->mainInputNode);
    dryMultiplier->coefficient->setFloatValue(0.7);
    
    wetMultiplier = system->createMultiplierNode();
    wetMultiplier->coefficient->setFloatValue(0.2);
    
    updateDelay();
    
    outputSummer = system->createSummerNode();
    outputSummer->connectInput(dryMultiplier);
    outputSummer->connectInput(wetMultiplier);
    
    system->mainOutputNode = outputSummer;
    
    wetDryParam = system->createParameter("Mix", BlockDSPNumberType::FLOAT, NULL, this);
    delayTimeParam = system->createParameter("Delay Time", BlockDSPNumberType::FLOAT, NULL, this);
}

void SimpleDelayEffect::onParameterChanged(BlockDSPParameter *parameter, BlockDSPNumber *value)
{
    if (parameter == wetDryParam) {
        wetMultiplier->coefficient->setFloatValue(value->floatValue());
        dryMultiplier->coefficient->setFloatValue(1 - value->floatValue());
    }
    
    else if (parameter == delayTimeParam) {
        msDelay = value->floatValue();
        updateDelay();
    }
}

void SimpleDelayEffect::onSampleRateChanged()
{
    BDLogFormat(kLogPrefix, "onSampleRateChanged(): %lu", getSampleRate());
    updateDelay();
}

void SimpleDelayEffect::updateDelay()
{
    size_t samples = calculateDelayIndexForMilisec(msDelay);
    BDLogFormat(kLogPrefix, "updateDelay(): samples = %lu", samples);
    if (samples > maxDelaySamples)
        samples = maxDelaySamples;
    
    BlockDSPDelayLineNode *newDLNode = delayLine->nodeForDelayIndex(samples);
    wetMultiplier->connectInput(newDLNode);
    if (delayLineNode)
        delete delayLineNode;
    
    delayLineNode = newDLNode;
}

size_t SimpleDelayEffect::calculateDelayIndexForMilisec(float ms)
{
    float fSampleRate = (float)getSampleRate();
    float fNumSamples = (ms * fSampleRate) / 1000.f;
    if (fNumSamples > maxDelaySamples) {
        return maxDelaySamples;
    }
    BDLogFormat(kLogPrefix, "fNumSamples = %f", fNumSamples);
    return floorf(fNumSamples);
}
