//
//  AudioManager.cpp
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_manager.hpp"
#include "autil_manager_private.hpp"
#include "autil_file.hpp"
#include "bdsp_logger.hpp"

#include <portaudio.h>

static const char *kAudioManagerLogPrefix = "[AudioManager]";

AudioManager::AudioManager()
{
    _pimpl = new pimpl;
    _pimpl->dspKernel = new AudioDSPKernel;
    _pimpl->dspKernel->streamFinishedCallback = AudioManager::streamFinishedCallback;
    _pimpl->dspKernel->streamFinishedCallbackCtx = this;
    _pimpl->outputDeviceIndex = 1;
    _pimpl->statusChangedCallback = NULL;
    
    Pa_Initialize();
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
            _pimpl->inputDeviceIndex = -1;
            break;
        default:
            _pimpl->dspKernel->useFileInput = false;
            break;
    }
}

AudioDeviceIndex AudioManager::getInputDeviceIndex()
{
    return _pimpl->inputDeviceIndex;
}

AudioDeviceIndex AudioManager::getOutputDeviceIndex()
{
    return _pimpl->outputDeviceIndex;
}

void AudioManager::setInputDeviceIndex(AudioDeviceIndex devIndex)
{
    _pimpl->inputDeviceIndex = devIndex;
}

void AudioManager::setOutputDeviceIndex(AudioDeviceIndex devIndex)
{
    _pimpl->outputDeviceIndex = devIndex;
}

AudioDeviceInfoRef AudioManager::getDevices()
{
    //Scan for audio devices
    BDLog(kAudioManagerLogPrefix, "Scan for audio devices");
    int devCount = Pa_GetDeviceCount();
    AudioDeviceInfoRef devInfoRef;
    AudioDeviceInfoRef currentRef;
    
    for (int i=0; i<devCount; i++)
    {
        const PaDeviceInfo *info = Pa_GetDeviceInfo(i);
        if (!devInfoRef) {
            devInfoRef = AudioDeviceInfoCreate(i, info->name);
            currentRef = devInfoRef;
        } else {
            currentRef->next = AudioDeviceInfoCreate(i, info->name);
            currentRef = currentRef->next;
        }
    }
    
    return devInfoRef;
}

void AudioManager::setAudioProcessingUnit(AudioProcessingUnit *unit)
{
    _pimpl->dspKernel->audioProcessingUnit = unit;
    unit->setSampleRate(_pimpl->dspKernel->sampleRate);
}

bool AudioManager::open()
{
    return _pimpl->dspKernel->open(getOutputDeviceIndex());
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

void AudioManager::setOnStatusChangedCallback(AudioManagerStatusChangedCallback *cb)
{
    _pimpl->statusChangedCallback = cb;
}

void AudioManager::streamFinishedCallback(void *ctx)
{
    AudioManager *audioManager = (AudioManager *)ctx;
    if (audioManager->_pimpl->statusChangedCallback) {
        audioManager->_pimpl->statusChangedCallback->onStatusChanged(audioManager);
    }
}

AudioManager::pimpl::~pimpl()
{
    delete dspKernel;
}
