//
//  AudioFile.cpp
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "AudioFile.hpp"
#include "AudioFile_Private.hpp"

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
    
    _pimpl = new pimpl;
    
    _pimpl->mode = mode;
    _pimpl->sndfile = sf_open(filepath, sfmode, &_pimpl->sfInfo);
    _pimpl->totalSize = _pimpl->sfInfo.channels * _pimpl->sfInfo.frames;
    _pimpl->readIndex = 0;
    _pimpl->buf = new float[totalSize()];
    
    //TODO: don't read the whole file at once
    sf_read_float(_pimpl->sndfile, &_pimpl->buf[0], _pimpl->totalSize);
}

AudioFile::~AudioFile()
{
    delete _pimpl;
}

void AudioFile::close()
{
    if (_pimpl->sndfile)
        sf_close(_pimpl->sndfile);
    
    _pimpl->sndfile = 0;
    
    delete []_pimpl->buf;
    _pimpl->buf = 0;
}

AudioFileBufferStatus AudioFile::nextFrame(float **frame)
{
    if (_pimpl->readIndex >= totalSize())
        return AudioFileBufferStatusOutOfBounds;
    
    *frame = &_pimpl->buf[_pimpl->readIndex];
    _pimpl->readIndex += _pimpl->sfInfo.channels;
    
    if (_pimpl->readIndex >= _pimpl->totalSize)
    {
        if (isLooping())
        {
            _pimpl->readIndex -= _pimpl->totalSize;
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
    return _pimpl->sfInfo.samplerate;
}

unsigned long AudioFile::numFrames()
{
    return _pimpl->sfInfo.frames;
}

int AudioFile::numChannels()
{
    return _pimpl->sfInfo.channels;
}

size_t AudioFile::totalSize()
{
    if (_pimpl->totalSize == 0)
        _pimpl->totalSize = _pimpl->sfInfo.channels * _pimpl->sfInfo.frames;
    
    return _pimpl->totalSize;
}

void AudioFile::setLooping(bool looping)
{
    _pimpl->looping = looping;
}

bool AudioFile::isLooping()
{
    return _pimpl->looping;
}

AudioFileMode AudioFile::mode()
{
    return _pimpl->mode;
}

AudioFile::pimpl::~pimpl()
{
    if (sndfile)
        sf_close(sndfile);
    
    if (buf)
        delete []buf;
}
