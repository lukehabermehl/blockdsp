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

class AudioManager::pimpl
{
public:
    ~pimpl();
    AudioDSPKernel *dspKernel;
};

#endif /* AudioManager_Private_h */
