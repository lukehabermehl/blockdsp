//
//  AudioDSPKernel.hpp
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef AudioDSPKernel_hpp
#define AudioDSPKernel_hpp

#include <portaudio.h>
#include "libblockdsp.h"
#include "AudioFile.hpp"

class AudioDSPKernel {
public:
    AudioDSPKernel()
    {
        dspCallbackContext = 0;
        numInputChannels = 1;
        numOutputChannels = 1;
        stream = 0;
        useFileInput = false;
        audioFile = 0;
        status = AudioManagerStatusDone;
        
        dspCallback = nullptr;
    }
    
    ~AudioDSPKernel()
    {
        if (stream)
            close();
    }
    
    bool open(PaDeviceIndex outputDevIndex)
    {
        PaStreamParameters outputParameters;
        outputParameters.device = outputDevIndex;
        if (outputParameters.device == paNoDevice)
        {
            return false;
        }
        
        outputParameters.channelCount = numOutputChannels;
        outputParameters.sampleFormat = paFloat32;
        outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
        
        outputParameters.hostApiSpecificStreamInfo = NULL;
        
        PaError err = Pa_OpenStream(&stream,
                                    NULL,
                                    &outputParameters,
                                    sampleRate,
                                    paFramesPerBufferUnspecified,
                                    0,
                                    &AudioDSPKernel::paCallback,
                                    this);
        
        if (err != paNoError)
        {
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
        return (err == paNoError);
    }
    
    void paStreamFinishedMethod()
    {
        status = AudioManagerStatusDone;
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
                if (audioFile->nextFrame(&in))
                {
                    ret = paComplete;
                    break;
                }
            }
            
            dspCallback(in, out, numInputChannels, numOutputChannels, dspCallbackContext);
        }
        
        return paContinue;
    }
    
    static int paCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData)
    {
        return ((AudioDSPKernel *)userData)->paCallbackMethod(inputBuffer,
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
    AudioProcessFunc dspCallback;
    void *dspCallbackContext;
    AudioManagerStatus status;
};


#endif /* AudioDSPKernel_hpp */
