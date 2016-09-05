//
//  bdsp_builtin_delay.hpp
//  libblockdsp
//
//

#ifndef BDSP_BUILTIN_DELAY_HPP_
#define BDSP_BUILTIN_DELAY_HPP_

#include "bdsp_apu.hpp"

class BDSPSimpleDelayEffect : public BlockDSPAPU
{
public:
    BDSPSimpleDelayEffect();
    
    unsigned long getMaxDelaySamples();
    virtual void onParameterChanged(BlockDSPParameter *parameter, APUNumber number);
    
    static void generateWithCodeBuilder(const char *outputDir);
    
    virtual const char * getName();
    
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

#endif /* BDSP_BUILTIN_DELAY_HPP_ */
