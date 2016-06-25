//
//  BDLogger.cpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "bdsp_logger.hpp"
#include "bdsp_logger_private.hpp"

#include <thread>
#include <mutex>
#include <unistd.h>
#include <sstream>

static std::once_flag onceFlag;
static const uint32_t nanosec_sleep = 1000;

struct BDLoggerQueueItem {
    std::string str;
    FILE *file;
};

BDLoggerQueueItem * BDLoggerQueueItemCreate(std::string s, FILE *file=stdout)
{
    BDLoggerQueueItem *qItem = new BDLoggerQueueItem;
    qItem->str = s;
    qItem->file = file;
    
    return qItem;
}

void logger_append(BDLogger::pimpl *logger, BDLoggerQueueItem *qItem)
{
    logger->queueLock.lock();
    logger->queue.push_back(qItem);
    logger->queueLock.unlock();
}

void logger_worker(BDLogger::pimpl *logger)
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
            fprintf(logger->queue[i]->file, "%s\n", logger->queue[i]->str.c_str());
            if (logger->outputFile)
            {
                fprintf(logger->outputFile, "%s\n", logger->queue[i]->str.c_str());
            }
        }
        
        logger->queue.clear();
        logger->queueLock.unlock();
    }
}

BDLogger::BDLogger()
{
    _pimpl = new pimpl;
    _pimpl->shutdown = false;
    _pimpl->outputFile = NULL;
    _pimpl->workerThread = std::thread(logger_worker, _pimpl);
}

BDLogger::~BDLogger()
{
    _pimpl->shutdown = true;
    _pimpl->workerThread.join();
    if (_pimpl->outputFile)
        fclose(_pimpl->outputFile);
    
    delete _pimpl;
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

void BDLogger::log(const char *prefix, const char *s, FILE *f)
{
    std::ostringstream oss;
    oss.clear();
    oss.str("");
    
    oss << prefix << ": " << s;
    
    logger_append(_pimpl, BDLoggerQueueItemCreate(oss.str(), f));
}

void BDLogger::setOutputFile(const char *filepath)
{
    _pimpl->outputFile = fopen(filepath, "a");
}

void BDLog(const char *prefix, const char *s)
{
    BDLogger::sharedLogger()->log(prefix, s, stdout);
}

void BDLogFormat(const char *prefix, const char *format, ...)
{
    char logstr[4096];
    va_list args;
    va_start(args, format);
    vsprintf(logstr, format, args);
    
    BDLogger::sharedLogger()->log(prefix, logstr, stdout);
    va_end(args);
}

void BDLogError(const char *prefix, const char *format, ...)
{
    char logstr[4096];
    va_list args;
    va_start(args, format);
    vsprintf(logstr, format, args);
    
    BDLogger::sharedLogger()->log(prefix, logstr, stderr);
    va_end(args);
}

