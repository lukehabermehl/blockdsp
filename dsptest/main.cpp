//
//  main.cpp
//  dsptest
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include <iostream>
#include "AudioManager.hpp"
#include "libblockdsp.h"

static const char * input_file_path = "/Users/Luke/Desktop/guitar.wav";


void dspCallback(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels, void *context)
{
    float *gainptr = (float *)context;
    
    outputBuffer[0] = (*gainptr) * inputBuffer[0];
    if (numInputChannels == 1 && numOutputChannels == 2)
        outputBuffer[1] = outputBuffer[0];
    else if (numInputChannels == 2 && numOutputChannels == 2)
        outputBuffer[1] = (*gainptr) * inputBuffer[1];
}

int main(int argc, const char * argv[]) {
    printf("Start\n");
    
    printf("Setup audio manager\n");
    AudioManager audioManager;
    audioManager.setNumOutputChannels(2);
    float gain = 1.0;
    audioManager.setProcessCallback(&dspCallback, &gain);
    audioManager.setInputFile(input_file_path);
    audioManager.setInputMode(AudioInputModeFile);
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
    printf("Done\n");
}
