//
//  BDCompiler.cpp
//  libblockdsp
//
//  Created by Luke on 5/28/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "BDCompiler.hpp"
#include <stdlib.h>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

static const char *default_libpath = "/usr/local/lib";
static const char *default_headerpath = "/usr/local/include";

BDCompiler::BDCompiler(BDCodeBuilder *builder)
{
    builder->writeHeaderFile();
    _libpath = std::string(default_libpath);
    _headerpath = std::string(default_headerpath);
    _srcpath = std::string(builder->dirpath());
    _buildpath = std::string(builder->dirpath());
    
    handler = 0;
    _error = BDCompilerErrorNoError;
}

BDCompiler::~BDCompiler()
{
}

void BDCompiler::setBlockDSPLibPath(const char *libpath)
{
    _libpath = std::string(libpath);
}

void BDCompiler::setBlockDSPHeaderPath(const char *headerpath)
{
    _headerpath = std::string(headerpath);
}

void BDCompiler::setBuildPath(const char *buildpath)
{
    _buildpath = buildpath ? std::string(buildpath) : std::string(_srcpath);
}

bool BDCompiler::compileLibrary(const char *outputPath)
{
    struct stat st;
    
    const char *buildpathCstr = _buildpath.c_str();
    
    if (stat(buildpathCstr, &st) == -1)
        mkdir(buildpathCstr, 0700);
    
    std::ostringstream oss;
    oss << "g++ -c -I" << _headerpath << " " << _srcpath << "/*.cpp";
    
    if (system(oss.str().c_str()))
    {
        _error = BDCompilerErrorSourceCompileFailed;
        
        if (handler)
            handler(false, this);
        
        return false;;
    }
    
    oss.clear();
    oss.str("");
    oss << "mv *.o " << _buildpath;
    if (system(oss.str().c_str()))
    {
        _error = BDCompilerErrorMoveObjectsFailed;
        
        if (handler)
            handler(false, this);
    }
    
    oss.clear();
    oss.str("");
    
    oss << "g++ -dynamiclib -undefined suppress -flat_namespace -lblockdsp -L" << _libpath << " " << _buildpath << "/*.o -o " << outputPath;
    

    if (system(oss.str().c_str()))
    {
        _error = BDCompilerErrorCompileLibFailed;
        
        if (handler)
            handler(false, this);
        
        return false;
    }
    
    if (handler)
        handler(true, this);
    
    return true;
}

BDCompilerError BDCompiler::error()
{
    BDCompilerError rv = _error;
    _error = BDCompilerErrorNoError;
    
    return rv;
}
