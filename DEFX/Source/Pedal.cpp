/*
  ==============================================================================

    Pedal.cpp
    Created: 17 Apr 2022 4:19:31pm
    Author:  david

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Pedal.h"


//==============================================================================
Pedal::Pedal(juce::String title, juce::AudioProcessorValueTreeState& apvts, FXDisplay& fxDisplay)
    : title(title) , 
    apvts(apvts),
    fxDisplay(fxDisplay)
{
    // Set the buttons visible
    setGUIVisible();
    // Enable/Disable pedals parameters display
    enableDisableGUI();
}

Pedal::~Pedal()
{
}

void Pedal::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::black);
    float fontSize = 25.0f;
    g.setFont (fontSize);
    g.drawText (title, 0,10,getWidth(), fontSize,juce::Justification::centred, true);   // draw some placeholder text
}

void Pedal::resized()
{
    drawGUI();
}

//Enum to String
juce::String Pedal::fromPedalsEnumToJuceString(PedalsEnum pedalsEnum)
{
    switch (pedalsEnum)
    {
    case PedalsEnum::begin:
        return "";
    case PedalsEnum::EQ:
        return "EQ";
    case PedalsEnum::DELAY:
        return "DELAY";
    case PedalsEnum::REVERB:
        return "REVERB";
    case PedalsEnum::end:
        return "";
    default:
        return "";
    }
}

// String to enum
PedalsEnum Pedal::fromJuceStringToPedalsEnum(juce::String juceString)
{
    if (juceString.toUpperCase() == "EQ")
        return PedalsEnum::EQ;
    if (juceString.toUpperCase() == "DELAY")
        return PedalsEnum::DELAY;
    if (juceString.toUpperCase() == "REVERB")
        return PedalsEnum::REVERB;
    else return PedalsEnum::end;
}

std::vector<juce::Component*> Pedal::getComponents()
{
    return
    {
        &powerButton,
        &displayButton
    };
}

void Pedal::setGUIVisible()
{
    // Gets all the GUI component and make them visible
    for (auto* component : getComponents())
    {
        addAndMakeVisible(component);
    }
}

void Pedal::enableDisableGUI()
{
    // Enacble / Disabled stuff according to the toggle buttons
    // Safe pointer to the pedal
    auto safePtr = juce::Component::SafePointer<Pedal>(this);

    // On Display button click
    displayButton.onClick = [this]()
    {
        // For the FX display
        int nextFXToDisplay = (int)fromJuceStringToPedalsEnum(title);
        // Choose the current value for CURRENT_VISIBLE_FX according the title
        juce::Value currentFXToDisplay = apvts.getParameterAsValue("CURRENT_VISIBLE_FX");
        currentFXToDisplay = nextFXToDisplay;

        //FXDisplay
        fxDisplay.setGUIVisible();
    };
}

void Pedal::drawGUI()
{
    // The pedal size 200x200
    // Power button
    juce::Rectangle<int> powerButtonRect(getWidth() / 4 * 1 - 25, getHeight() / 4 * 2 - 25, 50, 50);
    powerButton.setBounds(powerButtonRect);

    // display button
    juce::Rectangle<int> displayButtonRect(getWidth() / 4 * 3 - 25, getHeight() / 4 * 2 - 25, 50, 50);
    displayButton.setBounds(displayButtonRect);
}

