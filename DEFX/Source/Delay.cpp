/*
  ==============================================================================

    Delay.cpp
    Created: 28 Apr 2022 1:41:27pm
    Author:  david

  ==============================================================================
*/

#include "Delay.h"

Delay::Delay()
{
}

Delay::~Delay()
{
}

void Delay::prepareToPlay(const juce::dsp::ProcessSpec& spec, juce::AudioProcessorValueTreeState& apvts)
{
    // Inits:
    
    // Spec
    setSpec(spec);
    
    // Init feedback volume according to the apvts parameter
    float newFeedback = *apvts.getRawParameterValue("DELAY_FEEDBACK");
    const auto feedbackGain = juce::Decibels::decibelsToGain(newFeedback, -100.f);
    for (auto& volume : delayFeedBackVolume)
        volume.reset(spec.sampleRate, feedbackGain);

    // Init delay line
    linear.reset();

    // At the first time there will be no last delay output
    std::fill(lastDelayOutput.begin(), lastDelayOutput.end(), 0.f);
}

void Delay::processBlock(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, int totalNumOutputChannels, juce::AudioProcessorValueTreeState& apvts)
{
    // Init
    initProcessBlock(apvts);

    // Creat input and output contextes

    // checking for the max use channels amount
    const auto numChannels = juce::jmax(totalNumInputChannels, totalNumOutputChannels);

    // Creating the audio block from the buffer (including all its channels)
    auto audioBlock = juce::dsp::AudioBlock<float>(buffer).getSubsetChannelBlock(0, (size_t)numChannels);

    // Creating the contextes for input & output
    // For the feedback loop
    auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);
    const juce::dsp::AudioBlock<const float>& input = context.getInputBlock();
    const juce::dsp::AudioBlock<float>& output = context.getOutputBlock();

    // Dry signal from input to the mixer
    // The wet part is at the end of the function
    mixer.pushDrySamples(input);

    // The delay feedback loop
    // For each channel
    for (size_t channel = 0; channel < numChannels; ++channel)
    {
        // Refering the channel in both input and output block
        auto* samplesIn = input.getChannelPointer(channel);
        auto* samplesOut = output.getChannelPointer(channel);

        // For each input sample
        for (size_t sample = 0; sample < input.getNumSamples(); ++sample)
        {
            // This line is feeded back by the line in the end of this loop
            auto inputSample = samplesIn[sample] - lastDelayOutput[channel];

            // Setting the delay rate value
            auto delayAmount = delayValue[channel];

            // Pushing the sample into the delay line
            linear.pushSample(int(channel), inputSample);
            linear.setDelay(float(delayAmount));

            // Feeding it back to the samplesOut
            samplesOut[sample] = linear.popSample(int(channel));

            // This line will feed back the line into the beggining of the loop 
            lastDelayOutput[channel] = samplesOut[sample] * delayFeedBackVolume[channel].getNextValue();
        }
    }
    // Wet signal from the output to the mixer
    // The dry part in the begining of this function
    mixer.mixWetSamples(output);
}

const juce::dsp::ProcessSpec& Delay::getSpec()
{
    return spec;
}
// The linear(delayy line) and the mixer needs spec
void Delay::setSpec(const juce::dsp::ProcessSpec& spec)
{
    this->spec.maximumBlockSize = spec.maximumBlockSize;
    this->spec.numChannels = spec.numChannels;
    this->spec.sampleRate = spec.sampleRate;

    linear.prepare(this->spec);
    mixer.prepare(this->spec);
}

void Delay::initProcessBlock(juce::AudioProcessorValueTreeState& apvts)
{
    // Inits:

    // Feedback Volume
    float newFeedback = *apvts.getRawParameterValue("DELAY_FEEDBACK");
    const auto feedbackGain = juce::Decibels::decibelsToGain(newFeedback, -100.f);
    for (auto& volume : delayFeedBackVolume)
        volume.setTargetValue(feedbackGain);

    // Delay Rate
    float newRate = *apvts.getRawParameterValue("DELAY_RATE");
    std::fill(delayValue.begin(), delayValue.end(), newRate / 1000 * spec.sampleRate);

    // Mixer
    float newMix = *apvts.getRawParameterValue("DELAY_MIX");
    mixer.setWetMixProportion(newMix);
}
