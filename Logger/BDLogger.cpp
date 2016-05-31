//
//  BDLogger.cpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "BDLogger.hpp"

#include <thread>
#include <mutex>
#include <unistd.h>
#include <sstream>

static std::once_flag onceFlag;
static const uint32_t nanosec_sleep = 1000;

void logger_append(BDLogger *logger, std::string s)
{
    logger->queueLock.lock();
    
    logger->queue.push_back(s);
    
    logger->queueLock.unlock();
}

void logger_worker(BDLogger *logger)
{
    while (!logger->shutdown)
    {
        if (logger->queue.empty())
        {
            struct timespec tm1, tm2;
            tm1.tv_nsec = nanosec_sleep;
            nanosleep(&tm1, &tm2);
            continue;
        }
        
        logger->queueLock.lock();
        
        for (size_t i=0; i<logger->queue.size(); i++)
        {
            printf("%s\n", logger->queue[i].c_str());
        }
        
        logger->queue.clear();
        logger->queueLock.unlock();
    }
}

BDLogger::BDLogger()
{
    shutdown = false;
    workerThread = std::thread(logger_worker, this);
}

BDLogger::~BDLogger()
{
    shutdown = true;
    workerThread.join();
}

BDLogger *BDLogger::sharedLogger()
{
    static BDLogger *_sharedLogger = 0;
    std::call_once(onceFlag, [](){
        if (!_sharedLogger)
            _sharedLogger = new BDLogger;
    });
    
    return _sharedLogger;
}

void BDLogger::log(const char *prefix, const char *s)
{
    std::ostringstream oss;
    oss.clear();
    oss.str("");
    
    oss << prefix << ": " << s;
    
    logger_append(this, oss.str());
}

void BDLog(const char *prefix, const char *s)
{
    BDLogger::sharedLogger()->log(prefix, s);
}

void BDLogFormat(const char *prefix, const char *format, ...)
{
    char logstr[4096];
    va_list args;
    va_start(args, format);
    vsprintf(logstr, format, args);
    
    BDLogger::sharedLogger()->log(prefix, logstr);
    va_end(args);
    
}

