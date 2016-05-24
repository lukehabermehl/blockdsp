//
//  libblockdsp.h
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef libblockdsp_h
#define libblockdsp_h

#ifndef NULL
#define NULL 0
#endif

static const int kNoError = 0;
static const int kDoneReading = 1;
static const int kOutOfBounds = 2;

enum AudioManagerStatus : unsigned int
{
    AudioManagerStatusRunning,
    AudioManagerStatusDone
};

typedef void (*AudioProcessFunc)(float *inputBuffer,
                                float *outputBuffer,
                                int numInputChannels,
                                int numOutputChannels,
                                void *context);

#endif /* libblockdsp_h */
