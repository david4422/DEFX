/*
  ==============================================================================

    PeadalBoard.cpp
    Created: 17 Apr 2022 4:20:06pm
    Author:  david

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PeadalBoard.h"

//==============================================================================
PeadalBoard::PeadalBoard(juce::AudioProcessorValueTreeState& apvts, FXDisplay& fxDisplay)
    :apvts(apvts),
    fxDisplay(fxDisplay)
{
    // GUI visible
    setGUIVisible();
    // Size
    setSize(600, 200);
}

PeadalBoard::~PeadalBoard()
{
}

void PeadalBoard::paint (juce::Graphics& g)
{
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

  
}

void PeadalBoard::resized()
{
    drawGUI();
}

std::vector<juce::Component*> PeadalBoard::getComponents()
{
    return
    {
        &eqPedal,
        &delayPedal,
        &reverbPedal
    };
}

void PeadalBoard::setGUIVisible()
{
    // Gets all the GUI component and make them visible
    for (auto* component : getComponents())
    {
        addAndMakeVisible(component);
    }
}

void PeadalBoard::drawGUI()
{
    // EQ pedal
    juce::Rectangle<int> eqPedalRect(getWidth() / 3 * 0, 3 , getWidth() / 3 - 3, getHeight() - 6);
    eqPedal.setBounds(eqPedalRect);

    // Delay pedal
    juce::Rectangle<int> dalayPedalRect(getWidth() / 3 * 1, 3, getWidth() / 3 - 3, getHeight() - 6);
    delayPedal.setBounds(dalayPedalRect);

    // Reverb pedal
    juce::Rectangle<int> reverbPedalRect(getWidth() / 3 * 2, 3, getWidth() / 3 - 3, getHeight() - 6);
    reverbPedal.setBounds(reverbPedalRect);

   ;
}
