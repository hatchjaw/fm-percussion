/*
  ==============================================================================

    PatchSelectorComponent.cpp
    Created: 9 Jan 2022 1:19:39pm
    Author:  Tommy Rushton

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PatchSelectorComponent.h"

//==============================================================================
PatchSelectorComponent::PatchSelectorComponent(juce::AudioProcessorValueTreeState &apvts,
                                               const juce::String &oscSelectorID) {
    auto choices = dynamic_cast<juce::AudioParameterChoice *>(apvts.getParameter(oscSelectorID))->choices;
    patchSelector.addItemList(choices, 1);
    addAndMakeVisible(patchSelector);

    patchAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            apvts,
            oscSelectorID,
            patchSelector
    );
}

PatchSelectorComponent::~PatchSelectorComponent() {
}

void PatchSelectorComponent::paint(juce::Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

//    g.setColour(juce::Colours::grey);
//    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void PatchSelectorComponent::resized() {
    this->patchSelector.setBounds(10, 15, getWidth() - 20, getHeight() - 30);
}
