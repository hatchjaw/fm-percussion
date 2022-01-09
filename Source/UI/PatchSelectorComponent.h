/*
  ==============================================================================

    PatchSelectorComponent.h
    Created: 9 Jan 2022 1:19:39pm
    Author:  Tommy Rushton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PatchSelectorComponent  : public juce::Component
{
public:
    PatchSelectorComponent(juce::AudioProcessorValueTreeState &apvts,
                           const juce::String& oscSelectorID);
    ~PatchSelectorComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox patchSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> patchAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatchSelectorComponent)
};
