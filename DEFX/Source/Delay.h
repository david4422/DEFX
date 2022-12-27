/*
  ==============================================================================

    Delay.h
    Created: 28 Apr 2022 1:41:27pm
    Author:  david

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Delay
{
public:
	Delay();
	~Delay();

    void prepareToPlay(const juce::dsp::ProcessSpec& spec, juce::AudioProcessorValueTreeState& apvts);
    void processBlock(juce::AudioBuffer<float>& buffer, int totalNumInputChannels,int totalNumOutputChannels, juce::AudioProcessorValueTreeState& apvts);

    // Spec
    const juce::dsp::ProcessSpec& getSpec();
    void setSpec(const juce::dsp::ProcessSpec& spec);

private:
    // Init the process block according to the apvts
    void initProcessBlock(juce::AudioProcessorValueTreeState& apvts);

    // Spec
    juce::dsp::ProcessSpec spec;

    // The max of sound cards sample rate per one second of audio
    static constexpr auto effectDelaySamples = 192000;

    // Tht linear Delay Line
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> linear{ effectDelaySamples };

    // Dry/Wet Mixer
    juce::dsp::DryWetMixer<float> mixer;

    // Delay values for Rate
    std::array<float, 2> delayValue{ {} };

    // This is for the feedback loop
    std::array<float, 2> lastDelayOutput;

    // Delay feedback volume
    std::array<juce::LinearSmoothedValue<float>, 2> delayFeedBackVolume;
};

