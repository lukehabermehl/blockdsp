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
//        if (_result == paNoError)
//            //Pa_Terminate();
    }
    
    PaError result() const {
        return _result;
    }
    
private:
    PaError _result;
};

AudioManager::AudioManager()
{
    Pa_Initialize();
    
    int devCount = Pa_GetDeviceCount();
    printf("AudioManager: Device Count: %d\n", devCount);
    for (int i=0; i<devCount; i++)
    {
        const PaDeviceInfo *info = Pa_GetDeviceInfo(i);
        printf("AudioManager: found device: %s\n", info->name);
        
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

void AudioManager::setProcessCallback(AudioProcessFunc fn, void *context)
{
    dspKernel.dspCallback = fn;
    dspKernel.dspCallbackContext = context;
}

bool AudioManager::open()
{
    return dspKernel.open(1);
}

bool AudioManager::close()
{
    if (dspKernel.close())
    {
        Pa_Terminate();
        return true;
    }
    
    return false;
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

AudioManagerStatus AudioManager::status()
{
    return dspKernel.status;
}

void AudioManager::sleep(unsigned long millisec)
{
    Pa_Sleep(millisec);
}

void AudioManager::setLooping(bool looping)
{
    dspKernel.audioFile->setLooping(looping);
}

