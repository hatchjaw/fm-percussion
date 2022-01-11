/*
  ==============================================================================

    ModulationAmountComponent.h
    Created: 11 Jan 2022 1:58:21pm
    Author:  Tommy Rushton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

//==============================================================================
/*
*/
class ModulationAmountComponent : public juce::Component {
public:
    ModulationAmountComponent(juce::AudioProcessorValueTreeState &apvts, const juce::String& modAmountID);

    ~ModulationAmountComponent() override;

    void paint(juce::Graphics &) override;

    void resized() override;

private:
    juce::Slider modAmountSlider;
    std::unique_ptr<SliderAttachment> modAmountAttachment;
    juce::Label modAmountLabel{"Modulation Amount", "Mod Amount %"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulationAmountComponent)
};
