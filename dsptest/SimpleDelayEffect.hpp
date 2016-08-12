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
    virtual void onParameterChanged(BlockDSPParameter *parameter, BlockDSPNumberRef number);
    
protected:
    virtual void onSampleRateChanged();
    
private:
    void configureSystem();
    size_t calculateDelayIndexForMilisec(float ms);
    void updateDelay();
    unsigned long maxDelaySamples;
    float msDelay;
    
    BlockDSPParameter *wetDryParam;
    BlockDSPParameter *delayTimeParam;
    BlockDSPMultiplierNode *wetMultiplier;
    BlockDSPMultiplierNode *dryMultiplier;
    BlockDSPSummerNode *outputSummer;
    BlockDSPDelayLine *delayLine;
    BlockDSPDelayLineNode *delayLineNode;
};

#endif /* SimpleDelayEffect_hpp */
