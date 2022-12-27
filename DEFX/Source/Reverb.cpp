/*
  ==============================================================================

    Reverb.cpp
    Created: 28 Apr 2022 1:40:15pm
    Author:  david

  ==============================================================================
*/

#include "Reverb.h"

Reverb::Reverb()
{
}

Reverb::~Reverb()
{
}

void Reverb::prepareToPlay(const juce::dsp::ProcessSpec& spec)
{
    setSpec(spec);
}

void Reverb::processBlock(juce::AudioBuffer<float>& buffer, juce::AudioProcessorValueTreeState& apvts)
{
    // Update the reverb parameters
    params.roomSize = *apvts.getRawParameterValue("REVERB_ROOM_SIZE");
    params.damping = *apvts.getRawParameterValue("REVERB_DAMPING");
    params.width = *apvts.getRawParameterValue("REVERB_WIDTH");
    params.freezeMode = *apvts.getRawParameterValue("REVERB_FREEZE_MODE");
    params.wetLevel = *apvts.getRawParameterValue("REVERB_MIX");
    params.dryLevel = 1.0 - *apvts.getRawParameterValue("REVERB_MIX");

    // Set the updated parameters for each reverb
    leftReverb.setParameters(params);
    rightReverb.setParameters(params);

    // Make context for the processing
    // Getting the left and right channels from the buffer
    juce::dsp::AudioBlock<float> block(buffer);

    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);

    // Creating Processing Context
    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    // Process
    leftReverb.process(leftContext);
    rightReverb.process(rightContext);
}

const juce::dsp::ProcessSpec& Reverb::getSpec()
{
    return spec;
}


// Reverb (left and right) needs spec
void Reverb::setSpec(const juce::dsp::ProcessSpec& spec)
{
    this->spec.maximumBlockSize = spec.maximumBlockSize;
    this->spec.numChannels = spec.numChannels;
    this->spec.sampleRate = spec.sampleRate;

    leftReverb.prepare(this->spec);
    rightReverb.prepare(this->spec);
}
