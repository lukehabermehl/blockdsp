//
//  AudioFile.hpp
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef AudioFile_hpp
#define AudioFile_hpp

#include <sndfile.h>
#include "libblockdsp.h"

class AudioFile
{
public:
    AudioFile(const char *filepath, AudioFileMode mode);
    ~AudioFile();
    
    /** The sample rate of the file */
    unsigned long sampleRate();
    
    /** The number of frames the file contains */
    unsigned long numFrames();
    
    /** The number of audio channels in the file */
    int numChannels();
    
    /** The total number of samples in the file */
    size_t totalSize();
    
    /** Set frame to point to the next audio frame
     @returns The status of the buffer after reading the frame
     */
    AudioFileBufferStatus nextFrame(float **frame);
    
    /** Close the file */
    void close();
    
    void setLooping(bool looping);
    bool isLooping();
    
private:
    SNDFILE *sndfile;
    SF_INFO sfInfo;
    float *buf;
    size_t readIndex;
    size_t _totalSize;
    bool _looping;
};

#endif /* AudioFile_hpp */
