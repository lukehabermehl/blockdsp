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

class BDLogger::pimpl
{
public:
    std::vector<std::string> queue;
    
    std::mutex queueLock;
    std::thread workerThread;
    
    bool shutdown;
};


#endif /* BDLogger_Private_h */
