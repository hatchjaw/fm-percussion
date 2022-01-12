/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Voices/FMSound.h"
#include "Voices/FMVoice.h"
#include "Patches.h"
#include "defines.h"

//==============================================================================
PercussionFMAudioProcessor::PercussionFMAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
        : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                                 .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                                 .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
    fmSynth.addSound(new FMSound());

    for (int i = 0; i < NUM_VOICES; ++i) {
        auto osc = Patches::generateOscillator(static_cast<Patches::Patch>(currentPatch));
        auto voice = new FMVoice();
        voice->setCarrier(osc);

        fmSynth.addVoice(voice);
    }

    // Add a lowpass filter per channel.
    for (int channel = 0; channel < getTotalNumOutputChannels(); ++channel) {
        auto lpf = new LowPassFilter;
        filter.push_back(*lpf);
    }
}

PercussionFMAudioProcessor::~PercussionFMAudioProcessor() {
}

//==============================================================================
const juce::String PercussionFMAudioProcessor::getName() const {
    return JucePlugin_Name;
}

bool PercussionFMAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PercussionFMAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PercussionFMAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PercussionFMAudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int PercussionFMAudioProcessor::getNumPrograms() {
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int PercussionFMAudioProcessor::getCurrentProgram() {
    return 0;
}

void PercussionFMAudioProcessor::setCurrentProgram(int index) {
}

const juce::String PercussionFMAudioProcessor::getProgramName(int index) {
    return {};
}

void PercussionFMAudioProcessor::changeProgramName(int index, const juce::String &newName) {
}

//==============================================================================
void PercussionFMAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    fmSynth.setCurrentPlaybackSampleRate(sampleRate * oversamplingFactor);

    for (int i = 0; i < fmSynth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<FMVoice *>(fmSynth.getVoice(i))) {
            voice->prepareToPlay(sampleRate * oversamplingFactor, samplesPerBlock, this->getTotalNumOutputChannels());
        }
    }
}

void PercussionFMAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations

bool PercussionFMAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const {
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
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

#endif

void PercussionFMAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Read parameter values from the value tree state.
    auto scopeOn = apvts.getRawParameterValue("SCOPE")->load() > .5f;
    auto patchNum = (int) apvts.getRawParameterValue("PATCH")->load();
    // Modulation amount appears to the user as a percentage; divide by 100 so it can be used as a scaling value.
    auto modAmount = apvts.getRawParameterValue("MOD_SCALE")->load() / 100.f;

    // Check whether the patch number has changed; if so, load the new patch.
    if (patchNum != currentPatch) {
        fmSynth.clearVoices();
        currentPatch = patchNum;

        for (int i = 0; i < NUM_VOICES; ++i) {
            auto osc = Patches::generateOscillator(static_cast<Patches::Patch>(patchNum));
            auto voice = new FMVoice();
            voice->setCarrier(osc);
            voice->prepareToPlay(getSampleRate() * oversamplingFactor, getBlockSize(), getTotalNumOutputChannels());

            fmSynth.addVoice(voice);
        }
    }

    // Update the modulation amount; this recursively scales the peak deviation and feedback for all modulators.
    for (int i = 0; i < fmSynth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<FMVoice *>(fmSynth.getVoice(i))) {
            voice->updateModulationAmount(modAmount);
        }
    }

    // Set up a temporary, oversampled buffer.
    juce::AudioBuffer<float> oversampledBuffer{getTotalNumOutputChannels(),
                                               buffer.getNumSamples() * oversamplingFactor};
    oversampledBuffer.clear();

    // Render the next block of output to the oversampled buffer.
    fmSynth.renderNextBlock(oversampledBuffer, midiMessages, 0, oversampledBuffer.getNumSamples());

    // Half-band filter the oversampled buffer.
//    filter.processBlock(oversampledBuffer, oversampledBuffer.getNumSamples());

    for (int channel = 0; channel < oversampledBuffer.getNumChannels(); ++channel) {
        for (int sample = 0; sample < oversampledBuffer.getNumSamples(); ++sample) {
            oversampledBuffer.setSample(
                    channel,
                    sample,
                    filter[channel].processSample(oversampledBuffer.getSample(channel, sample))
            );
        }
    }

    // Write the filtered samples to the output buffer.
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            auto newSample = oversampledBuffer.getSample(channel, sample * oversamplingFactor);
            buffer.setSample(channel, sample, newSample);
        }
    }

    // If enabled, gather data to be displayed in the scope.
    if (scopeOn) {
        scopeDataCollector.process(buffer.getReadPointer(0), (size_t) buffer.getNumSamples());
    }
}

//==============================================================================
bool PercussionFMAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *PercussionFMAudioProcessor::createEditor() {
    return new PercussionFMAudioProcessorEditor(*this);
}

//==============================================================================
void PercussionFMAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PercussionFMAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PercussionFMAudioProcessor();
}

//=============================================================================/
// Value tree
juce::AudioProcessorValueTreeState::ParameterLayout PercussionFMAudioProcessor::createParams() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Scope on/off
    params.push_back(std::make_unique<juce::AudioParameterBool>("SCOPE", "Scope on", true));

    // Patch selector
    params.push_back(std::make_unique<juce::AudioParameterChoice>("PATCH", "Patch", juce::StringArray{
            "Bell 1 (Chowning)",
            "Bell 2 (fixed modulator)",
            "Bell 3 (MMFM)",
            "Drum 1 (Chowning)",
            "Drum 2 (Chowning)",
            "Drum 3",
            "Chime 1",
            "Marimba",
            "Sheet Metal",
            "Timpani"
    }, 0));

    // Scalar for modulation index and feedback.
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
            "MOD_SCALE",
            "Mod amount",
            juce::NormalisableRange<float>(0.0f, 200.0f, 1.f),
            100.f));

    return {params.begin(), params.end()};
}
