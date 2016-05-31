//
//  AudioManager.cpp
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "AudioManager.hpp"
#include "AudioFile.hpp"
#include "AudioManager_Private.hpp"
#include "BDLogger.hpp"

#include <portaudio.h>

AudioManager::AudioManager()
{
    Pa_Initialize();
    int devCount = Pa_GetDeviceCount();
    
    for (int i=0; i<devCount; i++)
    {
        const PaDeviceInfo *info = Pa_GetDeviceInfo(i);
        BDLogFormat("[AudioManager]", "found device: %s", info->name);
    }
    
    _pimpl = new pimpl;
    _pimpl->dspKernel = new AudioDSPKernel;
}

AudioManager::~AudioManager()
{
    delete _pimpl;
}

void AudioManager::setNumOutputChannels(int numOutputChannels)
{
    _pimpl->dspKernel->numOutputChannels = numOutputChannels;
}

void AudioManager::setInputMode(AudioInputMode mode)
{
    switch (mode)
    {
        case AudioInputModeFile:
            _pimpl->dspKernel->useFileInput = true;
            break;
        default:
            _pimpl->dspKernel->useFileInput = false;
            break;
    }
}

void AudioManager::setAudioProcessingUnit(AudioProcessingUnit *unit)
{
    _pimpl->dspKernel->audioProcessingUnit = unit;
}

bool AudioManager::open()
{
    return _pimpl->dspKernel->open(1);
}

bool AudioManager::close()
{
    if (_pimpl->dspKernel->close())
    {
        Pa_Terminate();
        return true;
    }
    
    return false;
}

bool AudioManager::start()
{
    return _pimpl->dspKernel->start();
}

bool AudioManager::stop()
{
    return _pimpl->dspKernel->stop();
}

bool AudioManager::setInputFile(AudioFile *file)
{
    if (file == NULL || file->mode() == AudioFileModeWriteOnly)
        return false;
    
    if (_pimpl->dspKernel->audioFile)
    {
        _pimpl->dspKernel->audioFile->close();
    }
    
    _pimpl->dspKernel->audioFile = file;
    
    return true;
}

AudioManagerStatus AudioManager::status()
{
    return _pimpl->dspKernel->status;
}

void AudioManager::sleep(unsigned long millisec)
{
    Pa_Sleep(millisec);
}

void AudioManager::setLooping(bool looping)
{
    _pimpl->dspKernel->audioFile->setLooping(looping);
}

AudioManager::pimpl::~pimpl()
{
    delete dspKernel;
}

