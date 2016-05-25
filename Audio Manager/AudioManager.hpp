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
#include "audio_constants.h"
#include "AudioDSPKernel.hpp"
#include "AudioProcessingUnit.hpp"

/** Provides an interface for loading and processing audio from a file or input device */
class AudioManager
{
public:
    AudioManager();
    /** Load input from file at specified path. Returns true if successful */
    bool setInputFile(const char *fpath);
    
    /** Specify the audio input source */
    void setInputMode(AudioInputMode mode);
    
    /** Pass in an instance of AudioProcessingUnit to do the DSP */
    void setAudioProcessingUnit(AudioProcessingUnit *unit);
    
    /** Set the number of output channels */
    void setNumOutputChannels(int numOutputChannels);
    
    /** Enable/disable looping when using an input file */
    void setLooping(bool looping);
    
    /** Open the audio I/O streams. Returns true if successful */
    bool open();
    
    /** Close the audio I/O streams. Returns true if successful */
    bool close();
    
    /** Start audio processing. Returns true if successful */
    bool start();
    
    /** Stop audio processing. Returns false if successful */
    bool stop();
    
    void sleep(unsigned long millisec);
    
    
    /** Get the current status of the Audio Manager */
    AudioManagerStatus status();
    
private:
    AudioDSPKernel dspKernel;
};


#endif /* AudioManager_hpp */
