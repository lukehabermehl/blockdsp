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

/** An AudioProcessingUnit built with a BlockDSP system */
class BlockDSPAPU : public AudioProcessingUnit
{
public:
    BlockDSPAPU(BlockDSPSystem *system);
    BlockDSPAPU(BlockDSPSystemFactoryFunc systemFactoryFunc);
    
    virtual ~BlockDSPAPU();
    
    /** The system that drives the audio processing */
    virtual BlockDSPSystem *getSystem();
    /** Process a frame of audio. See AudioProcessingUnit */
    virtual void processAudio(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels);
    
    /** Set max number of input channels */
    virtual void setMaxInputChannels(uint32_t num);
    /** Set max number of output channels */
    virtual void setMaxOutputChannels(uint32_t num);
    
    /** Called when a BlockDSPParameter's value is changed
      * @param parameter the BlockDSPParameter instance whose value was modified
      * @param value a number representing the new value. *NOTE* do not keep a reference to this value; its memory will be freed after the callback returns.
      */
    virtual void onParameterChanged(BlockDSPParameter *parameter, APUNumber value);
    
protected:
    /** Use this method to create parameters.
     * @param the name of the parameter
     * @param numberType the value type
     * @param target optional target number or NULL
     *
     * @return NULL if a parameter with the given name already exists in the APU
     */
    BlockDSPParameter * createParameter(const char *name, APUNumberType numberType, APUNumber target);
    
private:
    BlockDSPSystem *system_;
};


#endif /* BlockDSPAPU_hpp */
