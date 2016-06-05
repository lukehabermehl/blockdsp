//
//  BDCodeBuilder.cpp
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "BDCodeBuilder.hpp"
#include "BDCodeBuilder_Private.hpp"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BD_FILE_CHECK() if (!_pimpl->openFile) { _pimpl->error = BDCodeBuilderErrorFileNotOpen; return; }

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
            
        default:
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
    _pimpl = new pimpl;
    
    _pimpl->name = (char *)malloc(strlen(name) + 1);
    _pimpl->dirpath = (char *)malloc(strlen(dirpath) + 1);
    
    _pimpl->openFile = 0;
    _pimpl->error = BDCodeBuilderErrorNoError;
    
    strcpy(_pimpl->name, name);
    strcpy(_pimpl->dirpath, dirpath);
    
    struct stat st;
    if (stat(_pimpl->dirpath, &st) == -1)
        mkdir(_pimpl->dirpath, 0700);
    
}

BDCodeBuilder::~BDCodeBuilder()
{
    free(_pimpl->name);
    free(_pimpl->dirpath);
    
    delete _pimpl;
}

void BDCodeBuilder::addCallbackCode(const char *callbackName, const char *code)
{
    if (hasCallback(callbackName))
        return;
    
    _pimpl->callbackMap[std::string(callbackName)] = std::string(code);
}

bool BDCodeBuilder::hasNode(const char *name)
{
    auto it = _pimpl->nodeSet.find(std::string(name));
    return  !(it == _pimpl->nodeSet.end());
}

bool BDCodeBuilder::hasCallback(const char *name)
{
    auto it = _pimpl->callbackMap.find(std::string(name));
    return  !(it == _pimpl->callbackMap.end());
}

bool BDCodeBuilder::hasNumber(const char *name)
{
    auto it = _pimpl->numSet.find(std::string(name));
    return  !(it == _pimpl->numSet.end());
}

bool BDCodeBuilder::hasDelayLine(const char *name)
{
    auto it = _pimpl->delayLineSet.find(std::string(name));
    return  !(it == _pimpl->delayLineSet.end());
}

void BDCodeBuilder::writeHeaderFile()
{
    char filepath[1024];
    sprintf(filepath, "%s/%s.h", _pimpl->dirpath, _pimpl->name);
    
    FILE *f = fopen(filepath, "w");
    if (!f)
    {
        _pimpl->error = BDCodeBuilderErrorBadPath;
        return;
    }
    
    fprintf(f, "\n//This file was automatically generated.\n\n#ifndef BlockDSP_Factory_HPP\n#define BlockDSP_Factory_HPP\n");
    fprintf(f, "\n#include <BlockDSPSystem.hpp>\n#include <BlockDSPAPU.hpp>\n");
    fprintf(f, "\nextern \"C\" BlockDSPSystem * BlockDSPFactoryCreateSystem();\n");
    fprintf(f, "\nextern \"C\" AudioProcessingUnit *AudioProcessingUnitFactoryCreate();\n");
    fprintf(f, "\n#endif\n");
    
    fclose(f);
}

void BDCodeBuilder::openSourceFile()
{
    if (_pimpl->openFile)
    {
        fclose(_pimpl->openFile);
    }
    
    char filepath[1024];
    char headerFileName[256];
    sprintf(filepath, "%s/%s.cpp", _pimpl->dirpath, _pimpl->name);
    sprintf(headerFileName, "%s.h", _pimpl->name);
    
    FILE *f = fopen(filepath, "w");
    if (!f)
    {
        _pimpl->error = BDCodeBuilderErrorBadPath;
        return;
    }
    
    fprintf(f, "\n//This file was automatically generated.\n\n#include \"%s\"\n", headerFileName);
    
    for (auto it = _pimpl->callbackMap.begin(); it != _pimpl->callbackMap.end(); it++)
    {
        fprintf(f, "\nvoid %s(BlockDSPSystem *system, BlockDSPParameter *parameter, void *value) {", it->first.c_str());
        fprintf(f, "\n\t%s\n}\n", it->second.c_str());
    }
    
    _pimpl->nodeSet["MAIN_INPUT_NODE"] = true;
    _pimpl->nodeSet["MAIN_OUTPUT_NODE"] = true;
    
    fprintf(f, "\nBlockDSPSystem * BlockDSPFactoryCreateSystem() {\n");
    fprintf(f, "BlockDSPSystem *system = new BlockDSPSystem(2);\n");
    fprintf(f, "BlockDSPInputNode *MAIN_INPUT_NODE = system->mainInputNode;\n");
    fprintf(f, "BlockDSPMultiplierNode *MAIN_OUTPUT_NODE = system->createMultiplierNode();\n");
    fprintf(f, "system->mainOutputNode = MAIN_OUTPUT_NODE;\n");
    fprintf(f, "MAIN_OUTPUT_NODE->coefficient->setFloatValue(1.0);\n");
    
    _pimpl->openFile = f;
    
    BD_FILE_CHECK();
}

void BDCodeBuilder::closeSourceFile()
{
    BD_FILE_CHECK();
    
    fprintf(_pimpl->openFile, "\nreturn system;\n");
    fprintf(_pimpl->openFile, "\n}\n");
    fprintf(_pimpl->openFile, "\n\nAudioProcessingUnit *AudioProcessingUnitFactoryCreate() {\n");
    fprintf(_pimpl->openFile, "BlockDSPAPU *unit = new BlockDSPAPU();\nunit->system = BlockDSPFactoryCreateSystem();\nreturn unit;\n}\n");
    fclose(_pimpl->openFile);
    _pimpl->openFile = 0;
}

void BDCodeBuilder::addBlockNode(const char *name, BDBlockType type)
{
    BD_FILE_CHECK();
    
    std::string strName = std::string(name);
    if (hasNode(name))
    {
        _pimpl->error = BDCodeBuilderErrorNonUnique;
        return;
    }
    
    char typeStr[256];
    char factoryMethodName[256];
    
    _pimpl->nodeSet[strName] = true;
    
    BDInfoForBlockType(typeStr, factoryMethodName, type);
    
    fprintf(_pimpl->openFile, "%s *%s = system->%s();\n", typeStr, name, factoryMethodName);
    if (type == BDBlockTypeMultiplier)
    {
        //Use typeStr for space efficiency
        sprintf(typeStr, "%s->coefficient", name);
        _pimpl->numSet[typeStr] = true;
        fprintf(_pimpl->openFile, "%s = BlockDSPNumber::numberForFloat(0.0);\n", typeStr);
    }
}

void BDCodeBuilder::addDelayLine(const char *name, const char *inputNodeName, size_t size)
{
    BD_FILE_CHECK();
    
    if (!hasNode(inputNodeName))
    {
        _pimpl->error = BDCodeBuilderErrorNotFound;
        return;
    }
    
    
    _pimpl->delayLineSet[std::string(name)] = true;
    
    fprintf(_pimpl->openFile, "BlockDSPDelayLine *%s = system->createDelayLine(%s);\n", name, inputNodeName);
    fprintf(_pimpl->openFile, "%s->setSize(%lu);\n", name, size);
}

void BDCodeBuilder::getDelayLineNode(const char *nodeName, const char *delayLineName, size_t delayIndex)
{
    BD_FILE_CHECK();
    
    if (hasNode(nodeName))
    {
        _pimpl->error = BDCodeBuilderErrorNonUnique;
        return;
    }
    
    if (!hasDelayLine(delayLineName))
    {
        _pimpl->error = BDCodeBuilderErrorNotFound;
        return;
    }
    
    _pimpl->nodeSet[std::string(nodeName)] = true;
    fprintf(_pimpl->openFile, "BlockDSPDelayLineNode *%s = %s->nodeForDelayIndex(%lu);\n", nodeName, delayLineName, delayIndex);
}

void BDCodeBuilder::addCoefficient(const char *name, const char *callback, const char *target, BlockDSPParameterType type)
{
    BD_FILE_CHECK();
    
    const char *targetParam = target ?: "0";
    char typeParam[20];
    BDStringForParameterType(typeParam, type);
    if (callback)
    {
        if (!hasCallback(callback))
            return;
    }
    
    fprintf(_pimpl->openFile, "BlockDSPParameter *%s = system->createParameter(\"%s\", %s, %s);\n", name, name, typeParam, targetParam);
    if (callback)
        fprintf(_pimpl->openFile, "%s->callback = %s;\n", name, callback);
}

void BDCodeBuilder::addNumber(const char *name)
{
    BD_FILE_CHECK();
    
    if (hasNumber(name))
    {
        _pimpl->error = BDCodeBuilderErrorNonUnique;
        return;
    }
    
    std::string numName = std::string(name);
    
    _pimpl->numSet[numName] = true;
    
    fprintf(_pimpl->openFile, "BlockDSPNumber *%s = new BlockDSPNumber();\n", name);
    fprintf(_pimpl->openFile, "system->addNumber(\"%s\", %s);\n", name, name);
}

void BDCodeBuilder::setNumberDefaultValue(const char *numberName, BlockDSPParameterType valueType, void *value)
{
    BD_FILE_CHECK();
    
    std::string numName = std::string(numberName);
    if (!hasNumber(numberName))
    {
        _pimpl->error = BDCodeBuilderErrorNotFound;
        return;
    }
    
    switch (valueType)
    {
        case BlockDSPParameterTypeFloat:
        {
            fprintf(_pimpl->openFile, "%s->setFloatValue(%f);\n", numberName, *((float *)value));
            break;
        }
            
        case BlockDSPParameterTypeBoolean:
        {
            fprintf(_pimpl->openFile, "%s->setBoolValue(%s);\n", numberName, *((bool *)value) ? "true" : "false");
            break;
        }
            
        case BlockDSPParameterTypeInteger:
        {
            fprintf(_pimpl->openFile, "%s->setIntegerValue(%d);\n", numberName, *((int *)value));
            break;
        }
            
        case BlockDSPParameterTypeUnsignedInt:
        {
            fprintf(_pimpl->openFile, "%s->setIntegerValue(%ud);\n", numberName, *((unsigned int *)value));
            break;
        }
    }
}

void BDCodeBuilder::connect(const char *from, const char *to)
{
    BD_FILE_CHECK();
    
    if (!hasNode(from) || !hasNode(to))
    {
        _pimpl->error = BDCodeBuilderErrorNotFound;
        return;
    }
    
    fprintf(_pimpl->openFile, "%s->connectInput(%s);\n", to, from);
}

const char *BDCodeBuilder::name()
{
    return _pimpl->name;
}

const char *BDCodeBuilder::dirpath()
{
    return _pimpl->dirpath;
}

BDCodeBuilderError BDCodeBuilder::error()
{
    BDCodeBuilderError rv = _pimpl->error;
    _pimpl->error = BDCodeBuilderErrorNoError;
    
    return rv;
}

