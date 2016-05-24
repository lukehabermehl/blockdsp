//
//  AudioManager.hpp
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef AudioManager_hpp
#define AudioManager_hpp

#include <portaudio.h>
#include "libblockdsp.h"
#include "AudioDSPKernel.hpp"

class AudioManager
{
public:
    AudioManager();
    /** Load input from file at specified path. Returns true if successful */
    bool setInputFile(const char *fpath);
    
    /** Specify the audio input source */
    void setInputMode(AudioInputMode mode);
    
    /** Set the function to be called to process audio */
    void setProcessCallback(AudioProcessFunc fn, void *context);
    
    /** Set the number of output channels */
    void setNumOutputChannels(int numOutputChannels);
    
    
    /** Open the audio I/O streams. Returns true if successful */
    bool open();
    
    /** Close the audio I/O streams. Returns true if successful */
    bool close();
    
    /** Start audio processing. Returns true if successful */
    bool start();
    
    /** Stop audio processing. Returns false if successful */
    bool stop();
    
    
    /** Get the current status of the Audio Manager */
    AudioManagerStatus status();
    
private:
    AudioDSPKernel dspKernel;
};


#endif /* AudioManager_hpp */
