/*
  ==============================================================================

    ScopeOnOffComponent.h
    Created: 7 Jan 2022 5:22:47pm
    Author:  Tommy Rushton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

//==============================================================================
/*
*/
class ScopeOnOffComponent : public juce::Component {
public:
    ScopeOnOffComponent(juce::AudioProcessorValueTreeState &, juce::String);

    ~ScopeOnOffComponent() override;

    void paint(juce::Graphics &) override;

    void resized() override;

private:
    juce::ToggleButton scopeOnOffButton;
    juce::Label scopeOnOffLabel{"Scope on", "Scope on"};
    std::unique_ptr<ButtonAttachment> scopeOnOffAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScopeOnOffComponent)
};
