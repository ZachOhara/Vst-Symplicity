/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
VstsymplicityAudioProcessor::VstsymplicityAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	),
	symplicity(*this)
#endif
{
}

VstsymplicityAudioProcessor::~VstsymplicityAudioProcessor()
{
}

//==============================================================================
const String VstsymplicityAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VstsymplicityAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VstsymplicityAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double VstsymplicityAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VstsymplicityAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VstsymplicityAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VstsymplicityAudioProcessor::setCurrentProgram (int index)
{
	index; // suppress the warning
}

const String VstsymplicityAudioProcessor::getProgramName (int index)
{
	index; // suppress the warning
    return {};
}

void VstsymplicityAudioProcessor::changeProgramName (int index, const String& newName)
{
	index; // suppress the warning
	newName; // suppress the warning
}

//==============================================================================
void VstsymplicityAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	symplicity.PrepareToPlay(sampleRate, samplesPerBlock);
}

void VstsymplicityAudioProcessor::releaseResources()
{
	symplicity.ReleaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VstsymplicityAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void VstsymplicityAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	symplicity.ProcessBlock(buffer, midiMessages);
}

//==============================================================================
bool VstsymplicityAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* VstsymplicityAudioProcessor::createEditor()
{
    return &(symplicity.ConstructEditor(*this));
}

//==============================================================================
void VstsymplicityAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void VstsymplicityAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VstsymplicityAudioProcessor();
}
