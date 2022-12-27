/*
  ==============================================================================
 
    EQFilters.h
    Created: 3 Mar 2022 5:18:04pm
    Author:  david

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "EQFiltersDefinitions.h"

//**************************

// From apvts to chain setting
ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts);

// Coefficients
void updateCoefficients(Coefficients& old, const Coefficients& replacements);

// Filter update
template<int Index, typename ChainType, typename CoefficientType>
void updateSubCutFilter(ChainType& chain, const CoefficientType& coefficients)
{
    updateCoefficients(chain.template get<Index>().coefficients, coefficients[Index]);
    chain.template setBypassed<Index>(false);
}

template<typename ChainType, typename CoefficientType>
void updateCutFilter(ChainType& chain,
    const CoefficientType& coefficients,
    const Slope& slope)
{
    // ByPassing all the 4 links in the chain (12,24,36,48)
    chain.template setBypassed<0>(true);
    chain.template setBypassed<1>(true);
    chain.template setBypassed<2>(true);
    chain.template setBypassed<3>(true);

    // Un ByPassing all the links according to the Slope DB/OCT choice
    switch (slope)
    {
    case Slope::Slope_48:
        updateSubCutFilter<3>(chain, coefficients);
    case Slope::Slope_36:
        updateSubCutFilter<2>(chain, coefficients);
    case Slope::Slope_24:
        updateSubCutFilter<1>(chain, coefficients);
    case Slope::Slope_12:
        updateSubCutFilter<0>(chain, coefficients);
        break;
    }
}


//**********************************

juce::ReferenceCountedArray<juce::dsp::IIR::Coefficients<float>> makeLowCutFilter(const ChainSettings& chainSettings, double sampleRate);

Coefficients makePeakFilter(const ChainSettings& chainSettings, double sampleRate);

juce::ReferenceCountedArray<juce::dsp::IIR::Coefficients<float>> makeHighCutFilter(const ChainSettings& chainSettings, double sampleRate);

//**************************