//
//  bdsp_compiler.cpp
//  libblockdsp
//

#include "bdsp_compiler.hpp"
#include "bdsp_compiler_private.hpp"
#include <stdlib.h>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

static const char *default_libpath = "/usr/local/lib";
static const char *default_headerpath = "/usr/local/include";

BDCompiler::BDCompiler(BDCodeBuilder *builder)
{
    _pimpl = new pimpl;
    
    builder->writeHeaderFile();
    _pimpl->libpath     = std::string(default_libpath);
    _pimpl->headerpath  = std::string(default_headerpath);
    _pimpl->srcpath     = std::string(builder->dirpath());
    _pimpl->buildpath   = std::string(builder->dirpath());
    
    handler = 0;
    _pimpl->error = BDCompilerErrorNoError;
}

BDCompiler::~BDCompiler()
{
    delete _pimpl;
}

void BDCompiler::setBlockDSPLibPath(const char *libpath)
{
    _pimpl->libpath = std::string(libpath);
}

void BDCompiler::setBlockDSPHeaderPath(const char *headerpath)
{
    _pimpl->headerpath = std::string(headerpath);
}

void BDCompiler::setBuildPath(const char *buildpath)
{
    _pimpl->buildpath = buildpath ? std::string(buildpath) : std::string(_pimpl->srcpath);
}

bool BDCompiler::compileLibrary(const char *outputPath)
{
    struct stat st;
    
    const char *buildpathCstr = _pimpl->buildpath.c_str();
    
    if (stat(buildpathCstr, &st) == -1)
        mkdir(buildpathCstr, 0700);
    
    std::ostringstream oss;
    oss << "g++ -c -I" << _pimpl->headerpath << " " << _pimpl->srcpath << "/*.cpp";
    
    if (system(oss.str().c_str()))
    {
        _pimpl->error = BDCompilerErrorSourceCompileFailed;
        
        if (handler)
            handler->handleCompilerFinished(this);
        
        return false;;
    }
    
    oss.clear();
    oss.str("");
    oss << "mv *.o " << _pimpl->buildpath;
    if (system(oss.str().c_str()))
    {
        _pimpl->error = BDCompilerErrorMoveObjectsFailed;
        
        if (handler)
            handler->handleCompilerFinished(this);
    }
    
    oss.clear();
    oss.str("");
    
    oss << "g++ -dynamiclib -undefined suppress -flat_namespace -lblockdsp -L" << _pimpl->libpath << " " << _pimpl->buildpath << "/*.o -o " << outputPath;
    

    if (system(oss.str().c_str()))
    {
        _pimpl->error = BDCompilerErrorCompileLibFailed;
        
        if (handler)
            handler->handleCompilerFinished(this);
        
        return false;
    }
    
    if (handler)
        handler->handleCompilerFinished(this);
    
    return true;
}

BDCompilerError BDCompiler::error()
{
    BDCompilerError rv = _pimpl->error;
    _pimpl->error = BDCompilerErrorNoError;
    
    return rv;
}
