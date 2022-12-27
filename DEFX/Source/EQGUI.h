/*
  ==============================================================================

    EQGUI.h
    Created: 17 Apr 2022 4:17:51pm
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
class EQGUI  : public juce::Component
{
public:
    EQGUI(DEFXAudioProcessor& p);
    ~EQGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    DEFXAudioProcessor& audioProcessor;

    // The Sliders
    RotarySliderWithLabels lowCutFreqSlider;
    RotarySliderWithLabels lowCutSlopeSlider;
    RotarySliderWithLabels peakFreqSlider;
    RotarySliderWithLabels peakGainSlider;
    RotarySliderWithLabels peakQualitySlider;
    RotarySliderWithLabels highCutFreqSlider;
    RotarySliderWithLabels highCutSlopeSlider;

    // Slider Labels 
    juce::Label lowcutFreqLabel{ "Lowcut Freq", "Lowcut Freq" };
    juce::Label lowcutSlopeLabel{ "Lowcut Slope", "Lowcut Slope" };
    juce::Label peakFreqLabel{ "Peak Freq", "Peak Freq" };
    juce::Label peakGainLabel{ "Peak Gain", "Peak Gain" };
    juce::Label peakQualityLabel{ "Peak Quality", "Peak Quality" };
    juce::Label highcutFreqLabel{ "Highcut Freq", "Highcut Freq" };
    juce::Label highcutSlopeLabel{ "Highcut Slope", "Highcut Slope" };
    

    // Buttons
    PowerButton lowcutEnabledButton;
    PowerButton peakEnabledButton;
    PowerButton highcutEnabledButton;

    // Attachments
    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment lowCutFreqAttachment;
    Attachment lowCutSlopeAttachment;
    Attachment peakFreqAttachment;
    Attachment peakGainAttachment;
    Attachment peakQualityAttachment;
    Attachment highCutFreqAttachment;
    Attachment highCutSlopeAttachment;

    // Button attachment
    using ButtonAttachment = APVTS::ButtonAttachment;
    ButtonAttachment lowcutEnabledButtonAttachment;
    ButtonAttachment peakEnabledButtonAttachment;
    ButtonAttachment highcutEnabledButtonAttachment;

    //Getting the Components
    std::vector<juce::Component*> getComponents();

    // For constructor
    void setSlidersMinMaxLables();
    void setGUIVisible();
    void enableDisableFilters();

    // Justification to the center of the rect
    void setLabelsJustification();
    // Set Label visible
    void setLabelsVisible();


    // For resize
    void drawGUI();
    void drawLabels();
    void drawButtons();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQGUI)
};
