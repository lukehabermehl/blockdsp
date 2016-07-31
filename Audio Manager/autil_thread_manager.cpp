//
//  autil_thread_manager.cpp
//  libblockdsp
//
//  Created by Luke on 7/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_thread_manager.hpp"

void AUtilThreadManager::workerFunction(void *ctx)
{
    AUtilThreadManager *threadManager = (AUtilThreadManager *)ctx;
    do {
        threadManager->mtx_.lock();
        if (!threadManager->threads_.empty()) {
            std::thread *thread = threadManager->threads_.front();
            threadManager->threads_.pop_front();
            threadManager->mtx_.unlock();
            
            if (thread->joinable())
                thread->join();
            
            delete thread;
        }
        threadManager->mtx_.lock();
        if (threadManager->threads_.empty()) {
            threadManager->isRunning_ = false;
        } else {
            threadManager->mtx_.unlock();
        }
    } while (threadManager->isRunning_);
    
    threadManager->mtx_.unlock();
}

AUtilThreadManager::AUtilThreadManager()
: worker_(NULL)
, isRunning_(false)
{
}

AUtilThreadManager::~AUtilThreadManager()
{
    if (worker_ && isRunning_ && worker_->joinable()) {
        worker_->join();
        delete worker_;
    }
}

void AUtilThreadManager::dispatch(dispatchable_fn func, void *ctx)
{
    mtx_.lock();
    threads_.push_back(new std::thread(func, ctx));
    if (!isRunning_) {
        if (worker_) {
            if (worker_->joinable()) {
                worker_->join();
            }
            delete worker_;
        }
        isRunning_ = true;
        worker_ = new std::thread(workerFunction, this);
    }
    mtx_.unlock();
}

AUtilThreadManager * AUtilThreadManager::sharedThreadManager()
{
    static AUtilThreadManager singleton;
    
    return &singleton;
}

void AUtilDispatchThread(dispatchable_fn func, void *ctx)
{
    AUtilThreadManager::sharedThreadManager()->dispatch(func, ctx);
}
