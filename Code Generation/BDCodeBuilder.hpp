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
#include "audio_constants.h"

#include "BlockDSPSystem.hpp"

typedef NS_ENUM(unsigned int, BDBlockType)
{
    BDBlockTypeSummer,
    BDBlockTypeMultiplier,
    BDBlockTypeInput
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
    void addCoefficient(const char *name, const char *callback, const char *target, BlockDSPParameterType type);
    bool addNumber(const char *name);
    bool setNumberDefaultValue(const char *numberName, BlockDSPParameterType valueType, void *value);
    void connect(const char *from, const char *to);
    
    void addCallbackCode(const char *callbackName, const char *code);
    
    bool hasNode(const char *name);
    bool hasCallback(const char *name);
    bool hasNumber(const char *name);
    
private:
    char *_name;
    char *_dirpath;
    FILE *_openFile;
    std::unordered_map<std::string, std::string> callbackMap;
    std::unordered_map<std::string, bool> nodeSet;
    std::unordered_map<std::string, bool> numSet;
};

#endif /* BDCodeBuilder_hpp */
