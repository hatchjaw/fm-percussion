/*
  ==============================================================================

    Patches.h
    Created: 9 Jan 2022 5:40:03pm
    Author:  Tommy Rushton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Voices/FMVoice.h"
#include "Oscillators/FMOsc.h"

class Patches {
public:
    enum Patch {
        BELL1,
        BELL2,
        BELL3,
        DRUM1,
        DRUM2,
        CHIME1,
    };

    static FMOsc *getOscillator(Patch patch) {
        auto params = getPatch(patch);
        auto carrier = new FMOsc(params->carrierMode);

        for (auto s: params->modulatorSettings) {
            auto m = s.generateOscillator();
            carrier->addModulator(m);
        }

        carrier->setEnvelope(*params->envParams);
        return carrier;
    }

private:
    static FMVoice::Parameters *getPatch(Patch patch) {
        switch (patch) {
            case BELL1:
                return new FMVoice::Parameters(
                        FMOsc::LINEAR,
                        {FMOsc::Parameters(1.4, 1000.)},
                        new OADEnv::Parameters(0.0f, 0.0f, 7.5f)
                );
            case BELL2:
                return new FMVoice::Parameters(
                        FMOsc::EXPONENTIAL,
                        {
                                FMOsc::Parameters(FMOsc::ModulationMode::FIXED, 550., 1.5),
                                FMOsc::Parameters(FMOsc::ModulationMode::FIXED, 725., .1)
                        },
                        new OADEnv::Parameters(0.f, 0.f, 5.f)
                );
            case BELL3:
                return new FMVoice::Parameters(
                        FMOsc::LINEAR,
                        {
                                FMOsc::Parameters(1.4, 500., 0.1, FMOsc::EXPONENTIAL, nullptr, {
                                        FMOsc::Parameters(1.4, 1.9)
                                }),
                                FMOsc::Parameters(1.35, .5)
                        },
                        new OADEnv::Parameters(0.f, 0.f, 5.f)
                );
            case DRUM1:
                return new FMVoice::Parameters(
                        FMOsc::LINEAR,
                        {FMOsc::Parameters(1.4, 1000.)},
                        new OADEnv::Parameters(0.8f, 0.05f, .15f)
                );
            case DRUM2:
                return new FMVoice::Parameters(
                        FMOsc::LINEAR,
                        {
                                FMOsc::Parameters(
                                        80.f / 55.f,
                                        1000.,
                                        0.05,
                                        FMOsc::EXPONENTIAL,
                                        new OADEnv::Parameters(0.0f, 0.0f, .025f)
                                )
                        },
                        new OADEnv::Parameters(0.8f, 0.05f, .15f)
                );
            case CHIME1:
                return new FMVoice::Parameters(
                        FMOsc::LINEAR,
                        {
                                FMOsc::Parameters(
                                        2.05,
                                        1000.,
                                        0.0,
                                        FMOsc::EXPONENTIAL,
                                        new OADEnv::Parameters(0.0f, 0.0f, .75f)
                                )
                        },
                        new OADEnv::Parameters(0.0f, 0.0f, 1.f)
                );
            default:
                jassertfalse;
        }

        jassertfalse;
        return nullptr;
    }
};
