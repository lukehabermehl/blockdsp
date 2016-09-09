//
//  bdsp_codebuilder.hpp
//  libblockdsp
//

/** @file bdsp_codebuilder.hpp */

#ifndef BDCodeBuilder_hpp
#define BDCodeBuilder_hpp

#include "bdsp_system.hpp"

/** Represents the type of operation that a block performs */
enum BDBlockType
{
    BDBlockTypeSummer,
    BDBlockTypeMultiplier,
    BDBlockTypeInput,
    BDBlockTypeDelayLine,
    BDBlockTypeInvalid,
};

/** Error code for code generation/compiling */
enum BDCodeBuilderError
{
    BDCodeBuilderErrorNoError = 0,
    BDCodeBuilderErrorBadPath,
    BDCodeBuilderErrorNonUnique,
    BDCodeBuilderErrorNotFound,
    BDCodeBuilderErrorFileNotOpen
};

/** Used to generate the code for a BlockDSP APU */
class BDCodeBuilder
{
public:
    /** Constructor
      @param name - the name of the APU. This will be the base filename for the generated code files
      @param dirpath - output directory to write the code to
     */
    BDCodeBuilder(const char *name, const char *dirpath);
    ~BDCodeBuilder();
    
    /** Generate the header file and write it to the output directory */
    void writeHeaderFile();
    /** Close the source file. Call this when done with building. */
    void closeSourceFile();
    /** Open the source .cpp file for writing. Call this before using any of the building methods */
    void openSourceFile();
    
    /** Add a block node
      * @param name The name of the node. Think variable name.
      * @param type the operation type of the block
      */
    void addBlockNode(const char *name, BDBlockType type);
    /** Add a delay line
      * @param name name of the delay line
      * @param inputNodeName the name of the node at the input
      * @param size the number of samples of delay
      */
    void addDelayLine(const char *name, const char *inputNodeName, size_t size);
    /** Create a variable for the sample at the given delay index
      * @param nodeName the name to assign to the created variable
      * @param delayLineName the name used to create the delay line
      * @param delayIndex the sample index of the delay
      */
    void getDelayLineNode(const char *nodeName, const char *delayLineName, size_t delayIndex);
    /** Create a parameter
      * @param name variable name to assign
      * @param callback the name used to create a callback code block. NULL if none
      * @param type parameter value type
      */
    void addParameter(const char *name, const char *callback, APUNumberType type);
    /** Create a number
      * @param name var name to assign the number
      */
    void addNumber(const char *name);
    /** Assign a default value to a number variable
      * @param numberName the var name used to create the number
      * @param value an APUNumber containing the value and type of the default value
      * @param valueType Default type for the number
      */
    void setNumberDefaultValue(const char *numberName, APUNumber value, APUNumberType valueType);
    /** Connect the output of one node to the input of another 
      * @param from the var name used to create the output node
      * @param to the var name used to create the target node
     */
    void connect(const char *from, const char *to);
    /** Create a custom codeblock
      * @param callbackName function name of the callback block
      * @param code C++ code 
      */
    void addCallbackCode(const char *callbackName, const char *code);
    /** Determine if a node with the given name has been created */
    bool hasNode(const char *name);
    /** Determine if a code block with the given name has been created */
    bool hasCallback(const char *name);
    /** Determine if a number with the given name has been created */
    bool hasNumber(const char *name);
    /** Determine if a delay line with the given name has been created */
    bool hasDelayLine(const char *name);
    
    /** Get the name set in the constructor */
    const char *name();
    /** Get the output directory path set in the constructor */
    const char *dirpath();
    /** Get the generated BlockDSPAPU subclass name for the project. This name is based on the name set in the constructor */
    const char * className();
    
    /** Get the last error that occurred, if one exists */
    BDCodeBuilderError error();
    
private:
    class pimpl;
    pimpl *_pimpl;
};

#endif /* BDCodeBuilder_hpp */
