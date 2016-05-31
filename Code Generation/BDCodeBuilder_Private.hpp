//
//  BDCodeBuilder_Private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef BDCodeBuilder_Private_h
#define BDCodeBuilder_Private_h

#include "BDCodeBuilder.hpp"
#include <string>
#include <unordered_map>

class BDCodeBuilder::pimpl
{
public:
    char *name;
    char *dirpath;
    FILE *openFile;
    BDCodeBuilderError error;
    std::unordered_map<std::string, std::string> callbackMap;
    std::unordered_map<std::string, bool> nodeSet;
    std::unordered_map<std::string, bool> numSet;
    std::unordered_map<std::string, bool> delayLineSet;
};

#endif /* BDCodeBuilder_Private_h */
