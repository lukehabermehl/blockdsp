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

#include "BlockDSPSystem.hpp"

class BDCodeBuilder
{
public:
    void writeHeaderFile(const char *filepath);
    void writeSourceFile(const char *filepath, const char *headerfilename);
    
    void addCallbackCode(std::string callbackName, std::string code);
    
private:
    std::unordered_map<std::string, std::string> callbackMap;
};

#endif /* BDCodeBuilder_hpp */
