/*
  ==============================================================================

    EQFiltersDefinitions.h
    Created: 11 Apr 2022 12:40:12pm
    Author:  david

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//**************************

// Slope Options
enum class Slope
{
    Slope_12,
    Slope_24,
    Slope_36,
    Slope_48
};

// Saves parameters' values
struct ChainSettings
{
    float lowCutFreq{ 0.f };
    Slope lowCutSlope{ Slope::Slope_12 };
    bool lowCutBypassed{ false };

    float peakFreq{ 0.f };
    float peakGainDB{ 0.f };
    float peakQuality{ 1.f };
    bool peakBypassed{ false };

    float highCutFreq{ 0.f };
    Slope highCutSlope{ Slope::Slope_12 };
    bool highCutBypassed{ false };
};

// Filter Chains
using Filter = juce::dsp::IIR::Filter<float>;

using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>; // 12, 24, 36, 48 Filter Slope

using MonoChain = juce::dsp::ProcessorChain<CutFilter, Filter, CutFilter>; //LowCut, Peak, HighCut

// Enum for the index in the MonoChain
enum ChainPositions { LowCut, Peak, HighCut };

// Coefficients
using Coefficients = Filter::CoefficientsPtr;
