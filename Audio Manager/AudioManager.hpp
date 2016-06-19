//
//  AudioManager.hpp
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef AudioManager_hpp
#define AudioManager_hpp

#include "AudioProcessingUnit.hpp"
#include "AudioFile.hpp"

/** Indicates where the Audio Manager will pull input from */
enum AudioInputMode
{
    AudioInputModeFile,
    AudioInputModeMicrophone
};

/** Indicates the current status of the Audio Manager */
enum AudioManagerStatus
{
    AudioManagerStatusRunning = 0,
    AudioManagerStatusDone
};

typedef unsigned int AudioDeviceIndex;

/** Contains useful information about an output device */
struct AudioDeviceInfo
{
    /** Index number of the device */
    AudioDeviceIndex index;
    /** Name of the device */
    const char *name;
    /** Next device in the linked list or NULL */
    AudioDeviceInfo *next;
};

/** Provides an interface for loading and processing audio from a file or input device */
class AudioManager
{
public:
    AudioManager();
    ~AudioManager();
    
    /** Load input from file */
    bool setInputFile(AudioFile *file);
    
    /** Specify the audio input source */
    void setInputMode(AudioInputMode mode);
    
    /** Set the output device */
    void setOutputDeviceIndex(AudioDeviceIndex devIndex);
    
    /** Get the current output device index */
    AudioDeviceIndex getOutputDeviceIndex();
    
    /** Get a linked list of available output device info structs */
    AudioDeviceInfo* getOutputDeviceInfo() const;
    
    /** Pass in an instance of AudioProcessingUnit to do the DSP */
    void setAudioProcessingUnit(AudioProcessingUnit *unit);
    
    /** Set the number of output channels */
    void setNumOutputChannels(int numOutputChannels);
    
    /** Enable/disable looping when using an input file */
    void setLooping(bool looping);
    
    /** Open the audio I/O streams. Returns true if successful */
    bool open();
    
    /** Close the audio I/O streams. Returns true if successful */
    bool close();
    
    /** Start audio processing. Returns true if successful */
    bool start();
    
    /** Stop audio processing. Returns false if successful */
    bool stop();
    
    /** Sleep the calling thread */
    void sleep(unsigned long millisec);
    
    /** Get the current status of the Audio Manager */
    AudioManagerStatus status();
    
private:
    class pimpl;
    pimpl *_pimpl;
};


#endif /* AudioManager_hpp */
