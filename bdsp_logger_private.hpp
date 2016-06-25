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
#include <vector>
#include <thread>
#include <mutex>

struct BDLoggerQueueItem;

class BDLogger::pimpl
{
public:
    std::vector<BDLoggerQueueItem *> queue;
    
    std::mutex queueLock;
    std::thread workerThread;
    FILE *outputFile;
    
    bool shutdown;
};


#endif /* BDLogger_Private_h */
