//
//  main.cpp
//  dsptest
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include <iostream>
#include "AudioManager.hpp"
#include "BlockDSPAPU.hpp"

//static const char * input_file_path = "/Users/Luke/Desktop/guitar.wav";

//int main(int argc, const char * argv[]) {
//    BlockDSPAPU apunit;
//    apunit.system = BlockDSPSystem::systemForBiQuad(2);
//    
//    printf("Start\n");
//    printf("Setup audio manager\n");
//    AudioManager audioManager;
//    audioManager.setNumOutputChannels(2);
//    audioManager.setInputFile(input_file_path);
//    audioManager.setInputMode(AudioInputModeFile);
//    audioManager.setAudioProcessingUnit(&apunit);
//    audioManager.setLooping(true);
//    
//    printf("Start audio\n");
//    
//    if (!audioManager.open())
//    {
//        printf("AudioManager failed to open\n");
//        exit(1);
//    }
//    if (!audioManager.start())
//    {
//        printf("AudioManager failed to start\n");
//        exit(1);
//    }
//    
//    //Play for 10 seconds
//    audioManager.sleep(10 * 1000);
//    
//    audioManager.stop();
//    audioManager.close();
//    
//    delete apunit.system;
//    printf("Done\n");
//}

#include "BDCodeBuilder.hpp"

int main()
{
    BDCodeBuilder cb("test", "/Users/Luke/Desktop/");
    cb.writeHeaderFile();
    cb.openSourceFile();
    
    cb.addBlockNode("firstNode", BDBlockTypeSummer);
    cb.closeSourceFile();
}