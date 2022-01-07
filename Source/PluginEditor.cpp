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
          scopeOnOffComponent(audioProcessor.apvts, "SCOPE") {
    setSize(800, 600);

    addAndMakeVisible(scopeComponent);

    addAndMakeVisible(scopeOnOffComponent);
}

PercussionFMAudioProcessorEditor::~PercussionFMAudioProcessorEditor() {
}

//==============================================================================
void PercussionFMAudioProcessorEditor::paint(juce::Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void PercussionFMAudioProcessorEditor::resized() {
    auto area = getLocalBounds();

    scopeComponent.setTopLeftPosition(0, 80);
    scopeComponent.setSize(area.getWidth(), area.getHeight() - 100);

    scopeOnOffComponent.setBounds(0, 0, getWidth() / 5, 80);
}
