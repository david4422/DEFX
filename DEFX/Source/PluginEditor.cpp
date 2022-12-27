/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
DEFXAudioProcessorEditor::DEFXAudioProcessorEditor(DEFXAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    // Response Curve
    responseCurveComponent(audioProcessor)
{
    // Gets all the sliders and make them visible
    setGUIVisible();
    // Size 
    setSize(600, 700);
}

DEFXAudioProcessorEditor::~DEFXAudioProcessorEditor()
{
}

//==============================================================================
void DEFXAudioProcessorEditor::paint(juce::Graphics& g)
{
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // Background
    g.fillAll(juce::Colours::lightgrey);

    // Text
    g.setColour(juce::Colours::black);
    float fornSize = 25.f;
    g.setFont(fornSize);
    g.drawFittedText("DEFX",0, 2,getWidth(), fornSize, juce::Justification::centred, true);
}

void DEFXAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    drawGUI();

}

std::vector<juce::Component*> DEFXAudioProcessorEditor::getComponents()
{
    return
    {   // Responce Curve
        &responseCurveComponent,

        // FX display
        &fxDisplay,

        // Pedal Board
        &pedalBoard
    };
}

//*********************



void DEFXAudioProcessorEditor::setGUIVisible()
{
    // Gets all the sliders and make them visible
    for (auto* component : getComponents())
    {
        addAndMakeVisible(component);
    }
}

void DEFXAudioProcessorEditor::setGUIAttachment()
{
}

void DEFXAudioProcessorEditor::drawGUI()
{
    // Response Curve
    juce::Rectangle<int> responseCurveRect(0 , getHeight() / 8 * 1 - getHeight() / 8 / 1.5, getWidth(), getHeight() / 6);
    responseCurveComponent.setBounds(responseCurveRect);

    // FX Display
    juce::Rectangle<int> fxDisplayRect(0, getHeight() / 8 * 2 - getHeight() / 8 / 3, getWidth(), getHeight() / 2);
    fxDisplay.setBounds(fxDisplayRect);

    // Pedal Board
    juce::Rectangle<int> pedalboardRect(0, getHeight() / 8 * 6 - 30, getWidth(), getHeight() / 4);
    pedalBoard.setBounds(pedalboardRect);
}

//*********************
