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

enum AudioInputMode
{
    AudioInputModeFile,
    AudioInputModeMicrophone
};

class AudioManager
{
public:
    AudioManager();
    
    bool setInputFile(const char *fpath);
    void setInputMode(AudioInputMode mode);
    void setProcessCallback(AudioProcessFunc fn, void *context);
    
    void setNumOutputChannels(int numOutputChannels);
    
    bool open();
    bool close();
    bool start();
    bool stop();
    
    AudioManagerStatus status();
    
private:
    AudioDSPKernel dspKernel;
};


#endif /* AudioManager_hpp */
