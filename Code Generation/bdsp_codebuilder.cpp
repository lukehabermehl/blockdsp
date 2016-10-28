//
//  bdsp_codebuilder.cpp
//  libblockdsp
//
//

#include "bdsp_codebuilder.hpp"
#include "bdsp_codebuilder_private.hpp"
#include "autil_number.hpp"
#include "bdsp_logger.hpp"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sstream>

#define BD_FILE_CHECK() if (!_pimpl->openFile) { _pimpl->error = BDCodeBuilderErrorFileNotOpen; return; }

static const char * kCodeBuilderPrefix = "[BDCodeBuilder]";

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

void BDStringForParameterType(char *str, APUNumberType type)
{
    switch (type)
    {
        case APUNUM_FLOAT:
            strcpy(str, "APUNUM_FLOAT");
            break;
        case APUNUM_BOOLEAN:
            strcpy(str, "APUNUM_BOOLEAN");
            break;
        case APUNUM_INTEGER:
            strcpy(str, "APUNUM_INTEGER");
            break;
        case APUNUM_UINT:
            strcpy(str, "APUNUM_UINT");
            break;
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
    _pimpl->makeClassName();

    struct stat st;
    if (stat(_pimpl->dirpath, &st) == -1)
        mkdir(_pimpl->dirpath, 0700);

}

BDCodeBuilder::~BDCodeBuilder()
{
    free(_pimpl->name);
    free(_pimpl->className);
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
    std::string strName = std::string(name);
    auto it = _pimpl->nodeSet.find(strName);
    if (it != _pimpl->nodeSet.end()) return true;

    auto it2 = _pimpl->delayLineSet.find(strName);
    return !(it2 == _pimpl->delayLineSet.end());
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

bool BDCodeBuilder::hasParameter(const char *name)
{
    auto it = _pimpl->parameterSet.find(name);
    return !(it == _pimpl->parameterSet.end());
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
    fprintf(f, "\n#include <blockdsp.h>");
    fprintf(f, "\nextern \"C\" AudioProcessingUnit *AudioProcessingUnitFactoryCreate();\n");
    fprintf(f, "\nclass %s : public BlockDSPAPU {", className());
    fprintf(f, "\npublic:");
    fprintf(f, "\n\t%s();", className());
    fprintf(f, "\n\tconst char * getName();");
    fprintf(f, "\nprotected:");
    fprintf(f, "\n\tvirtual void configureSystem();");
    fprintf(f, "\n};");
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
        BDLogFormat(kCodeBuilderPrefix, "Failed to open file at path: %s", filepath);
        return;
    }

    fprintf(f, "\n//This file was automatically generated.\n\n#include \"%s\"\n", headerFileName);

    for (auto it = _pimpl->callbackMap.begin(); it != _pimpl->callbackMap.end(); it++)
    {
        fprintf(f, "\nvoid %s(BlockDSPAPU *contextAPU, BlockDSPParameter *parameter, APUNumber value) {", it->first.c_str());
        fprintf(f, "\n\t%s\n}\n", it->second.c_str());
    }

    _pimpl->nodeSet["MAIN_INPUT_NODE"] = true;
    _pimpl->nodeSet["MAIN_OUTPUT_NODE"] = true;

    fprintf(f, "\n%s::%s() : BlockDSPAPU(new BlockDSPSystem()) {\n", className(), className());
    fprintf(f, "\tconfigureSystem();\n}\n");
    fprintf(f, "\nconst char * %s::getName() { return \"%s\"; }", className(), name());
    fprintf(f, "\nvoid %s::configureSystem() {\n", className());
    fprintf(f, "BlockDSPSystem *system = getSystem();\n");
    fprintf(f, "BlockDSPInputNode *MAIN_INPUT_NODE = system->mainInputNode;\n");
    fprintf(f, "BlockDSPMultiplierNode *MAIN_OUTPUT_NODE = system->createMultiplierNode();\n");
    fprintf(f, "system->mainOutputNode = MAIN_OUTPUT_NODE;\n");
    fprintf(f, "MAIN_OUTPUT_NODE->coefficient.setFloatValue(1.0);\n");

    _pimpl->openFile = f;

    BD_FILE_CHECK();
}

void BDCodeBuilder::closeSourceFile()
{
    BD_FILE_CHECK();

    fprintf(_pimpl->openFile, "\n}\n");
    fprintf(_pimpl->openFile, "\n\nAudioProcessingUnit *AudioProcessingUnitFactoryCreate() { return new %s(); }", className());
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
        BDLogFormat(kCodeBuilderPrefix, "Failed to add block node %s: name non-unique", name);
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
    }
}

void BDCodeBuilder::addDelayLine(const char *name, size_t size)
{
    BD_FILE_CHECK();
    
    _pimpl->delayLineSet[name] = true;

    fprintf(_pimpl->openFile, "BlockDSPDelayLine *%s = system->createDelayLine(NULL);\n", name);
    fprintf(_pimpl->openFile, "%s->setSize(%lu);\n", name, size);
    //Must connect input node after nodes are initialized!
}

void BDCodeBuilder::setDelayLineInput(const char *delayLineName, const char *inputNodeName)
{
    BD_FILE_CHECK();

    if (!hasDelayLine(delayLineName))
    {
        _pimpl->error = BDCodeBuilderErrorNotFound;
        BDLogFormat(kCodeBuilderPrefix, "Delay Line not found: %s", delayLineName);
        return;
    }

    if (!hasNode(inputNodeName))
    {
        _pimpl->error = BDCodeBuilderErrorNotFound;
        BDLogFormat(kCodeBuilderPrefix, "Node not found: %s", inputNodeName);
        return;
    }

    fprintf(_pimpl->openFile, "%s->inputNode = %s;\n", delayLineName, inputNodeName);
}

void BDCodeBuilder::getDelayLineNode(const char *nodeName, const char *delayLineName, size_t delayIndex)
{
    BD_FILE_CHECK();

    if (hasNode(nodeName))
    {
        _pimpl->error = BDCodeBuilderErrorNonUnique;
        BDLogFormat(kCodeBuilderPrefix, "Failed to add delay line tap: %s: name non-unique", nodeName);
        return;
    }

    if (!hasDelayLine(delayLineName))
    {
        _pimpl->error = BDCodeBuilderErrorNotFound;
        BDLogFormat(kCodeBuilderPrefix, "Failed to find delay line %s", delayLineName);
        return;
    }

    _pimpl->nodeSet[std::string(nodeName)] = true;
    fprintf(_pimpl->openFile, "BlockDSPDelayLineNode *%s = %s->nodeForDelayIndex(%lu);\n", nodeName, delayLineName, delayIndex);
}

void BDCodeBuilder::addParameter(const char *varName, const char *name, APUNumber minValue, APUNumber maxValue, APUNumber defaultValue, const char *callback, APUNumberType type)
{
    BD_FILE_CHECK();

    char typeParam[126];
    BDStringForParameterType(typeParam, type);
    if (callback)
    {
        if (!hasCallback(callback))
            return;
    }

    fprintf(_pimpl->openFile, "BlockDSPParameter *%s = createParameter(\"%s\", %s, APUNUM_FLOAT(%f), APUNUM_FLOAT(%f), APUNUM_FLOAT(%f));\n", varName, name, typeParam, minValue.floatValue(), maxValue.floatValue(), defaultValue.floatValue());

    if (callback)
        fprintf(_pimpl->openFile, "%s->callback = %s;\n", varName, callback);

    _pimpl->parameterSet[varName] = true;
}

void BDCodeBuilder::connectParameterToMultiplierNode(const char *paramVarName, const char *multiplierNodeName)
{
    if (!hasNode(multiplierNodeName))
    {
        _pimpl->error = BDCodeBuilderErrorNotFound;
        return;
    }
    fprintf(_pimpl->openFile, "%s->parameter = %s;\n", multiplierNodeName, paramVarName);
    fprintf(_pimpl->openFile, "%s->useParameter = true;\n", multiplierNodeName);
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

    fprintf(_pimpl->openFile, "APUNumber %s;\n", name);
    fprintf(_pimpl->openFile, "system->addNumber(\"%s\", %s);\n", name, name);
}

void BDCodeBuilder::setNumberDefaultValue(const char *numberName, APUNumber value, APUNumberType valueType)
{
    BD_FILE_CHECK();

    std::string numName = std::string(numberName);
    if (!hasNumber(numberName))
    {
        _pimpl->error = BDCodeBuilderErrorNotFound;
        BDLogFormat(kCodeBuilderPrefix, "Number not found: %s", numberName);
        return;
    }

    switch (valueType)
    {
        case APUNumberType::APUNUM_FLOAT:
        {
            fprintf(_pimpl->openFile, "%s.setFloatValue(%f);\n", numberName, value.floatValue());
            break;
        }

        case APUNumberType::APUNUM_BOOLEAN:
        {
            fprintf(_pimpl->openFile, "%s.setBoolValue(%s);\n", numberName, value.boolValue() ? "true" : "false");
            break;
        }

        case APUNumberType::APUNUM_INTEGER:
        {
            fprintf(_pimpl->openFile, "%s.setIntegerValue(%d);\n", numberName, value.integerValue());
            break;
        }

        case APUNumberType::APUNUM_UINT:
        {
            fprintf(_pimpl->openFile, "%s.setUnsignedIntValue(%u);\n", numberName, value.unsignedIntValue());
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

const char * BDCodeBuilder::className()
{
    return _pimpl->className;
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

void BDCodeBuilder::pimpl::makeClassName()
{
    size_t len = strlen(name) + 9;
    className = (char *)malloc(len); //allocate new string with length of name + null terminator + "BDSP_APU_"
    memset(className, '\0', len);
    sprintf(className, "%s", "BDSP_APU_");
    sprintf(&className[9], "%s", name);
    for (size_t i=0; i<len; i++) {
        if (!isalnum(className[i]) && className[i] != '_') {
            className[i] = '_';
        }
    }
}

