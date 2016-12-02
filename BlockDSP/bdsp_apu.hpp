//
//  libblockdsp
//
//  Created by Luke on 5/24/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file bdsp_apu.hpp */

#ifndef BlockDSPAPU_hpp
#define BlockDSPAPU_hpp

#include "autil_audioprocessingunit.hpp"
#include "bdsp_system.hpp"
#include "bdsp_parameter.hpp"
#include "autil_number.hpp"

typedef BlockDSPSystem* (*BlockDSPSystemFactoryFunc)(BlockDSPAPU *);

///When creating Block IDs, ascend from this value
extern const BlockDSPNodeID kFirstUserBlockID;

/** An AudioProcessingUnit built with a BlockDSP system */
class BlockDSPAPU : public AudioProcessingUnit
{
public:
    BlockDSPAPU(BlockDSPSystem *system);
    BlockDSPAPU(BlockDSPSystemFactoryFunc systemFactoryFunc);
    
    virtual ~BlockDSPAPU();
    
    /** The system that drives the audio processing */
    virtual BlockDSPSystem *getSystem();

    /** Configure variables and default values before processing starts. If overriding, make sure to call the method on the base class */
    virtual void setupInitialState();
    
    /** Process a frame of audio. See AudioProcessingUnit */
    virtual void processAudio(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels);
    
    /** Set max number of input channels */
    virtual void setMaxInputChannels(uint32_t num);
    /** Set max number of output channels */
    virtual void setMaxOutputChannels(uint32_t num);
    
    /** Called when a BlockDSPParameter's value is changed
      * @param parameter the BlockDSPParameter instance whose value was modified
      */
    virtual void onParameterChanged(BlockDSPParameter *parameter);
    
protected:
    /** Use this method to create parameters.
     * @param the name of the parameter
     * @param numberType the value type
     * @param target optional target number or NULL
     * @param minValue minimum allowed value
     * @param maxValue maximum allowed value
     * @param defaultValue default value for the parameter
     *
     * @return NULL if a parameter with the given name already exists in the APU
     */
    BlockDSPParameter * createParameter(const char *name, APUNumberType numberType, APUNumber minValue, APUNumber maxValue, APUNumber defaultValue);
    
private:
    BlockDSPSystem *system_;
};


#endif /* BlockDSPAPU_hpp */
