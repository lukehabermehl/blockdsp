//
//  BDCodeBuilder.hpp
//  libblockdsp
//
//  Created by Luke on 5/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BDCodeBuilder_hpp
#define BDCodeBuilder_hpp

#include <unordered_map>
#include <string>

#include "BlockDSPSystem.hpp"

enum BDBlockType : unsigned int
{
    BDBlockTypeSummer,
    BDBlockTypeMultiplier,
    BDBlockTypeInput
};

enum BDCodeBuilderError : unsigned int
{
    BDCodeBuilderErrorNoError = 0,
    BDCodeBuilderErrorBadPath,
    BDCodeBuilderErrorNonUnique,
    BDCodeBuilderErrorNotFound,
    BDCodeBuilderErrorFileNotOpen
};

class BDCodeBuilder
{
public:
    BDCodeBuilder(const char *name, const char *dirpath);
    ~BDCodeBuilder();
    
    void writeHeaderFile();
    void closeSourceFile();
    void openSourceFile();
    
    void addBlockNode(const char *name, BDBlockType type);
    void addDelayLine(const char *name, const char *inputNodeName, size_t size);
    void getDelayLineNode(const char *nodeName, const char *delayLineName, size_t delayIndex);
    void addCoefficient(const char *name, const char *callback, const char *target, BlockDSPParameterType type);
    void addNumber(const char *name);
    void setNumberDefaultValue(const char *numberName, BlockDSPParameterType valueType, void *value);
    void connect(const char *from, const char *to);
    
    void addCallbackCode(const char *callbackName, const char *code);
    
    bool hasNode(const char *name);
    bool hasCallback(const char *name);
    bool hasNumber(const char *name);
    bool hasDelayLine(const char *name);
    
    const char *name();
    const char *dirpath();
    
    BDCodeBuilderError error();
    
private:
    char *_name;
    char *_dirpath;
    FILE *_openFile;
    BDCodeBuilderError _error;
    std::unordered_map<std::string, std::string> callbackMap;
    std::unordered_map<std::string, bool> nodeSet;
    std::unordered_map<std::string, bool> numSet;
    std::unordered_map<std::string, bool> delayLineSet;
    
};

#endif /* BDCodeBuilder_hpp */
