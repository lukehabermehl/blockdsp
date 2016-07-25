//
//  AudioManager_Private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef AudioManager_Private_h
#define AudioManager_Private_h

#include "autil_manager.hpp"
#include "autil_dspkernel.hpp"

#include <string.h>

class AudioManager::pimpl
{
public:
    ~pimpl();
    AudioDSPKernel *dspKernel;
    AudioDeviceIndex outputDeviceIndex;
    AudioDeviceIndex inputDeviceIndex;
};

AudioDeviceInfoRef AudioDeviceInfoCreate(AudioDeviceIndex index, const char *name)
{
    AudioDeviceInfoRef devInfoRef(new AudioDeviceInfo());
    
    devInfoRef->index = index;
    devInfoRef->next = NULL;
    
    char *str = (char *)malloc(strlen(name));
    strcpy(str, name);
    devInfoRef->name = str;
    
    return devInfoRef;
}

#endif /* AudioManager_Private_h */
