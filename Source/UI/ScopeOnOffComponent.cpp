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
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
}

void ScopeOnOffComponent::resized()
{
    scopeOnOffLabel.setBounds(10, 10, getWidth() / 2, getHeight());
    scopeOnOffButton.setBounds(scopeOnOffLabel.getRight(), scopeOnOffLabel.getY(), getWidth() / 2, getHeight());
}
