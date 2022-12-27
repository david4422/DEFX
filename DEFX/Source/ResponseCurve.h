/*
  ==============================================================================

    ResponseCurve.h
    Created: 3 Mar 2022 5:18:50pm
    Author:  david

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//****************************************

struct ResponseCurveComponent : juce::Component,
    juce::AudioProcessorParameter::Listener,
    juce::Timer
{
    ResponseCurveComponent(DEFXAudioProcessor&);
    ~ResponseCurveComponent();

    // Callbacks for the juce::AudioProcessorParameter::Listener 
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}

    // Callback for the juce::Timer
    void timerCallback() override;

    // Paint
    void paint(juce::Graphics& g) override;

    // Resize
    void resized() override;

private:
    // Refence to the processor which created this editor
    DEFXAudioProcessor& audioProcessor;

    // Atomic flag for listening if there was a change in the audio processor parameters
    juce::Atomic<bool> parametersChanged{ false };

    // Mono Chain: values from apvts to draw the response curve
    MonoChain monoChain;

    void updateMonoChain();

    // Grid
    juce::Image background;

    juce::Rectangle<int> getRendrArea();
    juce::Rectangle<int> getAnalysisArea();

    void calculateMagnitudes(std::vector<double>& magnitudes); // Magnitudes
    void paintResponseCurve(juce::Graphics& g, std::vector<double>& magnitudes); // Response Curve

    void drawVerticalGrid(juce::Graphics& g);
    void drawHorizentalGrid(juce::Graphics& g);
};




