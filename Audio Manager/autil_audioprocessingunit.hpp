//
//  libblockdsp
//
//  Created by Luke on 5/24/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file autil_audioprocessingunit.hpp */

#ifndef AudioProcessingUnit_hpp
#define AudioProcessingUnit_hpp

#include "autil_parameter.hpp"
#include "autil_parametermap.hpp"
#include <unordered_map>
#include <string>

/** Abstract class */
class AudioProcessingUnit
{
public:
    AudioProcessingUnit();
    virtual ~AudioProcessingUnit();

    /** This method is called by the APU host before starting any processing. Configure variables and default values in an override */
    virtual void setupInitialState();

    /** Override this method to do audio processing */
    virtual void processAudio(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels);
    
    /** Set the maximum number of input channels usable */
    virtual void setMaxInputChannels(uint32_t num);
    /** Set the maximum number of output channels usable */
    virtual void setMaxOutputChannels(uint32_t num);
    
    /** Set the sample rate */
    void setSampleRate(unsigned long sampleRate);
    /** Get the sample rate */
    unsigned long getSampleRate();
    /** Get the name of the APU */
    virtual const char * getName();

    /** Get the number of parameters associated with this APU */
    size_t getNumParameters();
    /** Get the parameter with the given name.
      * @return the parameter or NULL if not found
      */
    APUParameter * getParameterWithName(const char *name);
    /** Get the parameter map */
    const APUParameterMap getParameterMap();
    
    static AudioProcessingUnit * createPassthroughUnit();
    
protected:
    /** This method will be called when the sample rate is set. Override this in subclasses to be handle sample rate changes */
    virtual void onSampleRateChanged();
    /** Add a parameter to the APU 
      * @return true if the parameter was added successfully, false if a parameter with that name exists alreadu
      */
    virtual bool addParameter(APUParameter *param);
    
private:
    class Pimpl;
    Pimpl *_pimpl;
};

#endif /* AudioProcessingUnit_hpp */
