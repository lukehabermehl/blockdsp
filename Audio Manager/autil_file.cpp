//
//  AudioFile.cpp
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_file.hpp"
#include "autil_file_private.hpp"
#include "bdsp_logger.hpp"
#include "autil_thread_manager.hpp"

#include <vector>

#define FRAMES_PER_FILE_BUFFER 5000

static const char * kAudioFileLoggerPrefix = "[AudioFile]";

void file_buffer_worker(void *ctx)
{
    AudioFile::pimpl *fileImpl = (AudioFile::pimpl *)ctx;
    int writeBuffer = (fileImpl->currentBufIndex == 0) ? 1 : 0;
    sf_seek(fileImpl->sndfile, fileImpl->framesBuffered, SF_SEEK_SET);
    size_t read = sf_readf_float(fileImpl->sndfile, fileImpl->bufs[writeBuffer], FRAMES_PER_FILE_BUFFER);
    fileImpl->framesBuffered += read;
    if (read < FRAMES_PER_FILE_BUFFER)
    {
        if (fileImpl->looping)
        {
            sf_seek(fileImpl->sndfile, 0, SF_SEEK_SET);
            size_t samplesDidRead = read * fileImpl->sfInfo.channels;
            size_t framesToRead = (FRAMES_PER_FILE_BUFFER - read);
            sf_readf_float(fileImpl->sndfile, &(fileImpl->bufs[writeBuffer][samplesDidRead]), framesToRead);
            fileImpl->framesBuffered = framesToRead;
        }
        else
        {
            fileImpl->needsBuffer = false;
        }
    }
    
    fileImpl->isBuffering = false;
}

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
    _pimpl->currentBufIndex = 0;
    _pimpl->samplesRead = 0;
    
    size_t framesPerBuffer = FRAMES_PER_FILE_BUFFER;
    if (framesPerBuffer > numFrames())
    {
        framesPerBuffer = numFrames();
        _pimpl->bufferSize = framesPerBuffer * numChannels();
        _pimpl->needsBuffer = false;
        _pimpl->bufs[0] = new float[_pimpl->bufferSize];
        _pimpl->bufs[1] = NULL;
    }
    else
    {
        _pimpl->bufferSize = framesPerBuffer * numChannels();
        _pimpl->bufs[0] = new float[_pimpl->bufferSize];
        _pimpl->bufs[1] = new float[_pimpl->bufferSize];
        _pimpl->needsBuffer = true;
    }
    sf_seek(_pimpl->sndfile, 0, SF_SEEK_SET);
    _pimpl->framesBuffered = sf_read_float(_pimpl->sndfile, &(_pimpl->bufs[0][0]), _pimpl->bufferSize) / numChannels();
    sf_seek(_pimpl->sndfile, _pimpl->framesBuffered, SF_SEEK_SET);
    AUtilDispatchThread(file_buffer_worker, _pimpl);
}

AudioFile::~AudioFile()
{
    close();
    delete _pimpl;
}

void AudioFile::close()
{
    if (_pimpl->sndfile)
        sf_close(_pimpl->sndfile);
    
    _pimpl->sndfile = 0;
    
    if (_pimpl->bufs[0]) {
        delete [] _pimpl->bufs[0];
        _pimpl->bufs[0] = NULL;
    }
    if (_pimpl->bufs[1]) {
        delete [] _pimpl->bufs[1];
        _pimpl->bufs[1] = 0;
    }
}

AudioFileBufferStatus AudioFile::nextFrame(float **frame)
{
    if (_pimpl->readIndex >= _pimpl->bufferSize)
        return AudioFileBufferStatusOutOfBounds;
    
    *frame = &((_pimpl->bufs[_pimpl->currentBufIndex])[_pimpl->readIndex]);
    _pimpl->readIndex += _pimpl->sfInfo.channels;
    _pimpl->samplesRead += _pimpl->sfInfo.channels;
    
    if (_pimpl->readIndex >= _pimpl->bufferSize)
    {
        if (_pimpl->currentBufIndex == 0)
        {
            _pimpl->currentBufIndex = 1;
        }
        else
        {
            _pimpl->currentBufIndex = 0;
        }
        _pimpl->readIndex = 0;
        
        if (_pimpl->needsBuffer && !_pimpl->isBuffering)
        {
            _pimpl->isBuffering = true;
            AUtilDispatchThread(file_buffer_worker, _pimpl);
        }
    }
    
    if (_pimpl->samplesRead >= totalSize())
    {
        if (isLooping())
        {
            _pimpl->samplesRead = 0;
        }
        else
        {
            BDLog(kAudioFileLoggerPrefix, "Done reading");
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
}
