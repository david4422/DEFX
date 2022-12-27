/*
  ==============================================================================

    EQFilters.cpp
    Created: 3 Mar 2022 5:18:04pm
    Author:  david

  ==============================================================================
*/

#include "EQFilters.h"
//******************

// Creates a ChainSettings object
// Loads the parmeters from the layout to the chain settings
ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts)
{
    ChainSettings settings;

    // From the layout to the settings
    settings.lowCutFreq = apvts.getRawParameterValue("EQ_LOW_CUT_FREQ")->load();
    settings.lowCutSlope = static_cast<Slope>(apvts.getRawParameterValue("EQ_LOW_CUT_SLOPE")->load());
    settings.lowCutBypassed = !apvts.getRawParameterValue("EQ_LOW_CUT_ENABLED")->load();

    settings.peakFreq = apvts.getRawParameterValue("EQ_PEAK_FREQ")->load();
    settings.peakGainDB = apvts.getRawParameterValue("EQ_PEAK_GAIN")->load();
    settings.peakQuality = apvts.getRawParameterValue("EQ_PEAK_QUALITY")->load();
    settings.peakBypassed = !apvts.getRawParameterValue("EQ_PEAK_ENABLED")->load();

    settings.highCutFreq = apvts.getRawParameterValue("EQ_HIGH_CUT_FREQ")->load();
    settings.highCutSlope = static_cast<Slope>(apvts.getRawParameterValue("EQ_HIGH_CUT_SLOPE")->load());
    settings.highCutBypassed = !apvts.getRawParameterValue("EQ_HIGH_CUT_ENABLED")->load();

    return settings;
}

//******************

void updateCoefficients(Coefficients& old, const Coefficients& replacements)
{
    *old = *replacements;
}

//******************

juce::ReferenceCountedArray<juce::dsp::IIR::Coefficients<float>> makeLowCutFilter(const ChainSettings& chainSettings, double sampleRate)
{
    return juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(chainSettings.lowCutFreq,
        sampleRate,
        2 * (int(chainSettings.lowCutSlope) + 1));
}

Coefficients makePeakFilter(const ChainSettings& chainSettings, double sampleRate)
{
    return juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate,
        chainSettings.peakFreq,
        chainSettings.peakQuality,
        juce::Decibels::decibelsToGain(chainSettings.peakGainDB));
}

juce::ReferenceCountedArray<juce::dsp::IIR::Coefficients<float>> makeHighCutFilter(const ChainSettings& chainSettings, double sampleRate)
{
    return juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(chainSettings.highCutFreq,
        sampleRate,
        2 * (int(chainSettings.highCutSlope) + 1));
}

//******************
