//
//  AudioFile_Private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef AudioFile_Private_h
#define AudioFile_Private_h

#include "autil_file.hpp"
#include <sndfile.h>
#include <thread>
#include <mutex>

struct ThreadQueueLink {
    std::thread *thread;
    ThreadQueueLink *next;
    
    ThreadQueueLink(std::thread *thr) : thread(thr), next(NULL) {}
};

struct ThreadQueue {
    std::mutex lock;
    ThreadQueueLink *first;
    ThreadQueueLink *last;
    bool stop;
    
    ThreadQueue() : first(NULL), last(NULL), stop(false) {}
    
    void append(ThreadQueueLink *queueLink)
    {
        lock.lock();
        if (first == NULL)
        {
            first = queueLink;
            last = queueLink;
        }
        else
        {
            last->next = queueLink;
            last = last->next;
        }
        lock.unlock();
    }
};

class AudioFile::pimpl
{
public:
    ~pimpl();
    
    SNDFILE *sndfile;
    SF_INFO sfInfo;
    AudioFileMode mode;
    float *bufs[2];
    int currentBufIndex;
    size_t readIndex;
    size_t samplesRead;
    size_t framesBuffered;
    size_t totalSize;
    size_t bufferSize;
    bool looping;
    bool needsBuffer;
    bool isBuffering;
    
    std::thread *threadManagerWorker;
    ThreadQueue threadQueue;
};

#endif /* AudioFile_Private_h */
