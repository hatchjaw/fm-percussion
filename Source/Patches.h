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
        DRUM3,
        CHIME1,
        MARIMBA,
        SHEET_METAL,
        TIMPANI
    };

    static FMOsc *generateOscillator(Patch patch) {
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
                                        FMOsc::LINEAR,
                                        new OADEnv::Parameters(0.0f, 0.0f, .025f)
                                )
                        },
                        new OADEnv::Parameters(0.8f, 0.05f, .15f)
                );
            case DRUM3:
                return new FMVoice::Parameters(
                        FMOsc::LINEAR,
                        {
                                FMOsc::Parameters(
                                        .6875,
                                        2500.,
                                        0.0,
                                        FMOsc::LINEAR,
                                        new OADEnv::Parameters(0.0f, 0.0f, .016f)
                                )
                        },
                        new OADEnv::Parameters(.8f, .02f, .18f)
                );
            case CHIME1:
                return new FMVoice::Parameters(
                        FMOsc::EXPONENTIAL,
                        {
                                FMOsc::Parameters(
                                        2.05,
                                        5.,
                                        0.0,
                                        FMOsc::LINEAR,
                                        new OADEnv::Parameters(0.0f, 0.0f, .75f)
                                )
                        },
                        new OADEnv::Parameters(0.0f, 0.0f, 1.f)
                );
            case MARIMBA:
                return new FMVoice::Parameters(
                        FMOsc::LINEAR,
                        {FMOsc::Parameters(2.4, 500., 0.0, FMOsc::LINEAR,
                                           new OADEnv::Parameters(0.0f, 0.0f, .15f))},
                        new OADEnv::Parameters(.8f, .02f, .18f)
                );
            case SHEET_METAL: {
                auto duration = 1.f;
                return new FMVoice::Parameters(
                        FMOsc::LINEAR,
                        {FMOsc::Parameters(2.2, 4000., .05, FMOsc::LINEAR,
                                           new OADEnv::Parameters(0.0f, .12f * duration, .88f * duration))},
                        new OADEnv::Parameters(0.8f, .2f * duration, .8f * duration)
                );
            }
            case TIMPANI: {
                auto duration = 1.f;
                return new FMVoice::Parameters(
                        FMOsc::EXPONENTIAL,
                        {
                                FMOsc::Parameters(
                                        1.3,
                                        5.,
                                        .25,
                                        FMOsc::LINEAR,
                                        new OADEnv::Parameters(0.0f, 0.0f, .1f * duration)
                                ),
                                FMOsc::Parameters(
                                        1.6,
                                        9.,
                                        .25,
                                        FMOsc::LINEAR,
                                        new OADEnv::Parameters(0.0f, 0.0f, .8f * duration)
                                ),
                        },
                        new OADEnv::Parameters(0.8f, .125f * duration, .875f * duration)
                );
            }
            default:
                jassertfalse;
                break;
        }

        jassertfalse;
        return nullptr;
    }
};
