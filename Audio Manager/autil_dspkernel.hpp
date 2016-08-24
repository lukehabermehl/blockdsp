//
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file autil_dspkernel.hpp */

#ifndef AudioDSPKernel_hpp
#define AudioDSPKernel_hpp

#include <portaudio.h>

#include "autil_file.hpp"
#include "autil_audioprocessingunit.hpp"
#include "autil_manager.hpp"
#include "bdsp_logger.hpp"

static const char *kAudioDSPKernelLogPrefix = "[AudioDSPKernel]";

class AudioDSPKernel {
public:
    typedef void (*StreamFinishedCallback)(void *);
    AudioDSPKernel()
    {
        audioProcessingUnit = NULL;
        passthroughUnit = AudioProcessingUnit::createPassthroughUnit();
        numInputChannels = 1;
        numOutputChannels = 1;
        stream = 0;
        useFileInput = false;
        audioFile = 0;
        status = AudioManagerStatusDone;
        streamFinishedCallback = NULL;
        streamFinishedCallbackCtx = NULL;
    }
    
    ~AudioDSPKernel()
    {
        if (stream)
            close();
        
        if (audioFile)
        {
            audioFile->close();
        }
        
        delete passthroughUnit;
    }
    
    bool open(PaDeviceIndex outputDevIndex)
    {
        PaStreamParameters outputParameters;
        outputParameters.device = outputDevIndex;
        if (outputParameters.device == paNoDevice)
        {
            BDLogError(kAudioDSPKernelLogPrefix, "Failed to find output device!");
            return false;
        }
        
        outputParameters.channelCount = numOutputChannels;
        outputParameters.sampleFormat = paFloat32;
        const PaDeviceInfo *devInfo = Pa_GetDeviceInfo(outputParameters.device);
        outputParameters.suggestedLatency = devInfo->defaultLowOutputLatency;
        
        outputParameters.hostApiSpecificStreamInfo = NULL;
        
        if (useFileInput)
            sampleRate = audioFile->sampleRate();
        else
        {
            const PaDeviceInfo *inputDevInfo = Pa_GetDeviceInfo(Pa_GetDefaultInputDevice());
            sampleRate = inputDevInfo->defaultSampleRate;
        }
        
        BDLogFormat(kAudioDSPKernelLogPrefix, "Open stream with sample rate: %lu", sampleRate);
        
        PaError err = Pa_OpenStream(&stream,
                                    NULL,
                                    &outputParameters,
                                    sampleRate,
                                    1,
                                    0,
                                    &AudioDSPKernel::paCallback,
                                    this);
        
        if (err != paNoError)
        {
            BDLogError(kAudioDSPKernelLogPrefix, "Failed to open stream! PAError code: %d", err);
            return false;
        }
        
        err = Pa_SetStreamFinishedCallback(stream, &AudioDSPKernel::paStreamFinished);
        if (err != paNoError)
        {
            Pa_CloseStream(stream);
            stream = 0;
            return false;
        }
        
        return true;
    }
    
    bool close()
    {
        if (stream == 0)
            return false;
        
        PaError err = Pa_CloseStream(stream);
        stream = 0;
        
        return (err == paNoError);
    }
    
    bool start()
    {
        if (stream == 0)
            return false;
        
        PaError err = Pa_StartStream(stream);
        status = AudioManagerStatusRunning;
        return (err == paNoError);
    }
    
    bool stop()
    {
        if (stream == 0)
            return false;
        
        PaError err = Pa_StopStream(stream);
        status = AudioManagerStatusDone;
        return (err == paNoError);
    }
    
    void paStreamFinishedMethod()
    {
        status = AudioManagerStatusDone;
        if (streamFinishedCallback) {
            streamFinishedCallback(streamFinishedCallbackCtx);
        }
    }
    
    int paCallbackMethod(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags)
    {
        float *out = (float *)outputBuffer;
        float *in = 0;
        if (!useFileInput)
        {
            in = (float *)inputBuffer;
        }
        
        PaStreamCallbackResult ret = paContinue;
        
        for (unsigned long i=0; i<framesPerBuffer; i++)
        {
            if (useFileInput)
            {
                if (audioFile->nextFrame(&in) != AudioFileBufferStatusOK)
                {
                    ret = paComplete;
                    break;
                }
            }
            
            //If no APU, passthrough
            if (audioProcessingUnit == NULL)
            {
                passthroughUnit->processAudio(in, out, numInputChannels, numOutputChannels);
            }
            
            else
            {
                audioProcessingUnit->processAudio(in, out, numInputChannels, numOutputChannels);
            }
            
        }
        
        return ret;
    }
    
    static int paCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData)
    {
        AudioDSPKernel *kernel = (AudioDSPKernel *)userData;
        return kernel->paCallbackMethod(inputBuffer,
                                        outputBuffer,
                                        framesPerBuffer,
                                        timeInfo,
                                        statusFlags);
    }
    
    static void paStreamFinished(void *userData)
    {
        return ((AudioDSPKernel *)userData)->paStreamFinishedMethod();
    }
    
    int numOutputChannels;
    int numInputChannels;
    unsigned long sampleRate;
    PaStream *stream;
    bool useFileInput;
    AudioFile *audioFile;
    AudioProcessingUnit *audioProcessingUnit;
    AudioManagerStatus status;
    StreamFinishedCallback streamFinishedCallback;
    void *streamFinishedCallbackCtx;
    
private:
    AudioProcessingUnit *passthroughUnit;
};


#endif /* AudioDSPKernel_hpp */
