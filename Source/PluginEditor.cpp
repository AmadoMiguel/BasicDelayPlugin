/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayIntroNewAudioProcessorEditor::DelayIntroNewAudioProcessorEditor (DelayIntroNewAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (600, 400);
    
    auto& params = processor.getParameters();
    
    // Visual characteristics of sliders
    lookAndFeel.setColour(Slider::thumbColourId, Colours::azure);
    lookAndFeel.setColour(Slider::rotarySliderOutlineColourId, Colours::darkblue);
    lookAndFeel.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
    lookAndFeel.setColour(Slider::rotarySliderFillColourId, Colours::lightslategrey);
    
    // Customize and make visible
    AudioParameterFloat* dryWetParam = (AudioParameterFloat*)params.getUnchecked(0);
    dryWetSlider.setRange(dryWetParam->range.start, dryWetParam->range.end);
    dryWetSlider.setValue(*dryWetParam);
    dryWetSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 40);
    dryWetSlider.setNumDecimalPlacesToDisplay(2);
    dryWetSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    dryWetSlider.setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(dryWetSlider);
    
    // Setup label for dry wet slider
//    dryWetLabel.setText("delay", dontSendNotification);
//    dryWetLabel.attachToComponent(&dryWetSlider, true);
//    addAndMakeVisible(dryWetLabel);
    
    // Hook changes of the slider with the processor parameter
    dryWetSlider.onDragStart = [dryWetParam] {
        dryWetParam->beginChangeGesture();
    };
    dryWetSlider.onValueChange = [this, dryWetParam] {
        *dryWetParam = dryWetSlider.getValue();
    };
    dryWetSlider.onDragEnd = [dryWetParam] {
        dryWetParam->endChangeGesture();
    };
    
    AudioParameterFloat* feedbackParam = (AudioParameterFloat*)params.getUnchecked(1);
    feedbackSlider.setRange(feedbackParam->range.start, feedbackParam->range.end);
    feedbackSlider.setValue(*feedbackParam);
    feedbackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 40);
    feedbackSlider.setNumDecimalPlacesToDisplay(2);
    feedbackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    feedbackSlider.setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(feedbackSlider);
    
//    feedbackLabel.setText("feedback", dontSendNotification);
//    feedbackLabel.attachToComponent(&feedbackSlider, true);
//    addAndMakeVisible(feedbackLabel);
    
    feedbackSlider.onDragStart = [feedbackParam] {
        feedbackParam->beginChangeGesture();
    };
    feedbackSlider.onValueChange = [this, feedbackParam] {
        *feedbackParam = feedbackSlider.getValue();
    };
    feedbackSlider.onDragEnd = [feedbackParam] {
        feedbackParam->endChangeGesture();
    };
    
    AudioParameterFloat* delayTimeParam = (AudioParameterFloat*)params.getUnchecked(2);
    delayTimeSlider.setRange(delayTimeParam->range.start, delayTimeParam->range.end);
    delayTimeSlider.setValue(*delayTimeParam);
    delayTimeSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 40);
    delayTimeSlider.setTextValueSuffix(" sec");
    delayTimeSlider.setNumDecimalPlacesToDisplay(2);
    delayTimeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    delayTimeSlider.setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(delayTimeSlider);
    
//    delayTimeLabel.setText("delay", dontSendNotification);
//    delayTimeLabel.attachToComponent(&delayTimeSlider, true);
//    addAndMakeVisible(delayTimeLabel);
    
    delayTimeSlider.onDragStart = [delayTimeParam] {
        delayTimeParam->beginChangeGesture();
    };
    delayTimeSlider.onValueChange = [this, delayTimeParam] {
        *delayTimeParam = delayTimeSlider.getValue();
    };
    delayTimeSlider.onDragEnd = [delayTimeParam] {
        delayTimeParam->endChangeGesture();
    };
    
}

DelayIntroNewAudioProcessorEditor::~DelayIntroNewAudioProcessorEditor()
{
    delayTimeSlider.setLookAndFeel(nullptr);
    feedbackSlider.setLookAndFeel(nullptr);
    dryWetSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void DelayIntroNewAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::lightblue);
}

void DelayIntroNewAudioProcessorEditor::resized()
{
    // Assign each parameter its corresponding space in screen
    Rectangle<int> mainArea = getLocalBounds();
    Rectangle<int> topArea = mainArea.removeFromTop(getHeight() / 2);
    delayTimeSlider.setBounds(topArea.removeFromLeft(getWidth() / 2));
    feedbackSlider.setBounds(topArea.removeFromRight(getWidth() / 2));
    dryWetSlider.setBounds(mainArea.removeFromBottom(getHeight() / 2));
}
