//
//  BDCompiler_Private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
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
