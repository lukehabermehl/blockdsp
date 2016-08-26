
/*
 A simple example program that builds and exports an APU that performs a second order LPF, loads it, and uses it to process an
 audio file for a few seconds
*/

#include <iostream>
#include "blockdsp/dsphelpers.hpp"
#include "blockdsp.h"

#define SAFE_DEL(_x) if (_x) delete _x

static const char * input_file_path = "./test/resources/guitar.wav";
static const char * dylib_path = "./test/resources/dsptest.dylib"; //Export APU dylib path
static const char * built_code_path = "./test/resources/dsptest_output"; //Destination for generated code

static BDAPULoader *apuLoader = 0;

//Load an APU from the .dylib at the specified path
AudioProcessingUnit * load_apu(const char *filepath)
{
    if (!apuLoader)
        apuLoader = new BDAPULoader;
    
    return apuLoader->loadAPU(filepath);
}

//Generate the APU code and build the dylib
void write_test_system(const char *outputdir)
{
    BDCodeBuilder cb("test", outputdir);
    cb.writeHeaderFile();
    cb.openSourceFile();
    cb.addBlockNode("a0Node", BDBlockTypeMultiplier);
    cb.addBlockNode("a1Node", BDBlockTypeMultiplier);
    cb.addBlockNode("a2Node", BDBlockTypeMultiplier);
    cb.addBlockNode("b1Node", BDBlockTypeMultiplier);
    cb.addBlockNode("b2Node", BDBlockTypeMultiplier);
    
    cb.addBlockNode("summer", BDBlockTypeSummer);
    
    cb.addDelayLine("inDelayLine", "MAIN_INPUT_NODE", 2);
    cb.addDelayLine("outDelayLine", "summer", 2);
    
    cb.connect("a0Node", "summer");
    cb.connect("a1Node", "summer");
    cb.connect("a2Node", "summer");
    cb.connect("b1Node", "summer");
    cb.connect("b2Node", "summer");
    
    cb.getDelayLineNode("inDelay1", "inDelayLine", 1);
    cb.getDelayLineNode("inDelay2", "inDelayLine", 2);
    cb.getDelayLineNode("outDelay1", "outDelayLine", 1);
    cb.getDelayLineNode("outDelay2", "outDelayLine", 2);
    
    cb.connect("MAIN_INPUT_NODE", "a0Node");
    cb.connect("inDelay1", "a1Node");
    cb.connect("inDelay2", "a2Node");
    cb.connect("outDelay1", "b1Node");
    cb.connect("outDelay2", "b2Node");
    cb.connect("summer", "MAIN_OUTPUT_NODE");
    
    BiQuadCoefficients coeffs;
    coeffs.calculateForLPF(1000, 1.0, 44100);
    coeffs.b1 *= -1;
    coeffs.b2 *= -1;
    
    cb.setNumberDefaultValue("a0Node->coefficient", APUNumberType::APUNUM_FLOAT, &coeffs.a0);
    cb.setNumberDefaultValue("a1Node->coefficient", APUNumberType::APUNUM_FLOAT, &coeffs.a1);
    cb.setNumberDefaultValue("a2Node->coefficient", APUNumberType::APUNUM_FLOAT, &coeffs.a2);
    cb.setNumberDefaultValue("b1Node->coefficient", APUNumberType::APUNUM_FLOAT, &coeffs.b1);
    cb.setNumberDefaultValue("b2Node->coefficient", APUNumberType::APUNUM_FLOAT, &coeffs.b2);
    
    cb.closeSourceFile();
    
    BDCompiler compiler(&cb);
    compiler.compileLibrary(dylib_path);
}

int main(int argc, const char * argv[]) {
    write_test_system(built_code_path);
    
    AudioProcessingUnit *apunit = load_apu(dylib_path);
    if (!apunit) {
        BDLog("[setup]", "FAILED to load APU");
        abort();
    }
    
    BDLogFormat("[setup]", "Loaded APU: %s", apunit->getName());
    AudioManager audioManager;
    BDLog("[setup]", "Start Audio Manager");
    audioManager.setNumOutputChannels(2);
    
    //List all audio devices:
    AudioDeviceInfoRef devInfo = audioManager.getDevices();
    while (devInfo) {
        BDLogFormat("[setup]", "Found device: %s", devInfo->name);
        devInfo = devInfo->next;
    }
    
    
    AudioFile *file = new AudioFile(input_file_path, AudioFileModeReadOnly);
    apunit->setMaxInputChannels(file->numChannels());
    apunit->setMaxOutputChannels(file->numChannels());
    audioManager.setInputFile(file);
    audioManager.setInputMode(AudioInputModeFile);
    audioManager.setOutputDeviceIndex(1);
    audioManager.setLooping(false);
    
    BDLog("[setup]", "Start Audio");
    
    if (!audioManager.open())
    {
        BDLogError("[ERROR]", "Audio Manager failed to open");
        exit(1);
    }
    audioManager.setAudioProcessingUnit(apunit);
    
    if (!audioManager.start())
    {
        BDLogError("[ERROR]", "Audio Manager failed to start");
        exit(1);
    }
    
    //Play for 5 seconds
    audioManager.sleep(5 * 1000);
    
    audioManager.stop();
    audioManager.close();
    
    SAFE_DEL(apunit);
    SAFE_DEL(apuLoader);
    
    BDLog("", "Done");
}