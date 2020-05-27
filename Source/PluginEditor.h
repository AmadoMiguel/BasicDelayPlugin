/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DelayIntroNewAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    DelayIntroNewAudioProcessorEditor (DelayIntroNewAudioProcessor&);
    ~DelayIntroNewAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayIntroNewAudioProcessor& processor;
    
    Slider dryWetSlider;
    Slider feedbackSlider;
    Slider delayTimeSlider;
    
    Label dryWetLabel;
    Label feedbackLabel;
    Label delayTimeLabel;
    
    LookAndFeel_V4 lookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayIntroNewAudioProcessorEditor)
};
