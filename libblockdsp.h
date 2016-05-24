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

#ifndef NS_ENUM
#define NS_ENUM(_type, _name) enum _name : _type _name; enum _name : _type
#endif

typedef NS_ENUM(unsigned int, AudioFileBufferStatus)
{
    AudioFileBufferStatusOK = 0,
    AudioFileBufferStatusDoneReading,
    AudioFileBufferStatusOutOfBounds
};

typedef NS_ENUM(unsigned int, AudioManagerStatus)
{
    AudioManagerStatusRunning,
    AudioManagerStatusDone
};

typedef NS_ENUM(unsigned int, AudioInputMode)
{
    AudioInputModeFile,
    AudioInputModeMicrophone
};

typedef NS_ENUM(unsigned int, AudioFileMode)
{
    AudioFileModeReadOnly,
    AudioFileModeWriteOnly,
    AudioFileModeReadWrite
};


typedef void (*AudioProcessFunc)(float *inputBuffer,
                                float *outputBuffer,
                                int numInputChannels,
                                int numOutputChannels,
                                void *context);

#endif /* libblockdsp_h */
