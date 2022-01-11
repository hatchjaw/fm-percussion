/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/ScopeComponent.h"
#include "UI/ScopeOnOffComponent.h"
#include "UI/PatchSelectorComponent.h"
#include "UI/ModulationAmountComponent.h"

//==============================================================================
/**
*/
class PercussionFMAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PercussionFMAudioProcessorEditor (PercussionFMAudioProcessor&);
    ~PercussionFMAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PercussionFMAudioProcessor& audioProcessor;
    ScopeComponent<float> scopeComponent;
    ScopeOnOffComponent scopeOnOffComponent;
    PatchSelectorComponent patchSelectorComponent;
    ModulationAmountComponent modulationAmountComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PercussionFMAudioProcessorEditor)
};
