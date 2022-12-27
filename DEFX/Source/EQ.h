/*
  ==============================================================================

    EQ.h
    Created: 17 Apr 2022 4:16:51pm
    Author:  david

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "EQFilters.h"

class EQ
{
public:
	EQ();
	~EQ();

    void prepareToPlay(const juce::dsp::ProcessSpec& spec, juce::AudioProcessorValueTreeState& apvts);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::AudioProcessorValueTreeState& apvts);

    // Filters Update Functions
    void updateFilters(juce::AudioProcessorValueTreeState& apvts);

    // Spec
    const juce::dsp::ProcessSpec& getSpec();
    void setSpec(const juce::dsp::ProcessSpec& spec);

   
private:
    juce::dsp::ProcessSpec spec;

    // Filter Chains
    MonoChain leftChain, rightChain;

    // Filters Update Functions
    void updateLowCutFilters(const ChainSettings& chainSettings);
    void updatePeakFilter(const ChainSettings& chainSettings);
    void updateHighCutFilters(const ChainSettings& chainSettings);
};


