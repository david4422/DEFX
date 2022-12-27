/*
  ==============================================================================

    Reverb.h
    Created: 28 Apr 2022 1:40:15pm
    Author:  david

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Reverb
{
public:
	Reverb();
	~Reverb();

    void prepareToPlay(const juce::dsp::ProcessSpec& spec);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::AudioProcessorValueTreeState& apvts);

    // Spec
    const juce::dsp::ProcessSpec& getSpec();
    void setSpec(const juce::dsp::ProcessSpec& spec);

private:
    // Spec
    juce::dsp::ProcessSpec spec;

    // Reverb
    juce::dsp::Reverb::Parameters params;
    juce::dsp::Reverb leftReverb;
    juce::dsp::Reverb rightReverb;
};

