//
//  BDLogger_Private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BDLogger_Private_h
#define BDLogger_Private_h

#include "bdsp_logger.hpp"

#include <string>
#include <thread>
#include <mutex>

class BDLoggerQueue;

struct BDLoggerQueueItem {
    std::string str;
    FILE *file;
    BDLoggerQueueItem *next;
};

class BDLoggerQueue {
public:
    BDLoggerQueue() :
    first(NULL),
    last(NULL)
    {
    }
    ~BDLoggerQueue();
    BDLoggerQueueItem *first;
    BDLoggerQueueItem *last;
    
    void append(BDLoggerQueueItem *item);
    void popFront();
};

class BDLogger::pimpl
{
public:
    pimpl() :
    queue(new BDLoggerQueue),
    outputFile(NULL),
    shutdown(false)
    {
    }
    
    BDLoggerQueue *queue;
    
    std::mutex queueLock;
    std::thread workerThread;
    FILE *outputFile;
    
    bool shutdown;
};


#endif /* BDLogger_Private_h */
