/*
  ==============================================================================

    Pedal.h
    Created: 17 Apr 2022 4:19:31pm
    Author:  david

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GUIComponents.h"
#include "FXDisplay.h"
#include "PedalsEnum.h"

//==============================================================================
/*
*/
class Pedal  : public juce::Component
{
public:
    Pedal(juce::String title, juce::AudioProcessorValueTreeState& apvts, FXDisplay& fxDisplay);
    ~Pedal() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // APVTS
    juce::AudioProcessorValueTreeState& apvts;

    // FXdisplay
    FXDisplay& fxDisplay;

    // Title
    juce::String title;

    // Pedals Enum
    juce::String fromPedalsEnumToJuceString(PedalsEnum pedalsEnum);
    PedalsEnum fromJuceStringToPedalsEnum( juce::String juceString);

    // Buttons
    PowerButton powerButton;
    GButton displayButton;

    // Buttons Attachments
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    ButtonAttachment powerButtonAttachment{ apvts, title.toUpperCase() + "_ENABLED" , powerButton };
    ButtonAttachment displayButtonAttachment{ apvts, title.toUpperCase() + "_DISPLAY_ENABLED" , displayButton };


    //Getting the Components
    std::vector<juce::Component*> getComponents();

    // For constructor
    void setGUIVisible();
    //void setGUIAttachment();
    void enableDisableGUI();

    // For resize
    void drawGUI();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pedal)
};
