/*
  ==============================================================================

    GUIComponents.cpp
    Created: 3 Mar 2022 5:20:24pm
    Author:  david

  ==============================================================================
*/

#include "GUIComponents.h"

//*******************************************

// Look And Feel
void LookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    // Bounds
    auto bounds = juce::Rectangle<float>(x, y, width, height);

    bool enable = slider.isEnabled();

    // Slider BG Colour
    g.setColour ( enable ? juce::Colours::pink : juce::Colours::grey);
    g.fillEllipse(bounds);
    // Slider Line Colour
    g.setColour(juce::Colours::gold);
    g.drawEllipse(bounds, 3.f);

    //Making sure its a RotarySliderWithLabels
    if (auto* rswl = dynamic_cast<RotarySliderWithLabels*>(&slider))
    {
        // Center
        auto center = bounds.getCentre();

        // Rotary Path
        juce::Path path;
        juce::Rectangle<float> rect;
        rect.setLeft(center.getX() - 2);
        rect.setRight(center.getX() + 2);
        rect.setTop(bounds.getY());
        rect.setBottom(center.getY() - rswl->getTextHeight() * 1.5);

        // Round Rectangle
        path.addRoundedRectangle(rect, 2.f);

        // Just to make sure
        jassert(rotaryStartAngle < rotaryEndAngle);

        // Angle to Radian
        auto sliderAngRad = juce::jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);

        path.applyTransform(juce::AffineTransform().rotated(sliderAngRad, center.getX(), center.getY()));

        // Draw
        g.setColour(juce::Colours::gold);
        g.fillPath(path);

        // Param Value Text
        g.setFont(rswl->getTextHeight());
        juce::String text = rswl->getDisplayString();
        int stringWidth = g.getCurrentFont().getStringWidth(text);
        rect.setSize(stringWidth + 4, rswl->getTextHeight() + 2);
        rect.setCentre(bounds.getCentre());
        g.setColour(juce::Colours::pink);
        g.fillRect(rect);
        g.setColour(juce::Colours::black);
        g.drawFittedText(text, rect.toNearestInt(), juce::Justification::centred, 1);
    }
}

void LookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& toggleButton, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    if (auto* pb = dynamic_cast<PowerButton*>(&toggleButton))
    {
        juce::Path powerButtonPath;

        // Bounds for the ellipse
        juce::Rectangle<int> bounds = toggleButton.getLocalBounds();
        int size = juce::jmin(bounds.getWidth(), bounds.getHeight()) - 6 ;
        juce::Rectangle<float> r = bounds.withSizeKeepingCentre(size, size).toFloat();

        // Inner arc
        float ang = 30.f;
        size -= 6;

        powerButtonPath.addCentredArc(r.getCentreX(), r.getCentreY(), size * 0.5 , size * 0.5, 0.f, juce::degreesToRadians(ang),
                                        juce::degreesToRadians(360-ang), true);

        // The vertical line
        powerButtonPath.startNewSubPath(r.getCentreX(), r.getY());
        powerButtonPath.lineTo(r.getCentre());
        juce::PathStrokeType pathStrokeType(2.f, juce::PathStrokeType::JointStyle::curved);

        // Colors and drawing
        juce::Colour color = toggleButton.getToggleState() ? juce::Colours::antiquewhite : juce::Colours::silver;
        g.setColour(color);

        g.strokePath(powerButtonPath, pathStrokeType);

        g.drawEllipse(r, 2);

    }
    else if(auto* gButton = dynamic_cast<GButton*> (&toggleButton))
    {
        juce::Colour color = !toggleButton.getToggleState() ? juce::Colours::white : juce::Colours::white;
        g.setColour(color);

        juce::Rectangle<int> bounds = toggleButton.getLocalBounds();
        g.drawRect(bounds);

        g.strokePath(gButton->randomPath, juce::PathStrokeType(1.f));
    }
}

//******************************************

// Power button

PowerButton :: PowerButton()
{
    setLookAndFeel(&lnf);
}

PowerButton :: ~PowerButton()
{
    setLookAndFeel(nullptr);
}
//******************************************

GButton :: GButton()
{
    setLookAndFeel(&lnf);
}

GButton :: ~GButton()
{
    setLookAndFeel(nullptr);
}

void GButton::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    juce::Rectangle<int> insetRect = bounds.reduced(4);

    juce::Random random;
    randomPath.clear();

    randomPath.startNewSubPath(insetRect.getX(), insetRect.getY() + insetRect.getHeight() * random.nextFloat());

    for (int x = insetRect.getX() + 1; x < insetRect.getHeight(); x += 2)
    {
        randomPath.lineTo(x, insetRect.getY() + insetRect.getHeight() * random.nextFloat());
    }
}

//******************************************

// Rotary Slider With Lables
RotarySliderWithLabels :: RotarySliderWithLabels(juce::RangedAudioParameter& rap, const juce::String unitSuffix) :
                                                    juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                                    juce::Slider::TextEntryBoxPosition::TextBoxBelow),
                                                    param(&rap),
                                                    suffix(unitSuffix)
{
    setLookAndFeel(&lnf);
}

RotarySliderWithLabels :: ~RotarySliderWithLabels()
{
    setLookAndFeel(nullptr);
}

void RotarySliderWithLabels::paint(juce::Graphics& g)
{
    // Slider Angles For Value
    auto startAngle = juce::degreesToRadians(180.f + 45.f);
    auto endAngle = juce::degreesToRadians(180.f - 45.f) + juce::MathConstants<float>::twoPi;

    // Range
    auto range = getRange();

    //Bounds
    auto sliderBounds = getSliderBounds();

    // Draws the slider bounds
    /*g.setColour(juce::Colours::yellow);
    g.drawRect(getLocalBounds());
    g.setColour(juce::Colours::yellow);
    g.drawRect(sliderBounds);*/

    // Look and Feel
    getLookAndFeel().drawRotarySlider(g, sliderBounds.getX(), sliderBounds.getY(), sliderBounds.getWidth(), sliderBounds.getHeight(),
        juce::jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0), startAngle, endAngle, *this);

    // Slider min max values lables

    // Find center and radius
    auto center = sliderBounds.toFloat().getCentre();
    auto radius = sliderBounds.getWidth() * 0.5f;

    // For each label
    size_t labelsAmount = labels.size();
    for (size_t i = 0; i < labelsAmount; i++)
    {
        // Label Position
        float position = labels[i].position;
        jassert(0.f <= position);
        jassert(position <= 1.f);

        // Set labels center according to the position (knob position 0 or 1)
        auto angle = juce::jmap(position, 0.f, 1.f, startAngle, endAngle);
        auto c = center.getPointOnCircumference(radius + getTextHeight() * 0.5f + 1.f, angle);

        // Label Rectangle and Text 
        juce::Rectangle<float> rect;
        juce::String str = labels[i].label;
        rect.setSize(g.getCurrentFont().getStringWidth(str), getTextHeight());
        rect.setCentre(c);
        rect.setY(rect.getY() + getTextHeight());

        // Label draw
        g.setColour(juce::Colours::lightgrey);
        g.setFont(getTextBoxHeight());
        g.fillRect(rect);
        g.setColour(juce::Colours::black);
        g.drawFittedText(str, rect.toNearestInt(), juce::Justification::centred, 1);
    }
}

juce::Rectangle<int> RotarySliderWithLabels::getSliderBounds() const
{
    auto bounds = getLocalBounds();

    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());
    size -= getTextBoxHeight() * 2;

    juce::Rectangle<int> rect;
    rect.setSize(size, size);
    rect.setCentre(bounds.getCentreX(), 0);
    rect.setY(2);

    return rect;
}

juce::String RotarySliderWithLabels::getDisplayString() const
{
    // Check for choice parameter
    if (auto* choiceParam = dynamic_cast<juce::AudioParameterChoice*>(param))
        return choiceParam->getCurrentChoiceName();

    // Value's Unit
    juce::String str;
    bool addK = false;
    //making sure thats its a float
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(param))
    {
        // Formating the string
        float value = getValue();

        if (value >= 1000.f)
        {
            value /= 1000.f;
            addK = true;
        }
        else
        {
            addK = false;
        }

        str = juce::String(value, addK ? 2 : 0);
    }
    else
    {
        jassertfalse; // This shouldnt happen
    }

    // Add value unit as suffix
    if (suffix.isNotEmpty())
    {
        str << " ";
        if (addK)
            str << "k";
        str << suffix;
    }

    return str;
}

//******************************************