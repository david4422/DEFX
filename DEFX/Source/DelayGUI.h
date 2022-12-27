/*
  ==============================================================================

    DelayGUI.h
    Created: 28 Apr 2022 1:41:47pm
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
class DelayGUI  : public juce::Component
{
public:
    DelayGUI(DEFXAudioProcessor& p);
    ~DelayGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Audio Processor
    DEFXAudioProcessor& audioProcessor;

    // Sliders
    RotarySliderWithLabels feedbackSlider;
    RotarySliderWithLabels rateSlider;
    RotarySliderWithLabels mixSlider;

    // Sliders Labels
    juce::Label feedbackLabel{ "Feedback" ,"Feedback" };
    juce::Label rateLabel{ "Rate", "Rate" };
    juce::Label mixLabel{ "Mix" ,"Mix" };

    // Attachments
    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment feedbackSliderAttachment;
    Attachment rateSliderAttachment;
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayGUI)

};
