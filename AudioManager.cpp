//
//  AudioManager.cpp
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "AudioManager.hpp"
#include "AudioFile.hpp"


class ScopedPaHandler
{
public:
    ScopedPaHandler() : _result(Pa_Initialize()) {};
    
    ~ScopedPaHandler()
    {
        if (_result == paNoError)
            Pa_Terminate();
    }
    
    PaError result() const {
        return _result;
    }
    
private:
    PaError _result;
};

AudioManager::AudioManager()
{
    ScopedPaHandler paInit;
    if (paInit.result() != paNoError)
    {
        //TODO: notify PaInit error
    }
}

void AudioManager::setNumOutputChannels(int numOutputChannels)
{
    dspKernel.numOutputChannels = numOutputChannels;
}

void AudioManager::setInputMode(AudioInputMode mode)
{
    switch (mode)
    {
        case AudioInputModeFile:
            dspKernel.useFileInput = true;
            break;
        default:
            dspKernel.useFileInput = false;
            break;
    }
}

bool AudioManager::open()
{
    return dspKernel.open(Pa_GetDefaultOutputDevice());
}

bool AudioManager::close()
{
    return dspKernel.close();
}

bool AudioManager::start()
{
    return dspKernel.start();
}

bool AudioManager::stop()
{
    return dspKernel.stop();
}

bool AudioManager::setInputFile(const char *fpath)
{
    AudioFile *audioFile = new AudioFile(fpath, AudioFileModeReadOnly);
    if (audioFile == NULL)
        return false;
    
    if (dspKernel.audioFile)
    {
        dspKernel.audioFile->close();
    }
    
    dspKernel.audioFile = audioFile;
    
    return true;
}

