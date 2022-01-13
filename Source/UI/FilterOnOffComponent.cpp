/*
  ==============================================================================

    FilterOnOffComponent.cpp
    Created: 13 Jan 2022 11:36:30am
    Author:  Tommy Rushton

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterOnOffComponent.h"

//==============================================================================
FilterOnOffComponent::FilterOnOffComponent(juce::AudioProcessorValueTreeState &apvts, juce::String filterOnOffID) {
    filterOnOffLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    filterOnOffLabel.setJustificationType(juce::Justification::centred);
    filterOnOffLabel.setFont(15.f);
    addAndMakeVisible(filterOnOffLabel);

    addAndMakeVisible(filterOnOffButton);
    filterOnOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts,
            filterOnOffID,
            filterOnOffButton
    );
}

FilterOnOffComponent::~FilterOnOffComponent() {
}

void FilterOnOffComponent::paint(juce::Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

//    g.setColour (juce::Colours::grey);
//    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void FilterOnOffComponent::resized() {
    filterOnOffLabel.setBounds(0, 0, 2 * getWidth() / 3, getHeight());
    filterOnOffButton.setBounds(filterOnOffLabel.getRight(), filterOnOffLabel.getY(), getWidth() / 3, getHeight());
}
