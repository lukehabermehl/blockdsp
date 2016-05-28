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

void BDStringForParameterType(char *str, BlockDSPParameterType type)
{
    switch (type)
    {
        case BlockDSPParameterTypeFloat:
            strcpy(str, "BlockDSPParameterTypeFloat");
        case BlockDSPParameterTypeBoolean:
            strcpy(str, "BlockDSPParameterTypeBoolean");
        case BlockDSPParameterTypeInteger:
            strcpy(str, "BlockDSPParameterTypeInteger");
        case BlockDSPParameterTypeUnsignedInt:
            strcpy(str, "BlockDSPParameterTypeUnsignedInt");
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

void BDCodeBuilder::addCallbackCode(const char *callbackName, const char *code)
{
    if (hasCallback(callbackName))
        return;
    
    callbackMap[std::string(callbackName)] = std::string(code);
}

bool BDCodeBuilder::hasNode(const char *name)
{
    auto it = nodeSet.find(std::string(name));
    return  !(it == nodeSet.end());
}

bool BDCodeBuilder::hasCallback(const char *name)
{
    auto it = callbackMap.find(std::string(name));
    return  !(it == callbackMap.end());
}

bool BDCodeBuilder::hasNumber(const char *name)
{
    auto it = numSet.find(std::string(name));
    return  !(it == numSet.end());
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
    sprintf(filepath, "%s/%s.cpp", _dirpath, _name);
    sprintf(headerFileName, "%s.h", _name);
    
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
    std::string strName = std::string(name);
    if (hasNode(name))
        return;
    
    nodeSet[strName] = true;
    
    char typeStr[256];
    char factoryMethodName[256];
    
    BDInfoForBlockType(typeStr, factoryMethodName, type);
    
    fprintf(_openFile, "%s *%s = system->%s();\n", typeStr, name, factoryMethodName);
}

void BDCodeBuilder::addDelayLine(const char *name, const char *inputNodeName, size_t size)
{
    std::string nodeName = std::string(inputNodeName);
    auto it = nodeSet.find(nodeName);
    if (it == nodeSet.end())
        return;
    
    fprintf(_openFile, "BlockDSPDelayLine *%s = system->createDelayLine(%s);\n", name, inputNodeName);
    fprintf(_openFile, "%s->setSize(%lu);\n", name, size);
}

void BDCodeBuilder::addCoefficient(const char *name, const char *callback, const char *target, BlockDSPParameterType type)
{
    const char *targetParam = target ?: "0";
    char typeParam[20];
    BDStringForParameterType(typeParam, type);
    if (callback)
    {
        if (!hasCallback(callback))
            return;
    }
    
    fprintf(_openFile, "BlockDSPParameter *%s = system->createParameter(\"%s\", %s, %s);\n", name, name, typeParam, targetParam);
    if (callback)
        fprintf(_openFile, "%s->callback = %s;\n", name, callback);
}

bool BDCodeBuilder::addNumber(const char *name)
{
    if (hasNumber(name))
        return false;
    
    std::string numName = std::string(name);
    
    numSet[numName] = true;
    
    fprintf(_openFile, "BlockDSPNumber *%s = new BlockDSPNumber();\n", name);
    fprintf(_openFile, "system->addNumber(\"%s\", %s);\n", name, name);
    
    return true;
}

bool BDCodeBuilder::setNumberDefaultValue(const char *numberName, BlockDSPParameterType valueType, void *value)
{
    std::string numName = std::string(numberName);
    if (!hasNumber(numberName))
        return false;
    
    switch (valueType)
    {
        case BlockDSPParameterTypeFloat:
        {
            fprintf(_openFile, "%s->setFloatValue(%f);\n", numberName, *((float *)value));
            break;
        }
            
        case BlockDSPParameterTypeBoolean:
        {
            fprintf(_openFile, "%s->setBoolValue(%s);\n", numberName, *((bool *)value) ? "true" : "false");
            break;
        }
            
        case BlockDSPParameterTypeInteger:
        {
            fprintf(_openFile, "%s->setIntegerValue(%d);\n", numberName, *((int *)value));
            break;
        }
            
        case BlockDSPParameterTypeUnsignedInt:
        {
            fprintf(_openFile, "%s->setIntegerValue(%ud);\n", numberName, *((unsigned int *)value));
            break;
        }
    }
    
    return true;
}

void BDCodeBuilder::connect(const char *from, const char *to)
{
    std::string fromStr = std::string(from);
    std::string toStr = std::string(to);
    
    if (!hasNode(from) || !hasNode(to))
        return;
    
    fprintf(_openFile, "%s->connectInput(%s);\n", to, from);
}

