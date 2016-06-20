//
//  main.cpp
//  dsptest
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include <iostream>
#include "dsphelpers.hpp"
#include <blockdsp.h>

#define SAFE_DEL(_x) if (_x) delete _x

static const char * input_file_path = "/Users/Luke/Desktop/guitar.wav";
static BDAPULoader *apuLoader = 0;

AudioProcessingUnit * load_apu(const char *filepath)
{
    if (!apuLoader)
        apuLoader = new BDAPULoader;
    
    return apuLoader->loadAPU(filepath);
}

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
    
    cb.setNumberDefaultValue("a0Node->coefficient", BlockDSPParameterTypeFloat, &coeffs.a0);
    cb.setNumberDefaultValue("a1Node->coefficient", BlockDSPParameterTypeFloat, &coeffs.a1);
    cb.setNumberDefaultValue("a2Node->coefficient", BlockDSPParameterTypeFloat, &coeffs.a2);
    cb.setNumberDefaultValue("b1Node->coefficient", BlockDSPParameterTypeFloat, &coeffs.b1);
    cb.setNumberDefaultValue("b2Node->coefficient", BlockDSPParameterTypeFloat, &coeffs.b2);
    
    cb.closeSourceFile();
    
    BDCompiler compiler(&cb);
    compiler.compileLibrary("/Users/Luke/Desktop/test.dylib");
}

int main(int argc, const char * argv[]) {
    write_test_system("/Users/Luke/Desktop/test");
    
    AudioProcessingUnit *apunit = load_apu("/Users/Luke/Desktop/test.dylib");
    if (!apunit)
        BDLog("[setup]", "FAILED to load APU");
    
    BDLog("[setup]", "Start");
    AudioManager audioManager;
    BDLog("[setup]", "Start Audio Manager");
    audioManager.setNumOutputChannels(2);
    AudioFile *file = new AudioFile(input_file_path, AudioFileModeReadOnly);
    audioManager.setInputFile(file);
    audioManager.setInputMode(AudioInputModeFile);
    audioManager.setAudioProcessingUnit(apunit);
    audioManager.setLooping(true);
    
    BDLog("[setup]", "Start Audio");
    
    if (!audioManager.open())
    {
        BDLog("[ERROR]", "Audio Manager failed to open");
        exit(1);
    }
    if (!audioManager.start())
    {
        BDLog("[ERROR]", "Audio Manager failed to start");
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