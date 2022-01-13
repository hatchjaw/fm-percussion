/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PercussionFMAudioProcessorEditor::PercussionFMAudioProcessorEditor(PercussionFMAudioProcessor &p)
        : AudioProcessorEditor(&p),
          audioProcessor(p),
          scopeComponent(audioProcessor.getAudioBufferQueue(), audioProcessor.apvts),
          scopeOnOffComponent(audioProcessor.apvts, "SCOPE"),
          filterOnOffComponent(audioProcessor.apvts, "ANTIALIAS"),
          patchSelectorComponent(audioProcessor.apvts, "PATCH"),
          modulationAmountComponent(audioProcessor.apvts, "MOD_SCALE"){
    setSize(800, 600);

    addAndMakeVisible(scopeComponent);

    addAndMakeVisible(scopeOnOffComponent);

    addAndMakeVisible(filterOnOffComponent);

    addAndMakeVisible(patchSelectorComponent);

    addAndMakeVisible(modulationAmountComponent);
}

PercussionFMAudioProcessorEditor::~PercussionFMAudioProcessorEditor() {
}

//==============================================================================
void PercussionFMAudioProcessorEditor::paint(juce::Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void PercussionFMAudioProcessorEditor::resized() {
    auto area = getLocalBounds();
    auto scopeY = 80;

    scopeComponent.setTopLeftPosition(0, scopeY);
    scopeComponent.setSize(area.getWidth(), area.getHeight() - 100);

    scopeOnOffComponent.setBounds(0, 0, getWidth() / 5, scopeY/2);

    filterOnOffComponent.setBounds(0, scopeY/2, getWidth() / 5, scopeY/2);

    patchSelectorComponent.setBounds(scopeOnOffComponent.getRight(), 0, 3 * getWidth() / 10, scopeY);

    modulationAmountComponent.setBounds(patchSelectorComponent.getRight(), 0, getWidth() / 2, scopeY);
}
