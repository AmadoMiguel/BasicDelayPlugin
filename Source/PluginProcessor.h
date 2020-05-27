/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// Define the max time for the delay in seconds
#define MAX_DELAY_TIME 2

//==============================================================================
/**
*/
class DelayIntroNewAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DelayIntroNewAudioProcessor();
    ~DelayIntroNewAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // Method to calculate the linear interpolation between two points
    float linInterp(float sample_x, float sample_x1, float inPhase);

private:
    AudioParameterFloat* dryWetParam;
    AudioParameterFloat* feedbackParam;
    AudioParameterFloat* delayTimeParam;
    
    // Use unique_ptr in order to avoid dealing with memory deallocation and
    // sample rate changes issues.
    std::unique_ptr<float[]> circBufferLeft;
    std::unique_ptr<float[]> circBufferRight;
    int circBufferWriteHead;
    int circBufferLength;
    float delayTimeSamples;
    float delayReadHead;
    float feedbackLeft;
    float feedbackRight;
    float delayTimeSmooth;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayIntroNewAudioProcessor)
};
