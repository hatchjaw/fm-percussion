/*
  ==============================================================================

    FilterOnOffComponent.h
    Created: 13 Jan 2022 11:36:30am
    Author:  Tommy Rushton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

//==============================================================================
/*
*/
class FilterOnOffComponent : public juce::Component {
public:
    FilterOnOffComponent(juce::AudioProcessorValueTreeState &, juce::String);

    ~FilterOnOffComponent() override;

    void paint(juce::Graphics &) override;

    void resized() override;

private:
    juce::ToggleButton filterOnOffButton;
    juce::Label filterOnOffLabel{"Filter on", "Antialias on"};
    std::unique_ptr<ButtonAttachment> filterOnOffAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterOnOffComponent)
};
