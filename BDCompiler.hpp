//
//  BDCompiler.hpp
//  libblockdsp
//
//  Created by Luke on 5/28/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BDCompiler_hpp
#define BDCompiler_hpp

#include "BDCodeBuilder.hpp"

typedef void (* BDCompilerHandler)(bool, const char *);

class BDCompiler
{
public:
    BDCompiler(BDCodeBuilder *builder);
    ~BDCompiler();
    
    void setBlockDSPLibPath(const char *libpath);
    void setBlockDSPHeaderPath(const char *headerpath);
    void setBuildPath(const char *buildpath);
    
    bool compileLibrary(const char *outputPath);
    
    BDCompilerHandler handler;
    
    
private:
    char *_srcpath;
    char *_libpath;
    char *_headerpath;
    char *_buildpath;
};

#endif /* BDCompiler_hpp */
