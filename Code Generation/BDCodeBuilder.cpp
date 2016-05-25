//
//  BDCodeBuilder.cpp
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "BDCodeBuilder.hpp"
#include <stdio.h>

void BDCodeBuilder::addCallbackCode(std::string callbackName, std::string code)
{
    callbackMap[callbackName] = code;
}

void BDCodeBuilder::writeHeaderFile(const char *filepath)
{
    FILE *f = fopen(filepath, "w");
    
    fprintf(f, "\n//This file was automatically generated.\n\n#ifndef BlockDSP_Factory_HPP\n#define BlockDSP_Factory_HPP\n");
    fprintf(f, "\n#include <BlockDSPSystem.h>\n");
    fprintf(f, "\nBlockDSPSystem * BlockDSPFactoryCreateSystem();\n");
    fprintf(f, "\n#endif\n");
    
    fclose(f);
}

void BDCodeBuilder::writeSourceFile(const char *filepath, const char *headerfilename)
{
    FILE *f = fopen(filepath, "w");
    
    fprintf(f, "\n//This file was automatically generated.\n\n#include \"%s\"\n", headerfilename);
    fprintf(f, "\nBlockDSPSystem * BlockDSPFactoryCreateSystem() {\n");
    
    for (auto it = callbackMap.begin(); it != callbackMap.end(); it++)
    {
        fprintf(f, "\nvoid %s(BlockDSPSystem *system, BlockDSPParameter *parameter, void *value) {", it->first.c_str());
        fprintf(f, "\n\t%s\n}\n", it->second.c_str());
    }
    
    
    
    
    //TODO build code here
    
    fprintf(f, "\n}\n\n");
    
    fclose(f);
}