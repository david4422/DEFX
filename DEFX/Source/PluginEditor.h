/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUIComponents.h"
#include "ResponseCurve.h"
#include "FXDisplay.h"
#include "PeadalBoard.h"


//==============================================================================
/**
*/
class DEFXAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DEFXAudioProcessorEditor (DEFXAudioProcessor& p); 
    ~DEFXAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DEFXAudioProcessor& audioProcessor;

    // Response Curve Component
    ResponseCurveComponent responseCurveComponent;

    // FX display
    FXDisplay fxDisplay{audioProcessor};

    // Pedal Board
    PeadalBoard pedalBoard{ audioProcessor.apvts ,fxDisplay };
    
    //Getting the Components
    std::vector<juce::Component*> getComponents();

    // For constructor
    void setGUIVisible();
    void setGUIAttachment();
   
    
    // For resize
    void drawGUI();

    //********************************

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DEFXAudioProcessorEditor)
};