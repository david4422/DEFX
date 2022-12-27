/*
  ==============================================================================

    EQ.cpp
    Created: 17 Apr 2022 4:16:51pm
    Author:  david

  ==============================================================================
*/

#include "EQ.h"

EQ::EQ()
{
}

EQ::~EQ()
{
}

void EQ::prepareToPlay(const juce::dsp::ProcessSpec& spec, juce::AudioProcessorValueTreeState& apvts)
{
    setSpec(spec);

    updateFilters(apvts);
}

void EQ::processBlock(juce::AudioBuffer<float>& buffer, juce::AudioProcessorValueTreeState& apvts)
{
    // Update the filters
    updateFilters(apvts);

    // Getting the left and right channels from the buffer
    juce::dsp::AudioBlock<float> block(buffer);

    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);

    // Creating Processing Context
    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    // Sending the Context to the Filters' Chains so the filter can work
    leftChain.process(leftContext);
    rightChain.process(rightContext);

    //**********************
}

void EQ::updateFilters(juce::AudioProcessorValueTreeState& apvts)
{
    auto chainSettings = getChainSettings(apvts);

    updateLowCutFilters(chainSettings);
    updatePeakFilter(chainSettings);
    updateHighCutFilters(chainSettings);
}


void EQ::updateLowCutFilters(const ChainSettings& chainSettings)
{
    // Update the LowCut Filter

    // Create the Coeffocients
    auto lowCutCoefficients = makeLowCutFilter(chainSettings, spec.sampleRate);

    // Update the Left LowCut Filter
    auto& leftLowCut = leftChain.get<ChainPositions::LowCut>();
    leftChain.setBypassed<ChainPositions::LowCut>(chainSettings.lowCutBypassed);
    updateCutFilter(leftLowCut, lowCutCoefficients, chainSettings.lowCutSlope);

    // Update the Right LowCut Filter
    auto& rightLowCut = rightChain.get<ChainPositions::LowCut>();
    rightChain.setBypassed<ChainPositions::LowCut>(chainSettings.lowCutBypassed);
    updateCutFilter(rightLowCut, lowCutCoefficients, chainSettings.lowCutSlope);
}

void EQ::updatePeakFilter(const ChainSettings& chainSettings)
{
    auto peakCoefficients = makePeakFilter(chainSettings, spec.sampleRate);

    leftChain.setBypassed<ChainPositions::Peak>(chainSettings.peakBypassed);
    updateCoefficients(leftChain.get<ChainPositions::Peak>().coefficients, *peakCoefficients);

    rightChain.setBypassed<ChainPositions::Peak>(chainSettings.peakBypassed);
    updateCoefficients(rightChain.get<ChainPositions::Peak>().coefficients, *peakCoefficients);
}

void EQ::updateHighCutFilters(const ChainSettings& chainSettings)
{
    // Update the High Cut Filter

    // Process the HighCut Filter
    auto highCutCoefficients = makeHighCutFilter(chainSettings, spec.sampleRate);

    // Process the Left HighCut Filter
    auto& leftHighCut = leftChain.get<ChainPositions::HighCut>();
    leftChain.setBypassed<ChainPositions::HighCut>(chainSettings.highCutBypassed);
    updateCutFilter(leftHighCut, highCutCoefficients, chainSettings.highCutSlope);

    // Process the right HighCut Filter
    auto& rightHighCut = rightChain.get<ChainPositions::HighCut>();
    rightChain.setBypassed<ChainPositions::HighCut>(chainSettings.highCutBypassed);
    updateCutFilter(rightHighCut, highCutCoefficients, chainSettings.highCutSlope);
}

const juce::dsp::ProcessSpec& EQ::getSpec()
{
    return spec;
}

void EQ::setSpec(const juce::dsp::ProcessSpec& spec)
{
    this->spec.maximumBlockSize = spec.maximumBlockSize;
    this->spec.numChannels = spec.numChannels;
    this->spec.sampleRate = spec.sampleRate;

    // Send the spec to the chain, so it'll prepare
    leftChain.prepare(this->spec);
    rightChain.prepare(this->spec);
}