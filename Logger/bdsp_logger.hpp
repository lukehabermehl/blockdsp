//
//  BDLogger.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BDLogger_hpp
#define BDLogger_hpp

#include <mutex>
#include <thread>
#include <string>
#include <vector>

class BDLogger
{
public:
    BDLogger();
    ~BDLogger();
    
    static BDLogger *sharedLogger();
    
    void log(const char *prefix, const char *s, FILE *f);
    void setOutputFile(const char *filepath);
    
    class pimpl;
    
private:
    pimpl *_pimpl;
};

void BDLog(const char *prefix, const char *s);
void BDLogFormat(const char *prefix, const char *format, ...);
void BDLogError(const char *prefix, const char *format, ...);

#endif /* BDLogger_hpp */
