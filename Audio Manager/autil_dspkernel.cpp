//
//  AudioDSPKernel.cpp
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_dspkernel.hpp"

AudioDSPKernel::AudioDSPKernel()
: audioProcessingUnit(NULL)
, numInputChannels(1)
, numOutputChannels(1)
, stream(NULL)
, useFileInput(false)
, audioFile(NULL)
, status(AudioManagerStatusDone)
, streamStatusChangeCallback(NULL)
, streamStatusChangeCallbackCtx(NULL)
{
    passthroughUnit = AudioProcessingUnit::createPassthroughUnit();
}

AudioDSPKernel::~AudioDSPKernel()
{
    if (stream)
        close();

    delete passthroughUnit;
}

bool AudioDSPKernel::open(PaDeviceIndex outputDevIndex)
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

    paError = Pa_OpenStream(&stream,
                                NULL,
                                &outputParameters,
                                sampleRate,
                                1,
                                0,
                                &AudioDSPKernel::paCallback,
                                this);

    if (paError != paNoError)
    {
        BDLogError(kAudioDSPKernelLogPrefix, "Failed to open stream! PAError code: %d", paError);
        return false;
    }

    paError = Pa_SetStreamFinishedCallback(stream, &AudioDSPKernel::paStreamFinished);
    if (paError != paNoError)
    {
        Pa_CloseStream(stream);
        stream = 0;
        return false;
    }

    return true;
}

bool AudioDSPKernel::close()
{
    if (stream == 0)
        return false;

    paError = Pa_CloseStream(stream);
    stream = 0;

    return (paError == paNoError);
}

bool AudioDSPKernel::start()
{
    if (stream == 0)
        return false;

    doStop = false;

    paError = Pa_StartStream(stream);
    status = AudioManagerStatusRunning;
    if (streamStatusChangeCallback) {
        streamStatusChangeCallback(streamStatusChangeCallbackCtx);
    }
    return (paError == paNoError);
}

bool AudioDSPKernel::stop()
{
    if (stream == 0)
        return false;

    doStop = true;
    return true;
}

void AudioDSPKernel::paStreamFinishedMethod()
{
    status = AudioManagerStatusDone;
    close();
    if (streamStatusChangeCallback) {
        streamStatusChangeCallback(streamStatusChangeCallbackCtx);
    }
}

int AudioDSPKernel::paCallbackMethod(const void *inputBuffer, void *outputBuffer,
                     unsigned long framesPerBuffer,
                     const PaStreamCallbackTimeInfo *timeInfo,
                     PaStreamCallbackFlags statusFlags)
{
    if (doStop) return paComplete;

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

void AudioDSPKernel::paStreamFinished(void *userData)
{
    return ((AudioDSPKernel *)userData)->paStreamFinishedMethod();
}




