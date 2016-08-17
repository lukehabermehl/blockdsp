//
//  bdsp_builtin_delay.hpp
//  libblockdsp
//
//  Created by Luke on 8/9/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef bdsp_builtin_delay.hpp
#define bdsp_builtin_delay.hpp

#include "blockdsp.h"

class BDSPSimpleDelayEffect : public BlockDSPAPU
{
public:
    BDSPSimpleDelayEffect();
    
    unsigned long getMaxDelaySamples();
    virtual void onParameterChanged(BlockDSPParameter *parameter, BlockDSPNumberRef number);
    
    static void generateWithCodeBuilder(const char *outputDir);
    
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

#endif /* bdsp_builtin_delay.hpp */
