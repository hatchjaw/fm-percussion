/*
  ==============================================================================

    FMOsc.h
    Created: 2 Jan 2022 11:59:09am
    Author:  Tommy Rushton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Envelopes/OADEnv.h"

class FMOsc {
public:
    /**
     * Frequency modulation mode.
     * @see computeNextSample
     */
    enum FMMode {
        /**
         * Linear FM, y = sin(fc + I * sin(fm))
         */
        LINEAR,
        /**
         * Exponential FM, y = sin(fc * 2^(I * sin(fm))
         */
        EXPONENTIAL
    };

    /**
     * Modulation mode.
     * @see setupNote
     */
    enum ModulationMode {
        /**
         * Use a modulation frequency proportional to the carrier frequency.
         */
        PROPORTIONAL,
        /**
         * Use a fixed modulation frequency, irrespective of the carrier frequency.
         */
        FIXED
    };

    struct Parameters {
//        Parameters() = default;

        /**
         * Constructor to use for modulation proportional wrt carrier frequency
         * @param modFreqRatioToUse
         * @param peakDeviationToUse
         * @param feedbackToUse
         * @param modeToUse
         * @param submodulatorParams
         */
        Parameters(double modFreqRatioToUse,
                   double peakDeviationToUse,
                   double feedbackToUse = 0.0,
                   FMOsc::FMMode modeToUse = LINEAR,
                   std::vector<FMOsc::Parameters> submodulatorParams = {}) :
                mode(modeToUse),
                modFreqRatio(modFreqRatioToUse),
                peakDeviation(peakDeviationToUse),
                feedback(feedbackToUse),
                modulatorParams(std::move(submodulatorParams)) {}

        /**
         * Constructor to use for fixed-frequency modulation
         * @param modModeToUse
         * @param modFreqToUse
         * @param peakDeviationToUse
         * @param feedbackToUse
         * @param modeToUse
         * @param submodulatorParams
         */
        Parameters(ModulationMode modModeToUse,
                   double modFreqToUse,
                   double peakDeviationToUse,
                   double feedbackToUse = 0.0,
                   FMOsc::FMMode modeToUse = LINEAR,
                   std::vector<FMOsc::Parameters> submodulatorParams = {}) :
                mode(modeToUse),
                modMode(modModeToUse),
                modFreq(modFreqToUse),
                peakDeviation(peakDeviationToUse),
                feedback(feedbackToUse),
                modulatorParams(std::move(submodulatorParams)) {}

        /**
         * Generate an FM oscillator from a parameter object.
         * @return
         */
        FMOsc generateOscillator() {
            auto osc = FMOsc(mode);
            osc.modMode = modMode;
            osc.modulationFrequency = modFreq;
            osc.modulationFrequencyRatio = modFreqRatio;
            osc.peakDeviation = peakDeviation;
            for (auto p: modulatorParams) {
                osc.addModulator(p.generateOscillator());
            }
            return osc;
        }

        FMOsc::FMMode mode{LINEAR};
        FMOsc::ModulationMode modMode{PROPORTIONAL};
        double modFreqRatio{2.0};
        double modFreq{100.};
        double peakDeviation{1.0};
        double feedback{0.0};
        std::vector<FMOsc::Parameters> modulatorParams;
    };

    explicit FMOsc(FMMode modeToSet = LINEAR);

    void addModulator(FMOsc modulatorToAdd);

    void prepareToPlay(juce::dsp::ProcessSpec &spec);

    void setupNote(double sampleRate, double frequency, float noteAmplitude);

    void setupModulators(double sampleRate, double frequency, double peakDeviationToUse);

    float computeNextSample();

    void computeNextBlock(juce::AudioBuffer<float> &buffer, int startSample, int numSamples);

    void reset();

    bool isActive();

    /**
     * Set the amplitude envelope for the oscillator and its modulators.
     * Shouldn't be called until all modulators have been added.
     */
    void setEnvelope(OADEnv::Parameters &);

private:
    std::vector<FMOsc> modulators;

    FMMode mode;
    ModulationMode modMode{PROPORTIONAL};

    double sampleRate{0.0};

    double currentAngle{0.0};
    double angleDelta{0.0};
    double amplitude{1.0};

    // For proportional modulation
    double modulationFrequencyRatio{0.0};
    // For fixed modulation
    double modulationFrequency{0.0};

    double peakDeviation{0.0};

    OADEnv envelope;
};
