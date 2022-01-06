/*
  ==============================================================================

    FMBellSound.h
    Created: 1 Jan 2022 8:46:15pm
    Author:  Tommy Rushton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FMBellSound : public juce::SynthesiserSound {
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }

    bool appliesToChannel(int midiChannel) override { return true; }
};