//
//  SimpleDelayEffect.hpp
//  libblockdsp
//
//  Created by Luke on 8/9/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef SimpleDelayEffect_hpp
#define SimpleDelayEffect_hpp

#include "blockdsp.h"

class SimpleDelayEffect : public BlockDSPAPU
{
public:
    SimpleDelayEffect();
    
    unsigned long getMaxDelaySamples();
    virtual void onParameterChanged(BlockDSPParameter *parameter, BlockDSPNumber *number);
    
protected:
    static void onWetDryMixParamChanged(BlockDSPSystem *system, BlockDSPParameter *param, void *value);
    
private:
    void configureSystem();
    unsigned long maxDelaySamples;\
    
    BlockDSPParameter *wetDryParam;
    BlockDSPParameter *delayTimeParam;
    BlockDSPMultiplierNode *wetMultiplier;
    BlockDSPMultiplierNode *dryMultiplier;
    BlockDSPSummerNode *outputSummer;
};

#endif /* SimpleDelayEffect_hpp */
