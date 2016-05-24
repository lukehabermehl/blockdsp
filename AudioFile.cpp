//
//  AudioFile.cpp
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "AudioFile.hpp"

AudioFile::AudioFile(const char *filepath, AudioFileMode mode)
{
    int sfmode;
    switch (mode)
    {
        case AudioFileModeReadOnly:
            sfmode = SFM_READ;
            break;
        case AudioFileModeWriteOnly:
            sfmode = SFM_WRITE;
            break;
        case AudioFileModeReadWrite:
            sfmode = SFM_RDWR;
            break;
    }
    
    size_t nItems = totalSize();
    sndfile = sf_open(filepath, sfmode, &sfInfo);
    readIndex = 0;
    buf = new float[nItems];
    _totalSize = sfInfo.frames * sfInfo.channels;
    
    //TODO: don't read the whole file at once
    sf_read_float(sndfile, &buf[0], nItems);
}

AudioFile::~AudioFile()
{
    delete []buf;
    if (sndfile)
        sf_close(sndfile);
    
    if (buf)
        delete []buf;
    
}

void AudioFile::close()
{
    if (sndfile)
        sf_close(sndfile);
    
    sndfile = 0;
    
    delete []buf;
    buf = 0;
}

int AudioFile::nextFrame(float **frame)
{
    if (readIndex >= _totalSize)
        return kOutOfBounds;
    
    *frame = &buf[readIndex++];
    
    if (readIndex >= _totalSize)
        return kDoneReading;

    return kNoError;
}

unsigned long AudioFile::sampleRate()
{
    return sfInfo.samplerate;
}

unsigned long AudioFile::numFrames()
{
    return sfInfo.frames;
}

size_t AudioFile::totalSize()
{
    return _totalSize;
}
