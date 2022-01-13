/*
  ==============================================================================

    ScopeOnOffComponent.cpp
    Created: 7 Jan 2022 5:22:47pm
    Author:  Tommy Rushton

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ScopeOnOffComponent.h"

//==============================================================================
ScopeOnOffComponent::ScopeOnOffComponent(juce::AudioProcessorValueTreeState &apvts, juce::String scopeOnOffID)
{
    scopeOnOffLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    scopeOnOffLabel.setJustificationType(juce::Justification::centred);
    scopeOnOffLabel.setFont(15.f);
    addAndMakeVisible(scopeOnOffLabel);

    addAndMakeVisible(scopeOnOffButton);
    scopeOnOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts,
            scopeOnOffID,
            scopeOnOffButton
    );
}

ScopeOnOffComponent::~ScopeOnOffComponent()
{
}

void ScopeOnOffComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

//    g.setColour (juce::Colours::grey);
//    g.drawRect (getLocalBounds(), 1);
}

void ScopeOnOffComponent::resized()
{
    scopeOnOffLabel.setBounds(0, 0, 2 * getWidth() / 3, getHeight());
    scopeOnOffButton.setBounds(scopeOnOffLabel.getRight(), scopeOnOffLabel.getY(), getWidth() / 3, getHeight());
}
