//
//  libblockdsp
//
//  Created by Luke on 5/28/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file bdsp_compiler.hpp */

#ifndef BDCompiler_hpp
#define BDCompiler_hpp

#include "bdsp_codebuilder.hpp"

class BDCompiler;

/** typedef for handler function called when the compile job is finished. The function accepts a **bool** that is true if the job is successful, and a pointer to the BDCompiler instance used */
typedef void (* BDCompilerHandler)(bool, BDCompiler *);

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
    
    /** Public member to hold the handler function pointer */
    BDCompilerHandler handler;
    
    
private:
    class pimpl;
    pimpl *_pimpl;
};

#endif /* BDCompiler_hpp */
