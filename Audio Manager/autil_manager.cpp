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
    _pimpl->outputDeviceIndex = 1;
    _pimpl->inputDeviceInfo = NULL;
    _pimpl->outputDeviceInfo = NULL;
    
    Pa_Initialize();
    refreshDevices();
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

AudioDeviceIndex AudioManager::getInputDeviceIndex()
{
    return _pimpl->inputDeviceIndex;
}

AudioDeviceIndex AudioManager::getOutputDeviceIndex()
{
    return _pimpl->outputDeviceIndex;
}

void AudioManager::setOutputDeviceIndex(unsigned int devIndex)
{
    _pimpl->outputDeviceIndex = devIndex;
}

AudioDeviceInfo* AudioManager::getOutputDeviceInfo()
{
    return _pimpl->outputDeviceInfo;
}

void AudioManager::refreshDevices()
{
    //Delete existing AudioDeviceInfo * lists
    AudioDeviceInfo *dev = _pimpl->inputDeviceInfo;
    while (dev) {
        AudioDeviceInfo *tmp = dev;
        dev = dev->next;
        delete tmp;
    }
    
    dev = _pimpl->outputDeviceInfo;
    while (dev) {
        AudioDeviceInfo *tmp = dev;
        dev = dev->next;
        delete tmp;
    }
    
    _pimpl->inputDeviceInfo = NULL;
    _pimpl->outputDeviceInfo = NULL;
    
    //Scan for audio devices
    BDLog(kAudioManagerLogPrefix, "Scan for audio devices");
    int devCount = Pa_GetDeviceCount();
    AudioDeviceInfo *outputDevice = NULL;
    AudioDeviceInfo *inputDevice = NULL;
    
    for (int i=0; i<devCount; i++)
    {
        const PaDeviceInfo *info = Pa_GetDeviceInfo(i);
        AudioDeviceInfo *dev = AudioDeviceInfoCreate(i, info->name);
        if (info->maxInputChannels > 0) {
            if (inputDevice == NULL) {
                _pimpl->inputDeviceInfo = dev;
                inputDevice = dev;
            } else {
                inputDevice->next = dev;
                inputDevice = inputDevice->next;
            }
        }
        if (info->maxOutputChannels > 0) {
            if (outputDevice == NULL) {
                _pimpl->outputDeviceInfo = dev;
                outputDevice = dev;
            } else {
                outputDevice->next = dev;
                outputDevice = outputDevice->next;
            }
        }
    }
}

void AudioManager::setAudioProcessingUnit(AudioProcessingUnit *unit)
{
    _pimpl->dspKernel->audioProcessingUnit = unit;
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

AudioManager::pimpl::~pimpl()
{
    delete dspKernel;
}

