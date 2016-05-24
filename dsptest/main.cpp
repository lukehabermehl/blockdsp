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


class GainUnit : public AudioProcessingUnit
{
public:
    float gain;
    virtual void processAudio(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels)
    {
        outputBuffer[0] = inputBuffer[0] * gain;
        if (numInputChannels == 1 && numOutputChannels == 2)
        {
            outputBuffer[1] = outputBuffer[0];
        }
        
        else if (numInputChannels == 2 && numOutputChannels == 2)
        {
            outputBuffer[1] = inputBuffer[1] * gain;
        }
    }
};

int main(int argc, const char * argv[]) {
    GainUnit gainUnit;
    gainUnit.gain = 1.0;
    
    printf("Start\n");
    printf("Setup audio manager\n");
    AudioManager audioManager;
    audioManager.setNumOutputChannels(2);
    audioManager.setInputFile(input_file_path);
    audioManager.setInputMode(AudioInputModeFile);
    audioManager.setAudioProcessingUnit(&gainUnit);
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
