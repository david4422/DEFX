/*
  ==============================================================================

    DelayGUI.cpp
    Created: 28 Apr 2022 1:41:47pm
    Author:  david

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DelayGUI.h"

//==============================================================================
DelayGUI::DelayGUI(DEFXAudioProcessor& p) : audioProcessor(p),
// Sliders Presentation and Units
feedbackSlider(*audioProcessor.apvts.getParameter("DELAY_FEEDBACK"), "db"),
rateSlider(*audioProcessor.apvts.getParameter("DELAY_RATE"), "ms"),
mixSlider(*audioProcessor.apvts.getParameter("DELAY_MIX"), "%"),
// Sliders State Attachments
feedbackSliderAttachment(audioProcessor.apvts, "DELAY_FEEDBACK", feedbackSlider),
rateSliderAttachment(audioProcessor.apvts, "DELAY_RATE", rateSlider),
mixSliderAttachment(audioProcessor.apvts, "DELAY_MIX", mixSlider)
{
    // Min Max Labels
   setSlidersMinMaxLables();

    // Gets all the sliders and make them visible
    setGUIVisible();

    // Labels
    setLabelsJustification();
    setLabelsVisible();

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    // Set the Window Size
    setSize(600, 400);
}

DelayGUI::~DelayGUI()
{
}

void DelayGUI::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void DelayGUI::resized()
{
    drawGUI();
    drawLabels();
}

std::vector<juce::Component*> DelayGUI::getComponents()
{
    return
    {
        &feedbackSlider,
        &rateSlider,
        &mixSlider
    };
}

void DelayGUI::setSlidersMinMaxLables()
{
    feedbackSlider.labels.add({ 0.f,"-6db" });
    feedbackSlider.labels.add({ 1.f,"0db" });

    rateSlider.labels.add({ 0.f,"0.48ms" });
    rateSlider.labels.add({ 1.f,"1000ms" });

    mixSlider.labels.add({ 0.f,"0%" });
    mixSlider.labels.add({ 1.f,"100%" });
}

void DelayGUI::setGUIVisible()
{
    // Gets all the sliders and make them visible
    for (auto* component : getComponents())
    {
        addAndMakeVisible(component);
    }
}

void DelayGUI::setLabelsJustification()
{
    feedbackLabel.setJustificationType(juce::Justification::centred);
    rateLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setJustificationType(juce::Justification::centred);
}

void DelayGUI::setLabelsVisible()
{
    addAndMakeVisible(feedbackLabel);
    addAndMakeVisible(rateLabel);
    addAndMakeVisible(mixLabel);
}

void DelayGUI::drawGUI()
{
    juce::Rectangle<int> feedbackSliderRect(getWidth() / 6 * 1 - 87, getHeight() / 4 * 2 - 87, 175, 175);
    feedbackSlider.setBounds(feedbackSliderRect);

    juce::Rectangle<int> rateliderRect(getWidth() / 6 * 3 - 87, getHeight() / 4 * 2 - 87, 175, 175);
    rateSlider.setBounds(rateliderRect);

    juce::Rectangle<int>mixSliderRect(getWidth() / 6 * 5 - 87, getHeight() / 4 * 2 - 87, 175, 175);
    mixSlider.setBounds(mixSliderRect);
}

void DelayGUI::drawLabels()
{
    int fontSize = 18;

    feedbackLabel.setFont(fontSize);
    feedbackLabel.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    juce::Rectangle<int> feedbackLabelRect(getWidth() / 6 * 1 - 87, getHeight() / 4 * 2 - 87 - fontSize, 175, fontSize);
    feedbackLabel.setBounds(feedbackLabelRect);

    rateLabel.setFont(fontSize);
    rateLabel.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    juce::Rectangle<int> rateLabelRect(getWidth() / 6 * 3 - 87, getHeight() / 4 * 2 - 87 - fontSize, 175, fontSize);
    rateLabel.setBounds(rateLabelRect);

    mixLabel.setFont(fontSize);
    mixLabel.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    juce::Rectangle<int> mixLabelRect(getWidth() / 6 * 5 - 87, getHeight() / 4 * 2 - 87 - fontSize, 175, fontSize);
    mixLabel.setBounds(mixLabelRect);
}
