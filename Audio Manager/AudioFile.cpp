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
    
    sndfile = sf_open(filepath, sfmode, &sfInfo);
    _totalSize = sfInfo.channels * sfInfo.frames;
    readIndex = 0;
    buf = new float[totalSize()];
    
    //TODO: don't read the whole file at once
    sf_read_float(sndfile, &buf[0], _totalSize);
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

AudioFileBufferStatus AudioFile::nextFrame(float **frame)
{
    if (readIndex >= totalSize())
        return AudioFileBufferStatusOutOfBounds;
    
    *frame = &buf[readIndex];
    readIndex += sfInfo.channels;
    
    if (readIndex >= _totalSize)
    {
        if (isLooping())
        {
            readIndex -= _totalSize;
        }
        else
        {
            return AudioFileBufferStatusDoneReading;
        }
    }

    return AudioFileBufferStatusOK;
}

unsigned long AudioFile::sampleRate()
{
    return sfInfo.samplerate;
}

unsigned long AudioFile::numFrames()
{
    return sfInfo.frames;
}

int AudioFile::numChannels()
{
    return sfInfo.channels;
}

size_t AudioFile::totalSize()
{
    if (_totalSize == 0)
        _totalSize = sfInfo.channels * sfInfo.frames;
    
    return _totalSize;
}

void AudioFile::setLooping(bool looping)
{
    _looping = looping;
}

bool AudioFile::isLooping()
{
    return _looping;
}
