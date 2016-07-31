//
//  autil_thread_manager.hpp
//  libblockdsp
//
//  Created by Luke on 7/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

///@file autil_thread_manager.hpp

#ifndef autil_thread_manager_h
#define autil_thread_manager_h

#include <thread>
#include <deque>
#include <mutex>

/** typedef for void function pointer with a single void * argument */
typedef void (*dispatchable_fn)(void *);

/** Simplify memory managment for threads */
class AUtilThreadManager
{
public:
    AUtilThreadManager();
    ~AUtilThreadManager();
    
    /** Get the singleton instance */
    static AUtilThreadManager * sharedThreadManager();
    
    /** Create a new managed thread. 
      * @param func function to call in the thread
      * @param ctx context pointer to pass to the function
      */
    void dispatch(dispatchable_fn func, void *ctx);
    
private:
    std::thread *worker_;
    std::deque<std::thread *> threads_;
    std::mutex mtx_;
    bool isRunning_;
    
    static void workerFunction(void *ctx);
};

/** Add a new thread to the singleton thread manager
  * @param func function to call
  * @param ctx context pointer to pass
  */
void AUtilDispatchThread(dispatchable_fn func, void *ctx);

#endif /* autil_thread_manager_h */
