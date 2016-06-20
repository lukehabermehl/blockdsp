//
//  BDCompiler.hpp
//  libblockdsp
//
//  Created by Luke on 5/28/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BDCompiler_hpp
#define BDCompiler_hpp

#include "bdsp_codebuilder.hpp"

class BDCompiler;

typedef void (* BDCompilerHandler)(bool, BDCompiler *);

enum BDCompilerError
{
    BDCompilerErrorNoError = 0,
    BDCompilerErrorSourceCompileFailed,
    BDCompilerErrorMoveObjectsFailed,
    BDCompilerErrorCompileLibFailed
};

class BDCompiler
{
public:
    BDCompiler(BDCodeBuilder *builder);
    ~BDCompiler();
    
    void setBlockDSPLibPath(const char *libpath);
    void setBlockDSPHeaderPath(const char *headerpath);
    void setBuildPath(const char *buildpath);
    
    BDCompilerError error();
    
    bool compileLibrary(const char *outputPath);
    
    BDCompilerHandler handler;
    
    
private:
    class pimpl;
    pimpl *_pimpl;
};

#endif /* BDCompiler_hpp */
