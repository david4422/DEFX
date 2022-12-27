/*
  ==============================================================================

    GUIComponents.h
    Created: 3 Mar 2022 5:20:24pm
    Author:  david

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//****************************************

struct LookAndFeel : juce::LookAndFeel_V4
{
    void drawRotarySlider(juce::Graphics&,
                            int x, int y, int width, int height,
                            float sliderPosProportional,
                            float rotaryStartAngle,
                            float rotaryEndAngle,
                            juce::Slider&) override;

    void drawToggleButton(juce::Graphics& g,
                            juce::ToggleButton& toggleButton,
                            bool shouldDrawButtonAsHighlighted,
                            bool shouldDrawButtonAsDown) override;
};

//****************************************
 
struct PowerButton : juce :: ToggleButton
{
    PowerButton();
    ~PowerButton();

private:
    LookAndFeel lnf;
};

//****************************************

struct GButton : juce::ToggleButton
{
    GButton();
    ~GButton();

    void resized() override;

    juce::Path randomPath;

private:
    LookAndFeel lnf;
};

//****************************************

struct LabelPosition
{
    float position;
    juce::String label;
};

//****************************************

struct RotarySliderWithLabels : juce::Slider
{
    RotarySliderWithLabels(juce::RangedAudioParameter& rap, const juce::String unitSuffix);
    ~RotarySliderWithLabels();
  
    void paint(juce::Graphics& g) override;

    juce::Array<LabelPosition> labels;
    juce::Rectangle<int> getSliderBounds() const;
    int getTextHeight() const { return 14; };
    juce::String getDisplayString() const;

private:
    LookAndFeel lnf;
    juce::RangedAudioParameter* param;
    juce::String suffix;
};

//****************************************

