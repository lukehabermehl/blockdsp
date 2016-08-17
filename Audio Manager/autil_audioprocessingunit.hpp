//
//  libblockdsp
//
//  Created by Luke on 5/24/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file autil_audioprocessingunit.hpp */

#ifndef AudioProcessingUnit_hpp
#define AudioProcessingUnit_hpp

#include <stdlib.h>

/** Abstract class */
class AudioProcessingUnit
{
public:
    /** Override this method to do audio processing */
    virtual void processAudio(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels);
    
    /** Set the maximum number of input channels usable */
    virtual void setMaxInputChannels(uint32_t num);
    /** Set the maximum number of output channels usable */
    virtual void setMaxOutputChannels(uint32_t num);
    
    virtual ~AudioProcessingUnit() {};
    
    /** Set the sample rate */
    void setSampleRate(unsigned long sampleRate);
    /** Get the sample rate */
    unsigned long getSampleRate();
    /** Get the name of the APU */
    virtual const char * getName();
    
    static AudioProcessingUnit * createPassthroughUnit();
    
protected:
    /** This method will be called when the sample rate is set. Override this in subclasses to be handle sample rate changes */
    virtual void onSampleRateChanged();
    
private:
    unsigned long sampleRate_;
};


#endif /* AudioProcessingUnit_hpp */
