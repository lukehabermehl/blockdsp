//
//  bdsp_compiler_private.hpp
//  libblockdsp
//

#ifndef BDCompiler_Private_h
#define BDCompiler_Private_h

#include "bdsp_compiler.hpp"
#include <string>

class BDCompiler::pimpl
{
public:
    std::string srcpath;
    std::string libpath;
    std::string headerpath;
    std::string buildpath;
    
    BDCompilerError error;
};


#endif /* BDCompiler_Private_h */
