//
//  bdsp_builtin_gain.hpp
//  libblockdsp
//
//  Created by Luke on 10/24/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef bdsp_builtin_gain_hpp
#define bdsp_builtin_gain_hpp

#include "autil_audioprocessingunit.hpp"

class BDSPGainEffect : public AudioProcessingUnit
{
public:
    BDSPGainEffect();
    virtual void setupInitialState();
    virtual const char * getName();
    virtual void processAudio(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels);

protected:
    APUParameter *gainParameter;
};

#endif /* bdsp_builtin_gain_hpp */
