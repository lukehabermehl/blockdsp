//
//  AudioFile.hpp
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef AudioFile_hpp
#define AudioFile_hpp

#include <sndfile.h>
#include "libblockdsp.h"

enum AudioFileMode
{
    AudioFileModeReadOnly,
    AudioFileModeWriteOnly,
    AudioFileModeReadWrite
};

class AudioFile
{
public:
    AudioFile(const char *filepath, AudioFileMode mode);
    ~AudioFile();
    
    unsigned long sampleRate();
    unsigned long numFrames();
    int numChannels();
    size_t totalSize();
    
    void nextFrame(float *frame);
    
    void close();
    
private:
    SNDFILE *sndfile;
    SF_INFO sfInfo;
    float *buf;
    size_t readIndex;
};

#endif /* AudioFile_hpp */
