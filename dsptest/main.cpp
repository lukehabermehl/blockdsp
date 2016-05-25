//
//  main.cpp
//  dsptest
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include <iostream>
#include "AudioManager.hpp"
#include "BlockDSPSystem.hpp"

static const char * input_file_path = "/Users/Luke/Desktop/guitar.wav";


class DSPUnit : public AudioProcessingUnit
{
public:
    BlockDSPSystem *system;
    
    virtual void processAudio(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels)
    {
        system->mainInputNode->inputBuffer = inputBuffer;
        outputBuffer[0] = system->mainOutputNode->valueForChannel(0);
        
        if (numOutputChannels == 2)
            outputBuffer[1] = system->mainOutputNode->valueForChannel(1);
        
        system->next();
    }
};

int main(int argc, const char * argv[]) {
    DSPUnit apunit;
    BlockDSPSystem *system = BlockDSPSystem::systemForBiQuad(2);
    apunit.system = system;
    
    printf("Start\n");
    printf("Setup audio manager\n");
    AudioManager audioManager;
    audioManager.setNumOutputChannels(2);
    audioManager.setInputFile(input_file_path);
    audioManager.setInputMode(AudioInputModeFile);
    audioManager.setAudioProcessingUnit(&apunit);
    audioManager.setLooping(true);
    
    printf("Start audio\n");
    
    if (!audioManager.open())
    {
        printf("AudioManager failed to open\n");
        exit(1);
    }
    if (!audioManager.start())
    {
        printf("AudioManager failed to start\n");
        exit(1);
    }
    
    //Play for 10 seconds
    audioManager.sleep(10 * 1000);
    
    audioManager.stop();
    audioManager.close();
    
    delete system;
    printf("Done\n");
}
