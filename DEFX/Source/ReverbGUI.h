/*
  ==============================================================================

    ReverbGUI.h
    Created: 28 Apr 2022 1:41:11pm
    Author:  david

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUIComponents.h"

//==============================================================================
/*
*/
class ReverbGUI  : public juce::Component
{
public:
    ReverbGUI(DEFXAudioProcessor& p);
    ~ReverbGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Audio Processor
    DEFXAudioProcessor& audioProcessor;

    // Sliders
    RotarySliderWithLabels roomSizeSlider;
    RotarySliderWithLabels dampingSlider;
    RotarySliderWithLabels widthSlider;
    RotarySliderWithLabels freezeModeSlider;
    RotarySliderWithLabels mixSlider;

    // Slider Labels
    juce::Label roomSizeLabel{ "Room Size", "Room Size" };
    juce::Label dampingLabel{ "Damping", "Damping" };
    juce::Label widthLabel{ "Width", "Width" };
    juce::Label freezeModeLabel{ "FreezeModeS", "FreezeMode" };
    juce::Label mixLabel{ "Mix", "Mix" };

    // Attachments
    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment roomSizeSliderAttachment;
    Attachment dampingSliderAttachment;
    Attachment widthSliderAttachment;
    Attachment freezeModeSliderAttachment;
    Attachment mixSliderAttachment;

    //Getting the Components
    std::vector<juce::Component*> getComponents();

    // For constructor
    void setSlidersMinMaxLables();
    void setGUIVisible();

    // Justification to the center of the rect
    void setLabelsJustification();
    // Set Label visible
    void setLabelsVisible();

    // For resize
    void drawGUI();
    void drawLabels();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbGUI)
};
