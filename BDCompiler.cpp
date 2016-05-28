//
//  BDCompiler.cpp
//  libblockdsp
//
//  Created by Luke on 5/28/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "BDCompiler.hpp"
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

static const char *default_libpath = "/usr/local/lib";
static const char *default_headerpath = "/usr/local/include";

BDCompiler::BDCompiler(BDCodeBuilder *builder)
{
    builder->writeHeaderFile();
    _srcpath = (char *)malloc(strlen(builder->dirpath() + 1));
    _libpath = (char *)malloc(strlen(default_libpath + 1));
    _headerpath = (char *)malloc(strlen(default_headerpath  + 1));
    _buildpath = (char *)malloc(strlen(_srcpath) + 1);
    
    strcpy(_srcpath, builder->dirpath());
    strcpy(_libpath, default_libpath);
    strcpy(_headerpath, default_headerpath);
    strcpy(_buildpath, _srcpath);
    
    handler = 0;
}

BDCompiler::~BDCompiler()
{
    free(_srcpath);
    free(_libpath);
    free(_headerpath);
    free(_buildpath);
}

void BDCompiler::setBlockDSPLibPath(const char *libpath)
{
    free(_libpath);
    const char *str;
    if (libpath == 0)
        str = default_libpath;
    else
        str = libpath;
    
    _libpath = (char *)malloc(strlen(str) + 1);
    strcpy(_libpath, str);
}

void BDCompiler::setBlockDSPHeaderPath(const char *headerpath)
{
    free(_headerpath);
    const char *str;
    if (headerpath == 0)
        str = default_headerpath;
    else
        str = headerpath;
    
    _headerpath = (char *)malloc(strlen(str) + 1);
    strcpy(_headerpath, str);
}

void BDCompiler::setBuildPath(const char *buildpath)
{
    free(_buildpath);
    const char *str;
    if (buildpath == 0)
        str = _srcpath;
    else
        str = buildpath;
    
    _buildpath = (char *)malloc(strlen(str) + 1);
    strcpy(_buildpath, str);
}

bool BDCompiler::compileLibrary(const char *outputPath)
{
    struct stat st;
    if (stat(_buildpath, &st) == -1)
        mkdir(_buildpath, 0700);
    
    char cmd[4096];
    sprintf(cmd, "g++ -c -I%s %s/*.cpp", _headerpath, _srcpath);
    
    if (system(cmd))
    {
        if (handler)
            handler(false, outputPath);
        
        return false;;
    }
    
    sprintf(cmd, "mv *.o %s", _buildpath);
    system(cmd);
    
    sprintf(cmd, "g++ -dynamiclib -undefined suppress -flat_namespace -lblockdsp -L%s %s/*.o -o %s", _libpath, _buildpath, outputPath);
    if (system(cmd))
    {
        if (handler)
            handler(false, outputPath);
        
        return false;
    }
    
    if (handler)
        handler(true, outputPath);
    
    return true;
}
