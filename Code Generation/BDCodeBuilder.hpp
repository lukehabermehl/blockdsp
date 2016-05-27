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
    
    void addCallbackCode(std::string callbackName, std::string code);
    
private:
    char *_name;
    char *_dirpath;
    FILE *_openFile;
    std::unordered_map<std::string, std::string> callbackMap;
};

#endif /* BDCodeBuilder_hpp */
