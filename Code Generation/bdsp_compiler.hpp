//
//  bdsp_compiler.hpp
//  libblockdsp
//

/** @file bdsp_compiler.hpp */

#ifndef BDCompiler_hpp
#define BDCompiler_hpp

#include "bdsp_codebuilder.hpp"

class BDCompiler;

//Interface for a callback object to handle BDCompiler job completion
class BDCompilerHandler
{
public:
    /** Called when the compiler either fails or completes its job successfully
      * @param compiler a pointer to the sender instance
      */
    virtual void handleCompilerFinished(BDCompiler *compiler) = 0;
};

/** Error codes for compilation */
enum BDCompilerError
{
    BDCompilerErrorNoError = 0,
    BDCompilerErrorSourceCompileFailed,
    BDCompilerErrorMoveObjectsFailed,
    BDCompilerErrorCompileLibFailed
};

/** Compile the output of a BDCodeBuilder */
class BDCompiler
{
public:
    /** Constructor
      * @param builder a pointer to the BDCodeBuilder instanced used to generate the code */
    BDCompiler(BDCodeBuilder *builder);
    ~BDCompiler();
    
    /** Set the library search path to link against BlockDSP */
    void setBlockDSPLibPath(const char *libpath);
    /** Set the header search path for the BlockDSP header file **blockdsp.h** */
    void setBlockDSPHeaderPath(const char *headerpath);
    /** Set the output path for the build intermediates */
    void setBuildPath(const char *buildpath);
    
    /** Get the last error that occured, if one exists */
    BDCompilerError error();
    
    /** Run the compilation job 
      * @param outputPath the path at which to write the .dylib
      */
    bool compileLibrary(const char *outputPath);
    
    /** Public member to hold the handler pointer */
    BDCompilerHandler *handler;
    
    
private:
    class pimpl;
    pimpl *_pimpl;
};

#endif /* BDCompiler_hpp */
