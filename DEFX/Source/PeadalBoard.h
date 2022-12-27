/*
  ==============================================================================

    PeadalBoard.h
    Created: 17 Apr 2022 4:20:06pm
    Author:  david

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Pedal.h"

//==============================================================================
/*
*/
class PeadalBoard  : public juce::Component
{
public:
    PeadalBoard(juce::AudioProcessorValueTreeState& apvts, FXDisplay& fxDisplay);
    ~PeadalBoard() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // APVTS
    juce::AudioProcessorValueTreeState& apvts;

    // FXdisplay
    FXDisplay& fxDisplay;

    // Pedals
    Pedal eqPedal{ "EQ", apvts, fxDisplay };
    Pedal delayPedal{ "Delay", apvts, fxDisplay };
    Pedal reverbPedal{ "Reverb", apvts, fxDisplay };
    
    //Getting the Components
    std::vector<juce::Component*> getComponents();

    // For constructor
    void setGUIVisible();

    // For resize
    void drawGUI();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PeadalBoard)
};
