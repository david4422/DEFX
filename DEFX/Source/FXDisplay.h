/*
  ==============================================================================

    FXDisplay.h
    Created: 17 Apr 2022 4:20:40pm
    Author:  david

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "EQGUI.h"
#include "DelayGUI.h"
#include "ReverbGUI.h"

//==============================================================================
/*
*/
class FXDisplay  : public juce::Component
{
public:
    FXDisplay(DEFXAudioProcessor& p);
    ~FXDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    // For constructor
    void setGUIVisible();

private:
    // Audio Processor
    DEFXAudioProcessor& audioProcessor;

    // FX GUI
    EQGUI eqGUI{ audioProcessor };
    DelayGUI delayGUI{ audioProcessor };
    ReverbGUI reverbGUI{ audioProcessor };

    //Getting the Components
    std::vector<juce::Component*> getComponents();


    // For resize
    void drawGUI();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FXDisplay)
};
