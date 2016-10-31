//
//  bdsp_builtin_delay.cpp
//  libblockdsp
//
//

#include "bdsp_builtin_delay.hpp"
#include "bdsp_logger.hpp"
#include <cmath>

static const char * kLogPrefix = "[BDSPSimpleDelayEffect]";

BDSPSimpleDelayEffect::BDSPSimpleDelayEffect()
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

unsigned long BDSPSimpleDelayEffect::getMaxDelaySamples()
{
    return maxDelaySamples;
}

void BDSPSimpleDelayEffect::setupInitialState()
{
    BlockDSPAPU::setupInitialState();
}

void BDSPSimpleDelayEffect::configureSystem()
{
    BlockDSPSystem *system = getSystem();
    
    delayLine = system->createDelayLine(DELAY_LINE_ID, system->mainInputNode);
    delayLine->setSize(maxDelaySamples);
    
    dryMultiplier = system->createMultiplierNode(DRY_MULTIPLIER_ID);
    dryMultiplier->connectInput(system->mainInputNode);
    dryMultiplier->coefficient.setFloatValue(0.7);
    
    wetMultiplier = system->createMultiplierNode(WET_MULTIPLIER_ID);
    wetMultiplier->coefficient.setFloatValue(0.2);
    
    updateDelay();
    
    outputSummer = system->createSummerNode(OUTPUT_SUMMER_ID);
    outputSummer->connectInput(dryMultiplier);
    outputSummer->connectInput(wetMultiplier);
    
    system->mainOutputNode = outputSummer;
    
    wetDryParam = createParameter("Mix", APUNumberType::APUNUM_FLOAT, APUNUM_FLOAT(0), APUNUM_FLOAT(1), APUNUM_FLOAT(0.5));

    delayTimeParam = createParameter("Delay Time", APUNumberType::APUNUM_FLOAT, APUNUM_FLOAT(0), APUNUM_FLOAT(30), APUNUM_FLOAT(10));
}

void BDSPSimpleDelayEffect::onParameterChanged(BlockDSPParameter *parameter, APUNumber value)
{
    if (parameter == wetDryParam) {
        wetMultiplier->coefficient.setFloatValue(value.floatValue());
        dryMultiplier->coefficient.setFloatValue(1 - value.floatValue());
    }
    
    else if (parameter == delayTimeParam) {
        msDelay = value.floatValue();
        updateDelay();
    }
}

void BDSPSimpleDelayEffect::onSampleRateChanged()
{
    BDLogFormat(kLogPrefix, "onSampleRateChanged(): %lu", getSampleRate());
    updateDelay();
}

void BDSPSimpleDelayEffect::updateDelay()
{
    size_t samples = calculateDelayIndexForMilisec(msDelay);
    BDLogFormat(kLogPrefix, "updateDelay(): samples = %lu", samples);
    if (samples > maxDelaySamples)
        samples = maxDelaySamples;
    
    BlockDSPDelayLineNode *newDLNode = delayLine->nodeForDelayIndex(DELAY_LINE_TAP_ID, samples);
    wetMultiplier->connectInput(newDLNode);
    if (delayLineNode)
        delete delayLineNode;
    
    delayLineNode = newDLNode;
}

size_t BDSPSimpleDelayEffect::calculateDelayIndexForMilisec(float ms)
{
    float fSampleRate = (float)getSampleRate();
    float fNumSamples = (ms * fSampleRate) / 1000.f;
    if (fNumSamples > maxDelaySamples) {
        return maxDelaySamples;
    }
    BDLogFormat(kLogPrefix, "fNumSamples = %f", fNumSamples);
    return floorf(fNumSamples);
}

const char * BDSPSimpleDelayEffect::getName()
{
    return "SimpleDelay";
}
