//
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file autil_dspkernel.hpp 
  * Internal interface to PortAudio API
  */

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
    typedef void (*StreamStatusChangeCallback)(void *);
    AudioDSPKernel();
    ~AudioDSPKernel();
    
    bool open(PaDeviceIndex outputDevIndex);
    bool close();
    bool start();
    bool stop();
    
    void paStreamFinishedMethod();
    
    int paCallbackMethod(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags);

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
    
    static void paStreamFinished(void *userData);
    
    int numOutputChannels;
    int numInputChannels;
    unsigned long sampleRate;
    PaStream *stream;
    bool useFileInput;
    AudioFile *audioFile;
    AudioProcessingUnit *audioProcessingUnit;
    AudioManagerStatus status;
    StreamStatusChangeCallback streamStatusChangeCallback;
    PaError paError;
    void *streamStatusChangeCallbackCtx;
    
private:
    AudioProcessingUnit *passthroughUnit;
    bool doStop;
};


#endif /* AudioDSPKernel_hpp */
