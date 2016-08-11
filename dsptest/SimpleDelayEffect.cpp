//
//  SimpleDelayEffect.cpp
//  libblockdsp
//
//  Created by Luke on 8/9/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "SimpleDelayEffect.hpp"

SimpleDelayEffect::SimpleDelayEffect()
: BlockDSPAPU(new BlockDSPSystem())
{
    configureSystem();
    maxDelaySamples = 132300; // 3 seconds at 44.1 kHz
    wetDryParam = NULL;
    wetMultiplier = NULL;
    dryMultiplier = NULL;
}

unsigned long SimpleDelayEffect::getMaxDelaySamples()
{
    return maxDelaySamples;
}

void SimpleDelayEffect::configureSystem()
{
    BlockDSPSystem *system = getSystem();
    
    BlockDSPDelayLine * delayLine = system->createDelayLine(system->mainInputNode);
    delayLine->setSize(maxDelaySamples);
    
    dryMultiplier = system->createMultiplierNode();
    dryMultiplier->connectInput(system->mainInputNode);
    dryMultiplier->coefficient->setFloatValue(1);
    
    wetMultiplier = system->createMultiplierNode();
    //TODO: connect input delay line node
    wetMultiplier->coefficient->setFloatValue(0);
    
    outputSummer = system->createSummerNode();
    outputSummer->connectInput(dryMultiplier);
    outputSummer->connectInput(wetMultiplier);
    
    wetDryParam = system->createParameter("Mix", BlockDSPNumberType::FLOAT, NULL, this);
    delayTimeParam = system->createParameter("Delay Time", BlockDSPNumberType::FLOAT, NULL, this);
}

void SimpleDelayEffect::onParameterChanged(BlockDSPParameter *parameter, BlockDSPNumber *value)
{
    if (parameter == wetDryParam) {
        wetMultiplier->coefficient->setFloatValue(value->floatValue());
        dryMultiplier->coefficient->setFloatValue(1 - value->floatValue());
    }
}
