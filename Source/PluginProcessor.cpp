/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Voices/FMBellSound.h"
#include "Voices/FMBellVoice.h"
#include "Oscillators/FMOsc.h"
#include "defines.h"
#include "Voices/FMVoice.h"

//==============================================================================
PercussionFMAudioProcessor::PercussionFMAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    fmSynth.addSound(new FMBellSound());

    for (int i = 0; i < NUM_VOICES; ++i) {
        auto voice = new FMVoice();
        auto carrier = new FMOsc();
        carrier->addModulator(new FMOsc());
        voice->setCarrier(carrier);
        fmSynth.addVoice(voice);
//        fmSynth.addVoice(new FMBellVoice(new FMOsc()));
    }
}

PercussionFMAudioProcessor::~PercussionFMAudioProcessor()
{
}

//==============================================================================
const juce::String PercussionFMAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PercussionFMAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PercussionFMAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PercussionFMAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PercussionFMAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PercussionFMAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PercussionFMAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PercussionFMAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PercussionFMAudioProcessor::getProgramName (int index)
{
    return {};
}

void PercussionFMAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PercussionFMAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    fmSynth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < fmSynth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<FMVoice *>(fmSynth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, this->getTotalNumOutputChannels());
        }
    }
}

void PercussionFMAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PercussionFMAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void PercussionFMAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
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

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
//    for (int channel = 0; channel < totalNumInputChannels; ++channel)
//    {
//        auto* channelData = buffer.getWritePointer (channel);
//
//        // ..do something to the data...
//    }

    fmSynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool PercussionFMAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PercussionFMAudioProcessor::createEditor()
{
    return new PercussionFMAudioProcessorEditor (*this);
}

//==============================================================================
void PercussionFMAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PercussionFMAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PercussionFMAudioProcessor();
}
