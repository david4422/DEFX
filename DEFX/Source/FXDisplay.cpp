/*
  ==============================================================================

    FXDisplay.cpp
    Created: 17 Apr 2022 4:20:40pm
    Author:  david

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FXDisplay.h"

//==============================================================================
FXDisplay::FXDisplay(DEFXAudioProcessor& p): audioProcessor(p)
{
    setGUIVisible();

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    // Set the Window Size
    setSize(600, 400);

}

FXDisplay::~FXDisplay()
{
}

void FXDisplay::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void FXDisplay::resized()
{
    drawGUI();
}

std::vector<juce::Component*> FXDisplay::getComponents()
{
    return
    {
        // EQ
        &eqGUI,

        // Delay
        &delayGUI,

        // Reverb
        &reverbGUI
    };
}


void FXDisplay::setGUIVisible()
{
    // Gets all the GUI component and make them visible
    std::vector<juce::Component*> components = getComponents();
    int currentVisibleFX = *audioProcessor.apvts.getRawParameterValue("CURRENT_VISIBLE_FX");
    for (size_t i = 0; i < components.size(); i++)
    {
        addAndMakeVisible(components[i]);
        if (i == currentVisibleFX)
            components[i]->setVisible(true);
        else
            components[i]->setVisible(false);
    }
}


void FXDisplay::drawGUI()
{
    juce::Rectangle<int> bounds = getLocalBounds();

    // EQ
    eqGUI.setBounds(bounds);

    // Dealay
    delayGUI.setBounds(bounds);

    // Reverb
    reverbGUI.setBounds(bounds);

}
