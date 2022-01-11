/*
  ==============================================================================

    ModulationAmountComponent.cpp
    Created: 11 Jan 2022 1:58:21pm
    Author:  Tommy Rushton

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ModulationAmountComponent.h"

//==============================================================================
ModulationAmountComponent::ModulationAmountComponent(
        juce::AudioProcessorValueTreeState &apvts,
        const juce::String &modAmountID
) {
    modAmountSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    modAmountSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxLeft, true, 50, 25);
    addAndMakeVisible(modAmountSlider);

    modAmountAttachment = std::make_unique<SliderAttachment>(apvts, modAmountID, modAmountSlider);

    modAmountLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    modAmountLabel.setJustificationType(juce::Justification::centred);
    modAmountLabel.setFont(15.f);
    addAndMakeVisible(modAmountLabel);
}

ModulationAmountComponent::~ModulationAmountComponent() {
}

void ModulationAmountComponent::paint(juce::Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

//    g.setColour(juce::Colours::grey);
//    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void ModulationAmountComponent::resized() {
    modAmountLabel.setBounds(0, 0, static_cast<int>(getWidth() * .25), getHeight());
    modAmountSlider.setBounds(modAmountLabel.getRight(), 0, static_cast<int>(getWidth() * .75), getHeight());
}
