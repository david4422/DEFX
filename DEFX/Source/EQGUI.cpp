/*
  ==============================================================================

    EQGUI.cpp
    Created: 17 Apr 2022 4:17:51pm
    Author:  david

  ==============================================================================
*/

#include <JuceHeader.h>
#include "EQGUI.h"

//==============================================================================
EQGUI::EQGUI(DEFXAudioProcessor& p) : audioProcessor(p), 
// Sliders Presentation and Units
lowCutFreqSlider(*audioProcessor.apvts.getParameter("EQ_LOW_CUT_FREQ"), "Hz"),
lowCutSlopeSlider(*audioProcessor.apvts.getParameter("EQ_LOW_CUT_SLOPE"), "dB/Oct"),
peakFreqSlider(*audioProcessor.apvts.getParameter("EQ_PEAK_FREQ"), "Hz"),
peakGainSlider(*audioProcessor.apvts.getParameter("EQ_PEAK_GAIN"), "dB"),
peakQualitySlider(*audioProcessor.apvts.getParameter("EQ_PEAK_QUALITY"), ""),
highCutFreqSlider(*audioProcessor.apvts.getParameter("EQ_HIGH_CUT_FREQ"), "Hz"),
highCutSlopeSlider(*audioProcessor.apvts.getParameter("EQ_HIGH_CUT_SLOPE"), "dB/Oct"),
// Sliders State Attachments
lowCutFreqAttachment(audioProcessor.apvts, "EQ_LOW_CUT_FREQ", lowCutFreqSlider),
lowCutSlopeAttachment(audioProcessor.apvts, "EQ_LOW_CUT_SLOPE", lowCutSlopeSlider),
peakFreqAttachment(audioProcessor.apvts, "EQ_PEAK_FREQ", peakFreqSlider),
peakGainAttachment(audioProcessor.apvts, "EQ_PEAK_GAIN", peakGainSlider),
peakQualityAttachment(audioProcessor.apvts, "EQ_PEAK_QUALITY", peakQualitySlider),
highCutFreqAttachment(audioProcessor.apvts, "EQ_HIGH_CUT_FREQ", highCutFreqSlider),
highCutSlopeAttachment(audioProcessor.apvts, "EQ_HIGH_CUT_SLOPE", highCutSlopeSlider),
// Toggle buttons state attachment
lowcutEnabledButtonAttachment(audioProcessor.apvts, "EQ_LOW_CUT_ENABLED", lowcutEnabledButton),
peakEnabledButtonAttachment(audioProcessor.apvts, "EQ_PEAK_ENABLED", peakEnabledButton),
highcutEnabledButtonAttachment(audioProcessor.apvts, "EQ_HIGH_CUT_ENABLED", highcutEnabledButton)
{
    // Min Max Labels
    setSlidersMinMaxLables();

    // Gets all the sliders and make them visible
    setGUIVisible();

    // Labels
    setLabelsJustification();
    setLabelsVisible();

    // Enable/disable Filters
    enableDisableFilters();

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    // Set the Window Size
    setSize(600, 400);
}

EQGUI::~EQGUI()
{
}

void EQGUI::paint (juce::Graphics& g)
{
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void EQGUI::resized()
{
    drawGUI();
    drawLabels();
    drawButtons();
}

std::vector<juce::Component*> EQGUI::getComponents()
{
    return
    {  
        // Low Cut
        &lowCutFreqSlider,
        &lowCutSlopeSlider,
        &lowcutEnabledButton,

        // Peak
        &peakFreqSlider,
        &peakGainSlider,
        &peakQualitySlider,
        &peakEnabledButton,

        // High Cut
        &highCutFreqSlider,
        &highCutSlopeSlider,
        &highcutEnabledButton
    };
}

void EQGUI::setSlidersMinMaxLables()
{
    // Min Max Labels
    lowCutFreqSlider.labels.add({ 0.f,"20Hz" });
    lowCutFreqSlider.labels.add({ 1.f,"20kHz" });
    lowCutSlopeSlider.labels.add({ 0.f,"12dB/Oct" });
    lowCutSlopeSlider.labels.add({ 1.f,"48dB/Oct" });

    peakFreqSlider.labels.add({ 0.f,"20Hz" });
    peakFreqSlider.labels.add({ 1.f,"20kHz" });
    peakGainSlider.labels.add({ 0.f,"-24dB" });
    peakGainSlider.labels.add({ 1.f,"24dB" });
    peakQualitySlider.labels.add({ 0.f,"0.1" });
    peakQualitySlider.labels.add({ 1.f,"10.0" });

    highCutFreqSlider.labels.add({ 0.f,"20Hz" });
    highCutFreqSlider.labels.add({ 1.f,"20kHz" });
    highCutSlopeSlider.labels.add({ 0.f,"12dB/Oct" });
    highCutSlopeSlider.labels.add({ 1.f,"48dB/Oct" });
}

void EQGUI::setGUIVisible()
{
    // Gets all the sliders and make them visible
    for (auto* component : getComponents())
    {
        addAndMakeVisible(component);
    }
}

void EQGUI::enableDisableFilters()
{
    // Enable/disable stuff according to the toggle buttons 
    auto safePtr = juce::Component::SafePointer<EQGUI>(this);

    // Enable/disable lowcut sliders
    lowcutEnabledButton.onClick = [safePtr]()
    {
        if (auto* component = safePtr.getComponent())
        {
            bool enabled = component->lowcutEnabledButton.getToggleState();

            component->lowCutFreqSlider.setEnabled(enabled);
            component->lowCutSlopeSlider.setEnabled(enabled);
        }
    };

    // For the  plugin restart
    lowCutFreqSlider.setEnabled(lowcutEnabledButton.getToggleState());
    lowCutSlopeSlider.setEnabled(lowcutEnabledButton.getToggleState());

    // Enable/disable peak sliders
    peakEnabledButton.onClick = [safePtr]()
    {
        if (auto* component = safePtr.getComponent())
        {
            bool enabled = component->peakEnabledButton.getToggleState();

            component->peakFreqSlider.setEnabled(enabled);
            component->peakGainSlider.setEnabled(enabled);
            component->peakQualitySlider.setEnabled(enabled);

        }
    };

    // For the  plugin restart
    peakFreqSlider.setEnabled(peakEnabledButton.getToggleState());
    peakGainSlider.setEnabled(peakEnabledButton.getToggleState());
    peakQualitySlider.setEnabled(peakEnabledButton.getToggleState());

    // Enable/disable highcut sliders
    highcutEnabledButton.onClick = [safePtr]()
    {
        if (auto* component = safePtr.getComponent())
        {
            bool enabled = component->highcutEnabledButton.getToggleState();

            component->highCutFreqSlider.setEnabled(enabled);
            component->highCutSlopeSlider.setEnabled(enabled);
        }
    };

    // For the  plugin restart
    highCutFreqSlider.setEnabled(highcutEnabledButton.getToggleState());
    highCutSlopeSlider.setEnabled(highcutEnabledButton.getToggleState());
}

void EQGUI::setLabelsJustification()
{
    lowcutFreqLabel.setJustificationType(juce::Justification::centred);
    lowcutSlopeLabel.setJustificationType(juce::Justification::centred);
    peakFreqLabel.setJustificationType(juce::Justification::centred);
    peakGainLabel.setJustificationType(juce::Justification::centred);
    peakQualityLabel.setJustificationType(juce::Justification::centred);
    highcutFreqLabel.setJustificationType(juce::Justification::centred);
    highcutSlopeLabel.setJustificationType(juce::Justification::centred);
}

void EQGUI::setLabelsVisible()
{
    addAndMakeVisible(lowcutFreqLabel);
    addAndMakeVisible(lowcutSlopeLabel);
    addAndMakeVisible(peakFreqLabel);
    addAndMakeVisible(peakGainLabel);
    addAndMakeVisible(peakQualityLabel);
    addAndMakeVisible(highcutFreqLabel);
    addAndMakeVisible(highcutSlopeLabel);
}

void EQGUI::drawGUI()
{
    // Low Cut
    juce::Rectangle<int> lowCutFreqSliderRect(getWidth() / 6 * 1 - 75, getHeight() / 4 * 1 - 71, 150, 150);
    lowCutFreqSlider.setBounds(lowCutFreqSliderRect);

    juce::Rectangle<int> lowCutSlopeSliderRect(getWidth() / 6 * 1 - 75, getHeight() / 4 * 3 - 71, 150, 150);
    lowCutSlopeSlider.setBounds(lowCutSlopeSliderRect);

    // Peak
    juce::Rectangle<int> peakFreqSliderRect(getWidth() / 6 * 3 - 50, getHeight() / 4 * 1 - 70, 100, 100);
    peakFreqSlider.setBounds(peakFreqSliderRect);

    juce::Rectangle<int> peakGainSliderRect(getWidth() / 6 * 3 - 50, getHeight() / 4 * 2 - 40, 100, 100);
    peakGainSlider.setBounds(peakGainSliderRect);

    juce::Rectangle<int> peakQualitySliderRect(getWidth() / 6 * 3 - 50, getHeight() / 4 * 3 - 10, 100, 100);
    peakQualitySlider.setBounds(peakQualitySliderRect);
    
    // High Cut
    juce::Rectangle<int> highCutFreqSliderRect(getWidth() / 6 * 5 - 75, getHeight() / 4 * 1 - 71, 150, 150);
    highCutFreqSlider.setBounds(highCutFreqSliderRect);

    juce::Rectangle<int> highCutSlopeSliderRect(getWidth() / 6 * 5 - 75, getHeight() / 4 * 3 - 71, 150, 150);
    highCutSlopeSlider.setBounds(highCutSlopeSliderRect);
}

void EQGUI::drawLabels()
{
    int fontSize = 18;

    // Low cut Label
    lowcutFreqLabel.setFont(fontSize);
    lowcutFreqLabel.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    juce::Rectangle<int> lowcutFreqLabelRect(getWidth() / 6 * 1 - 75, getHeight() / 4 * 1 - 71 - fontSize, 150, fontSize);
    lowcutFreqLabel.setBounds(lowcutFreqLabelRect);

    lowcutSlopeLabel.setFont(fontSize);
    lowcutSlopeLabel.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    juce::Rectangle<int> lowcutSlopeLabelRect(getWidth() / 6 * 1 - 75, getHeight() / 4 * 3 - 71 - fontSize, 150, fontSize);
    lowcutSlopeLabel.setBounds(lowcutSlopeLabelRect);


    // Peak Label
    peakFreqLabel.setFont(fontSize);
    peakFreqLabel.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    juce::Rectangle<int> peakFreqLabelRect(getWidth() / 6 * 3 - 75, getHeight() / 4 * 1 - 70 - fontSize, 150, fontSize);
    peakFreqLabel.setBounds(peakFreqLabelRect);

    peakGainLabel.setFont(fontSize);
    peakGainLabel.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    juce::Rectangle<int> peakGainLabelRect(getWidth() / 6 * 3 - 75, getHeight() / 4 * 2 - 40 - fontSize, 150, fontSize);
    peakGainLabel.setBounds(peakGainLabelRect);

    peakQualityLabel.setFont(fontSize);
    peakQualityLabel.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    juce::Rectangle<int> peakQualityLabelRect(getWidth() / 6 * 3 - 75, getHeight() / 4 * 3 - 10 - fontSize, 150, fontSize);
    peakQualityLabel.setBounds(peakQualityLabelRect);

    // High cut Label
    highcutFreqLabel.setFont(fontSize);
    highcutFreqLabel.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    juce::Rectangle<int> highcutFreqLabelRect(getWidth() / 6 * 5 - 75, getHeight() / 4 * 1 - 71 - fontSize, 150, fontSize);
    highcutFreqLabel.setBounds(highcutFreqLabelRect);

    highcutSlopeLabel.setFont(fontSize);
    highcutSlopeLabel.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    juce::Rectangle<int> highcutSlopeLabelRect(getWidth() / 6 * 5 - 75, getHeight() / 4 * 3 - 71 - fontSize, 150, fontSize);
    highcutSlopeLabel.setBounds(highcutSlopeLabelRect);

}

// Draw the byPasses Button
void EQGUI::drawButtons()
{
    int buttonSize = 25;

    // Low
    juce::Rectangle<int> lowcutBypassedButtonRect(getWidth() / 6 * 1 - 75, getHeight() / 4 * 1 - 71 , buttonSize, buttonSize);
    lowcutEnabledButton.setBounds(lowcutBypassedButtonRect);

    // Peak
    juce::Rectangle<int> peakBypassedButtonRect(getWidth() / 6 * 3 - 75, getHeight() / 4 * 1 - 71 , buttonSize, buttonSize);
    peakEnabledButton.setBounds(peakBypassedButtonRect);

    // High
    juce::Rectangle<int> highcutBypassedButtonRect(getWidth() / 6 * 5 - 75, getHeight() / 4 * 1 - 71 , buttonSize, buttonSize);
    highcutEnabledButton.setBounds(highcutBypassedButtonRect);

}
