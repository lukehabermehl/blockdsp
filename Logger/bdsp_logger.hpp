//
//  bdsp_logger.hpp
//  libblockdsp
//

/** @file bdsp_logger.hpp */

#ifndef BDLogger_hpp
#define BDLogger_hpp

#include <mutex>
#include <thread>
#include <string>
#include <vector>

/** A mostly non-blocking logger implementation. You should generally use the global logging functions instead
 of interfacing directly with this class */
class BDLogger
{
public:
    BDLogger();
    ~BDLogger();
    
    /** Get a pointer to the singleton instance */
    static BDLogger *sharedLogger();
    /** Log a string. Format is *[%prefix]: %s* */
    void log(const char *prefix, const char *s, FILE *f);
    /** Set the default output file for the logger */
    void setOutputFile(const char *filepath);
    
    class pimpl;
    
private:
    pimpl *_pimpl;
};

/** General log statement */
void BDLog(const char *prefix, const char *s);
/** Log with string formatting. Works like printf() */
void BDLogFormat(const char *prefix, const char *format, ...);
/** Log formatted string to stderr */
void BDLogError(const char *prefix, const char *format, ...);

#endif /* BDLogger_hpp */
