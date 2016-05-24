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

static const char * input_file_path = "";

struct LPFData {
    float z1_left;
    float z1_right;
};


void dspCallback(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels, void *context)
{
    LPFData *lpfData = (LPFData *)context;
    
    outputBuffer[0] = (0.5 * inputBuffer[0]) + (0.5 * lpfData->z1_left);
    
    lpfData->z1_left = inputBuffer[0];
    
    if (numOutputChannels == 2 && numInputChannels == 1)
    {
        outputBuffer[1] = outputBuffer[0];
    }
    
    else if (numOutputChannels == 2 && numInputChannels == 2)
    {
        outputBuffer[1] = (0.5 * inputBuffer[0]) + (0.5 * lpfData->z1_right);
        lpfData->z1_right = inputBuffer[1];
    }
}

int main(int argc, const char * argv[]) {
    LPFData lpfData{0, 0};
    
    AudioManager audioManager;
    audioManager.setProcessCallback(&dspCallback, &lpfData);
    audioManager.setInputFile(input_file_path);
    audioFileManager.setInputMode(AudioInputModeFile);
    
    audioManager.start();
}
