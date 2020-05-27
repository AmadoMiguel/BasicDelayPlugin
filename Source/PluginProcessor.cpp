/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayIntroNewAudioProcessor::DelayIntroNewAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                       )
{
    addParameter(dryWetParam = new AudioParameterFloat("drywet", "Dry Wet",
                                                      0, 1.0, 0.5));
    addParameter(feedbackParam = new AudioParameterFloat("feedback", "Feedback",
                                                       0, 0.98, 0.5));
    addParameter(delayTimeParam = new AudioParameterFloat("delay time", "Delay Time",
                                                          0.01, MAX_DELAY_TIME, 0.5));
    
    
    circBufferLeft = nullptr;
    circBufferRight = nullptr;
    circBufferWriteHead = 0;
    circBufferLength = 0;
    delayTimeSamples = 0;
    delayReadHead = 0;
    feedbackLeft = 0;
    feedbackRight = 0;
    delayTimeSmooth = 0;
}

DelayIntroNewAudioProcessor::~DelayIntroNewAudioProcessor()
{
}

//==============================================================================
const String DelayIntroNewAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayIntroNewAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayIntroNewAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayIntroNewAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayIntroNewAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayIntroNewAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayIntroNewAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayIntroNewAudioProcessor::setCurrentProgram (int index)
{
}

const String DelayIntroNewAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayIntroNewAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DelayIntroNewAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    circBufferLength = (int)(sampleRate * MAX_DELAY_TIME);
    if (circBufferLeft == nullptr) {
        circBufferLeft.reset(new float[circBufferLength]);
    }
    
    if (circBufferRight == nullptr) {
        circBufferRight.reset(new float[circBufferLength]);
    }
    
    circBufferWriteHead = 0;
    *delayTimeParam = sampleRate * 0.5;
    delayTimeSmooth = *delayTimeParam;
}

void DelayIntroNewAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool DelayIntroNewAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainInputChannelSet() == AudioChannelSet::stereo()
     && layouts.getMainOutputChannelSet() == AudioChannelSet::stereo())
        return true;
    return false;
}

void DelayIntroNewAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    float* leftChanData = buffer.getWritePointer(0);
    float* rightChanData = buffer.getWritePointer(1);
    for (int s = 0; s < buffer.getNumSamples(); s++) {
        // Smooth the changes of the delay time parameter
        delayTimeSmooth = delayTimeSmooth - 0.001 * (delayTimeSmooth - *delayTimeParam);
        
        // Add original signal and feedback
        circBufferLeft[circBufferWriteHead] = leftChanData[s] + feedbackLeft;
        circBufferRight[circBufferWriteHead] = rightChanData[s] + feedbackRight;
        
        delayTimeSamples = getSampleRate() * delayTimeSmooth;
        delayReadHead = circBufferWriteHead - delayTimeSamples;
        // This will be assigned the delayed signal samples previously stored on the
        // circular buffers
        if (delayReadHead < 0)
            delayReadHead += circBufferLength;
        
        // Get decimal part of the delayReadHead
        // These are the two values for which the linear interpolation is going to be applied
        int readHeadInt = (int) delayReadHead;
        int readHeadIntNext = readHeadInt + 1;
        if (readHeadIntNext >= circBufferLength) {
            readHeadIntNext = readHeadIntNext - circBufferLength;
        }
        
        float readHeadDecimals = delayReadHead - readHeadInt;
        
        // Adding feedback in order to have a more realistic delay
        // Use interpolation in order to handle sample values that are not covered by the circular buffer
        // indexes.
        float delaySampleLeft = linInterp(circBufferLeft[readHeadInt],
                                          circBufferLeft[readHeadIntNext], readHeadDecimals);
        feedbackLeft = delaySampleLeft * *feedbackParam;
        float delaySampleRight = linInterp(circBufferRight[readHeadInt],
                                          circBufferRight[readHeadIntNext], readHeadDecimals);
        feedbackRight = delaySampleRight * *feedbackParam;
        
        float balancedDelayLeft = buffer.getSample(0, s) * (1 - *dryWetParam) +
        delaySampleLeft * *dryWetParam;
        float balancedDelayRight = buffer.getSample(1, s) * (1 - *dryWetParam) +
        delaySampleRight * *dryWetParam;
        
        buffer.setSample(0, s, balancedDelayLeft);
        buffer.setSample(1, s, balancedDelayRight);
        
        circBufferWriteHead += 1;
        
        if (circBufferWriteHead >= circBufferLength)
            circBufferWriteHead = 0;
    }
}

//==============================================================================
bool DelayIntroNewAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelayIntroNewAudioProcessor::createEditor()
{
    return new DelayIntroNewAudioProcessorEditor (*this);
}

//==============================================================================
void DelayIntroNewAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayIntroNewAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayIntroNewAudioProcessor();
}

// This interpolation is one way of handling sample values in the circular buffers.
// Sample 1, sample 2 and the time between the 2 samples
float DelayIntroNewAudioProcessor::linInterp(float sampleX, float sampleX1, float inPhase) {
    return (1 - inPhase) * sampleX1 + inPhase * sampleX;
}
