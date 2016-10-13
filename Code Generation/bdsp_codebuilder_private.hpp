//
//  bdsp_codebuilder_private.hpp
//  libblockdsp
//

#ifndef BDCodeBuilder_Private_h
#define BDCodeBuilder_Private_h

#include "bdsp_codebuilder.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class BDCodeBuilder::pimpl
{
public:
    void makeClassName();
    char *name;
    char *dirpath;
    char *className;
    FILE *openFile;
    BDCodeBuilderError error;
    std::unordered_map<std::string, std::string> callbackMap;
    std::unordered_map<std::string, bool> nodeSet;
    std::unordered_map<std::string, bool> numSet;
    std::unordered_map<std::string, bool> delayLineSet;
};

#endif /* BDCodeBuilder_Private_h */
