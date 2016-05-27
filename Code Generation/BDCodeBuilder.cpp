//
//  BDCodeBuilder.cpp
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "BDCodeBuilder.hpp"
#include <stdio.h>
#include <string.h>

void BDInfoForBlockType(char *typeName, char *factoryMethodName, BDBlockType type)
{
    switch (type)
    {
        case BDBlockTypeInput:
            strcpy(typeName, "BlockDSPInputNode");
            strcpy(factoryMethodName, "createInputNode");
            break;
            
        case BDBlockTypeSummer:
            strcpy(typeName, "BlockDSPSummerNode");
            strcpy(factoryMethodName, "createSummerNode");
            break;
            
        case BDBlockTypeMultiplier:
            strcpy(typeName, "BlockDSPMultiplierNode");
            strcpy(factoryMethodName, "createMultiplierNode");
            break;
    }
}

BDCodeBuilder::BDCodeBuilder(const char *name, const char *dirpath)
{
    _name = (char *)malloc(strlen(name) + 1);
    _dirpath = (char *)malloc(strlen(dirpath) + 1);
    
    _openFile = 0;
    
    strcpy(_name, name);
    strcpy(_dirpath, dirpath);
}

BDCodeBuilder::~BDCodeBuilder()
{
    free(_name);
    free(_dirpath);
}

void BDCodeBuilder::addCallbackCode(std::string callbackName, std::string code)
{
    callbackMap[callbackName] = code;
}

void BDCodeBuilder::writeHeaderFile()
{
    char filepath[1024];
    sprintf(filepath, "%s/%s.h", _dirpath, _name);
    
    FILE *f = fopen(filepath, "w");
    
    fprintf(f, "\n//This file was automatically generated.\n\n#ifndef BlockDSP_Factory_HPP\n#define BlockDSP_Factory_HPP\n");
    fprintf(f, "\n#include <BlockDSPSystem.h>\n");
    fprintf(f, "\nBlockDSPSystem * BlockDSPFactoryCreateSystem();\n");
    fprintf(f, "\n#endif\n");
    
    fclose(f);
}

void BDCodeBuilder::openSourceFile()
{
    if (_openFile)
    {
        fclose(_openFile);
    }
    
    char filepath[1024];
    char headerFileName[256];
    sprintf(filepath, "%s/%s.h", _dirpath, _name);
    sprintf(filepath, "%s/%s.h", _dirpath, _name);
    
    FILE *f = fopen(filepath, "w");
    
    fprintf(f, "\n//This file was automatically generated.\n\n#include \"%s\"\n", headerFileName);
    fprintf(f, "\nBlockDSPSystem * BlockDSPFactoryCreateSystem() {\n");
    
    for (auto it = callbackMap.begin(); it != callbackMap.end(); it++)
    {
        fprintf(f, "\nvoid %s(BlockDSPSystem *system, BlockDSPParameter *parameter, void *value) {", it->first.c_str());
        fprintf(f, "\n\t%s\n}\n", it->second.c_str());
    }
    
    fprintf(f, "\nBlockDSPSystem * BlockDSPFactoryCreateSystem() {\n");
    fprintf(f, "BlockDSPSystem *system = new BlockDSPSystem(2);\n");
    
    _openFile = f;
}

void BDCodeBuilder::closeSourceFile()
{
    if (!_openFile)
        return;
    
    fprintf(_openFile, "\n}\n");
    fclose(_openFile);
    _openFile = 0;
}

void BDCodeBuilder::addBlockNode(const char *name, BDBlockType type)
{
    char typeStr[256];
    char factoryMethodName[256];
    
    BDInfoForBlockType(typeStr, factoryMethodName, type);
    
    fprintf(_openFile, "%s *%s = system->%s();\n", typeStr, name, factoryMethodName);
}

void BDCodeBuilder::addDelayLine(const char *name, const char *inputNodeName, size_t size)
{
    fprintf(_openFile, "BlockDSPDelayLine *%s = system->createDelayLine(%s);\n", name, inputNodeName);
}

