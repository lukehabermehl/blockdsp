//
//  AudioManager_Private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef AudioManager_Private_h
#define AudioManager_Private_h

#include "AudioManager.hpp"
#include "AudioDSPKernel.hpp"

#include <string.h>

class AudioManager::pimpl
{
public:
    ~pimpl();
    AudioDSPKernel *dspKernel;
    AudioDeviceIndex outputDeviceIndex;
    AudioDeviceInfo *outputDeviceInfo;
};

AudioDeviceInfo *AudioDeviceInfoCreate(AudioDeviceIndex index, const char *name)
{
    AudioDeviceInfo *info = new AudioDeviceInfo;
    info->index = index;
    info->next = NULL;
    
    char *str = (char *)malloc(strlen(name));
    strcpy(str, name);
    info->name = str;
    
    return info;
}

#endif /* AudioManager_Private_h */
