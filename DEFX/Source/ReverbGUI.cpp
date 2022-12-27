/*
  ==============================================================================

    ReverbGUI.cpp
    Created: 28 Apr 2022 1:41:11pm
    Author:  david

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ReverbGUI.h"

//==============================================================================
ReverbGUI::ReverbGUI(DEFXAudioProcessor& p) : audioProcessor(p),
// Sliders Presentation and Units
roomSizeSlider(*audioProcessor.apvts.getParameter("REVERB_ROOM_SIZE"), "m"),
dampingSlider(*audioProcessor.apvts.getParameter("REVERB_DAMPING"), "%"),
widthSlider(*audioProcessor.apvts.getParameter("REVERB_WIDTH"), "s/m"),// Stereo Mono 
freezeModeSlider(*audioProcessor.apvts.getParameter("REVERB_FREEZE_MODE"), "F/T"),// False True
mixSlider(*audioProcessor.apvts.getParameter("REVERB_MIX"), "%"),

// Sliders State Attachments
roomSizeSliderAttachment(audioProcessor.apvts, "REVERB_ROOM_SIZE", roomSizeSlider),
dampingSliderAttachment(audioProcessor.apvts, "REVERB_DAMPING", dampingSlider),
widthSliderAttachment(audioProcessor.apvts, "REVERB_WIDTH", widthSlider),
freezeModeSliderAttachment(audioProcessor.apvts, "REVERB_FREEZE_MODE", freezeModeSlider),
mixSliderAttachment(audioProcessor.apvts, "REVERB_MIX", mixSlider)
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

ReverbGUI::~ReverbGUI()
{
}

void ReverbGUI::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::black);
    float fontSize = 50.f;
    g.setFont(fontSize);
}

void ReverbGUI::resized()
{
    drawGUI();
    drawLabels();
}

std::vector<juce::Component*> ReverbGUI::getComponents()
{
    return
    {
        &roomSizeSlider,
        &dampingSlider,
        &widthSlider,
        &freezeModeSlider,
        &mixSlider
    };
}

void ReverbGUI::setSlidersMinMaxLables()
{
    roomSizeSlider.labels.add({ 0.f,"0.m" });
    roomSizeSlider.labels.add({ 1.f,"100.m" });

    dampingSlider.labels.add({ 0.f,"0.%" });
    dampingSlider.labels.add({ 1.f,"100.%" });

    widthSlider.labels.add({ 0.f,"0.s/m" });
    widthSlider.labels.add({ 1.f,"1.s/m" });

    freezeModeSlider.labels.add({ 0.f,"Off" });
    freezeModeSlider.labels.add({ 1.f,"On" });

    mixSlider.labels.add({ 0.f,"0%" });
    mixSlider.labels.add({ 1.f,"100%" });
}

void ReverbGUI::setGUIVisible()
{
    // Gets all the sliders and make them visible
    for (auto* component : getComponents())
    {
        addAndMakeVisible(component);
    }
}

// Justification to the center of the rect
void ReverbGUI::setLabelsJustification()
{
    roomSizeLabel.setJustificationType(juce::Justification::centred);
    dampingLabel.setJustificationType(juce::Justification::centred);
    widthLabel.setJustificationType(juce::Justification::centred);
    freezeModeLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setJustificationType(juce::Justification::centred);
}

void ReverbGUI::setLabelsVisible()
{
    addAndMakeVisible(roomSizeLabel);
    addAndMakeVisible(dampingLabel);
    addAndMakeVisible(widthLabel);
    addAndMakeVisible(freezeModeLabel);
    addAndMakeVisible(mixLabel);
}

void ReverbGUI::drawGUI()
{
    juce::Rectangle<int> roomSizeSliderRect(getWidth() / 6 * 1 - 75, getHeight() / 4 * 1 - 75, 150, 150);
    roomSizeSlider.setBounds(roomSizeSliderRect);

    juce::Rectangle<int> dampingSliderRect(getWidth() / 6 * 1 - 75, getHeight() / 4 * 3 - 75, 150, 150);
    dampingSlider.setBounds(dampingSliderRect);

    juce::Rectangle<int> widthSliderRect(getWidth() / 6 * 5 - 75, getHeight() / 4 * 1 - 75, 150, 150);
    widthSlider.setBounds(widthSliderRect);

    juce::Rectangle<int> freezeModeSliderRect(getWidth() / 6 * 5 - 75, getHeight() / 4 * 3 - 75, 150, 150);
    freezeModeSlider.setBounds(freezeModeSliderRect);

    juce::Rectangle<int> mixSliderRect(getWidth() / 6 * 3 - 75, getHeight() / 4 * 2 - 75, 150, 150);
    mixSlider.setBounds(mixSliderRect);
}

void ReverbGUI::drawLabels()
{
    int fontSize = 18;

    roomSizeLabel.setFont(fontSize);
    roomSizeLabel.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    juce::Rectangle<int> roomSizeLabelRect(getWidth() / 6 * 1 - 75, getHeight() / 4 * 1 - 71 - fontSize, 150, fontSize);
    roomSizeLabel.setBounds(roomSizeLabelRect);

    dampingLabel.setFont(fontSize);
    dampingLabel.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    juce::Rectangle<int> dampingLabelRect(getWidth() / 6 * 1 - 75, getHeight() / 4 * 3 - 71 - fontSize, 150, fontSize);
    dampingLabel.setBounds(dampingLabelRect);

    widthLabel.setFont(fontSize);
    widthLabel.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    juce::Rectangle<int> widthLabelRect(getWidth() / 6 * 5 - 75, getHeight() / 4 * 1 - 71 - fontSize, 150, fontSize);
    widthLabel.setBounds(widthLabelRect);

    freezeModeLabel.setFont(fontSize);
    freezeModeLabel.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    juce::Rectangle<int> freezeModeLabelRect(getWidth() / 6 * 5 - 75, getHeight() / 4 * 3 - 71 - fontSize, 150, fontSize);
    freezeModeLabel.setBounds(freezeModeLabelRect);

    mixLabel.setFont(fontSize);
    mixLabel.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    juce::Rectangle<int> mixLabelRect(getWidth() / 6 * 3 - 75, getHeight() / 4 * 2 - 71 - fontSize, 150, fontSize);
    mixLabel.setBounds(mixLabelRect);
}
