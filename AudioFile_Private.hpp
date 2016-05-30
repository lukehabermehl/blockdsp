//
//  AudioFile_Private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef AudioFile_Private_h
#define AudioFile_Private_h

#include "AudioFile.hpp"
#include <sndfile.h>

class AudioFile::pimpl
{
public:
    ~pimpl();
    
    SNDFILE *sndfile;
    SF_INFO sfInfo;
    float *buf;
    size_t readIndex;
    size_t totalSize;
    bool looping;
};

#endif /* AudioFile_Private_h */
