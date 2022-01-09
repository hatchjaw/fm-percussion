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
          patchSelectorComponent(audioProcessor.apvts, "PATCH") {
    setSize(800, 600);

    addAndMakeVisible(scopeComponent);

    addAndMakeVisible(scopeOnOffComponent);

    addAndMakeVisible(patchSelectorComponent);
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

    scopeOnOffComponent.setBounds(0, 0, getWidth() / 5, scopeY);

    patchSelectorComponent.setBounds(scopeOnOffComponent.getRight(), 0, 3 * getWidth() / 10, scopeY);
}
