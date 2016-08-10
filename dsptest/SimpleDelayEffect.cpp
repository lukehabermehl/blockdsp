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
    maxDelaySamples_ = 132300; // 3 seconds at 44.1 kHz
}

unsigned long SimpleDelayEffect::getMaxDelaySamples()
{
    return maxDelaySamples_;
}

void SimpleDelayEffect::configureSystem()
{
    BlockDSPSystem *system = getSystem();
    
    BlockDSPDelayLine * delayLine = system->createDelayLine(system->mainInputNode);
    delayLine->setSize(maxDelaySamples_);
    
    BlockDSPMultiplierNode * dryMultiplier = system->createMultiplierNode();
    dryMultiplier->connectInput(system->mainInputNode);
    
    BlockDSPMultiplierNode * wetMultiplier = system->createMultiplierNode();
    wetMultiplier->connectInput(system->mainOutputNode);
}

void SimpleDelayEffect::onWetDryMixParamChanged(BlockDSPSystem *system, BlockDSPParameter *param, void *value)
{
    
}
