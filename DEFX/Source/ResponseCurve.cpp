/*
  ==============================================================================

    ResponseCurve.cpp
    Created: 3 Mar 2022 5:18:50pm
    Author:  david

  ==============================================================================
*/

#include "ResponseCurve.h"

//******************************************

ResponseCurveComponent::ResponseCurveComponent(DEFXAudioProcessor& p) : audioProcessor(p)
{
    // Add Listeners to the Audio Processor's Parameters (in order to change the Response Curve GUI)
    const auto& params = audioProcessor.getParameters();
    for (auto param : params)
    {
        param->addListener(this);
    }

    // Update the mono chain to repaint the response curve
    updateMonoChain();

    // Starts the timer 
    startTimerHz(60);
}

ResponseCurveComponent::~ResponseCurveComponent()
{
    // Removes the listeners to the parameters
    const auto& params = audioProcessor.getParameters();
    for (auto param : params)
    {
        param->removeListener(this);
    }
}

// Atomic flag for listening if there was a change in the audio processor parameters
void ResponseCurveComponent::parameterValueChanged(int parameterIndex, float newValue)
{
    parametersChanged.set(true);
}

void ResponseCurveComponent::timerCallback()
{
    if (parametersChanged.compareAndSetBool(false, true)) //Resets back to false
    {
        updateMonoChain();
    }
}

// Paint
void ResponseCurveComponent::paint(juce::Graphics& g)
{
    // Draws the grid
    g.drawImage(background, getLocalBounds().toFloat());

    // Calculat Magnitudes
    std::vector<double> magnitudes;
    calculateMagnitudes(magnitudes);

    // Paint the response curve
    paintResponseCurve(g, magnitudes);
}

void ResponseCurveComponent::resized()
{
    background = juce::Image(juce::Image::PixelFormat::RGB, getWidth(), getHeight(), true);
    juce::Graphics g(background);

    g.fillAll(juce::Colours::wheat);

    // Grid
    drawVerticalGrid(g);
    drawHorizentalGrid(g);
}

void ResponseCurveComponent::updateMonoChain()
{
    // Update the mono chain
    auto chainSetting = getChainSettings(audioProcessor.apvts);
    auto sampleRate = audioProcessor.getSampleRate();
    // Low cut
    monoChain.setBypassed<ChainPositions::LowCut>(chainSetting.lowCutBypassed);
    auto lowCutCoefficients = makeLowCutFilter(chainSetting, sampleRate);
    updateCutFilter(monoChain.get<ChainPositions::LowCut>(), lowCutCoefficients, chainSetting.lowCutSlope);
    // Peak
    monoChain.setBypassed<ChainPositions::Peak>(chainSetting.peakBypassed);
    auto peakCoefficients = makePeakFilter(chainSetting, sampleRate);
    updateCoefficients(monoChain.get<ChainPositions::Peak>().coefficients, peakCoefficients);
    // High cut
    monoChain.setBypassed<ChainPositions::HighCut>(chainSetting.highCutBypassed);
    auto highCutCoefficients = makeHighCutFilter(chainSetting, sampleRate);
    updateCutFilter(monoChain.get<ChainPositions::HighCut>(), highCutCoefficients, chainSetting.highCutSlope);

    // Signal a reapaint (call paint again)
    repaint();
}
juce::Rectangle<int> ResponseCurveComponent::getRendrArea()
{
    auto bounds = getLocalBounds();

    /*bounds.reduce(JUCE_LIVE_CONSTANT(5),
        JUCE_LIVE_CONSTANT(5));*/

        /*bounds.reduce(10,
            8);*/

    bounds.removeFromTop(12);
    bounds.removeFromBottom(2);
    bounds.removeFromLeft(20);
    bounds.removeFromRight(20);

    return bounds;
}

juce::Rectangle<int> ResponseCurveComponent::getAnalysisArea()
{
    auto bounds = getRendrArea();

    bounds.removeFromTop(4);
    bounds.removeFromBottom(4);

    return bounds;
}
void ResponseCurveComponent::calculateMagnitudes(std::vector<double>& magnitudes)
{
    auto analysisArea = getAnalysisArea();
    auto w = analysisArea.getWidth();

    // Magnitudes
    magnitudes.resize(w);

    // Getting the filters parameters
    auto& lowCut = monoChain.get<ChainPositions::LowCut>();
    auto& peak = monoChain.get<ChainPositions::Peak>();
    auto& highCut = monoChain.get<ChainPositions::HighCut>();

    // Getting the sample rate
    auto sampleRate = audioProcessor.getSampleRate();

    // Gets the magnitudes For each width pixel in the response curve
    for (size_t i = 0; i < w; i++)
    {
        // Normelizing the frequency according to the pixel location
        double magnitude = 1.f;
        double freq = juce::mapToLog10(double(i) / double(w), 20.0, 20000.0);

        // Low cut 
        if (!monoChain.isBypassed<ChainPositions::LowCut>())
        {
            if (!lowCut.isBypassed<0>())
                magnitude *= lowCut.get<0>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!lowCut.isBypassed<1>())
                magnitude *= lowCut.get<1>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!lowCut.isBypassed<2>())
                magnitude *= lowCut.get<2>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!lowCut.isBypassed<3>())
                magnitude *= lowCut.get<3>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        }


        // Peak
        if (!monoChain.isBypassed<ChainPositions::Peak>())
        {
            if (!monoChain.isBypassed<ChainPositions::Peak>())
                magnitude *= peak.coefficients->getMagnitudeForFrequency(freq, sampleRate);
        }


        // High cut 
        if (!monoChain.isBypassed<ChainPositions::HighCut>())
        {
            if (!highCut.isBypassed<0>())
                magnitude *= highCut.get<0>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!highCut.isBypassed<1>())
                magnitude *= highCut.get<1>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!highCut.isBypassed<2>())
                magnitude *= highCut.get<2>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!highCut.isBypassed<3>())
                magnitude *= highCut.get<3>().coefficients->getMagnitudeForFrequency(freq, sampleRate);

        }

        // Converts magnitude to DB
        magnitudes[i] = juce::Decibels::gainToDecibels(magnitude);
    }
}

void ResponseCurveComponent::paintResponseCurve(juce::Graphics& g, std::vector<double>& magnitudes)
{    
    // Making the response curve 
    juce::Path responseCurve;

    juce::Rectangle<int> analysisArea = getAnalysisArea();

    // Curve limits
    const double outputMin = analysisArea.getBottom();
    const double outputMax = analysisArea.getY();

    // Scaling the values into range (Lambda)
    auto map = [outputMin, outputMax](double input)
    {
        // Rescailing the values into range of the peak gain
        return juce::jmap(input, -24.0, 24.0, outputMin, outputMax);
    };

    // Subcurve for the magnitude
    responseCurve.startNewSubPath(analysisArea.getX(), map(magnitudes.front()));

    for (size_t i = 1; i < magnitudes.size(); i++)
    {
        responseCurve.lineTo(analysisArea.getX() + i, map(magnitudes[i]));
    }

    // Draw the response curve area
    g.setColour(juce::Colours::rebeccapurple);
    g.drawRoundedRectangle(getRendrArea().toFloat(), 4.f, 3.f);

    // Draw the response curve
    g.setColour(juce::Colours::rebeccapurple);
    g.strokePath(responseCurve, juce::PathStrokeType(2.f));
}

void ResponseCurveComponent::drawVerticalGrid(juce::Graphics& g)
{
    // Vertical frequencies lines
    juce::Array<float> freqs
    {
        20, 30 ,40 ,50, 100,
        200, 300 ,400, 500, 1000,
        2000, 3000 ,4000 ,5000, 10000,
        20000
    };

    // Analize area bounds
    juce::Rectangle<int> analysisArea = getAnalysisArea();
    int left = analysisArea.getX();
    int right = analysisArea.getRight();
    int top = analysisArea.getY();
    int bottom = analysisArea.getBottom();
    int width = analysisArea.getWidth();

    // X positions for the verticals lines 
    juce::Array<float> xs;

    for (float f : freqs)
    {
        float normX = juce::mapFromLog10(f, 20.f, 20000.f);
        xs.add(left + width * normX);
    }

    // Drawing the vertical line of the grid
    g.setColour(juce::Colours::purple);
    for (float x : xs)
    {
        g.drawVerticalLine(x, top, bottom);
    }

    // Grid Text
    const int fontHeight = 10;
    g.setFont(fontHeight);

    // Frequencies text
    for (size_t i = 0; i < freqs.size(); i++)
    {
        // For each vertical line
        float f = freqs[i];
        float x = xs[i];

        // Should add k ?
        bool addK = false;
        if (f >= 1000.f)
        {
            f /= 1000.f;
            addK = true;
        }

        // Put in the string
        juce::String str(f, 0);

        if (addK)
            str << "K";

        auto textWidth = g.getCurrentFont().getStringWidth(str);

        juce::Rectangle<int> r;
        r.setSize(textWidth, fontHeight);
        r.setCentre(x, 0);
        r.setY(1);

        // Draw the string
        g.setColour(juce::Colours::black);
        g.drawFittedText(str, r, juce::Justification::centred, 1);
    }
}

void ResponseCurveComponent::drawHorizentalGrid(juce::Graphics& g)
{


    // Horizontal Decibel lines
    juce::Array<float> gain
    {
        -24, -12 , 0 , 12 , 24
    };

    // Analize area bounds
    juce::Rectangle<int> analysisArea = getAnalysisArea();
    int left = analysisArea.getX();
    int right = analysisArea.getRight();
    int top = analysisArea.getY();
    int bottom = analysisArea.getBottom();
    int width = analysisArea.getWidth();

    // Draw Horizontal lines
    for (float gDb : gain)
    {
        float y = juce::jmap(gDb, -24.f, 24.f, float(bottom), float(top));
        g.setColour(gDb == 0.f ? juce::Colours::white : juce::Colours::purple);
        g.drawHorizontalLine(y, left, right);
    }

    // gain text
    for (float gdb : gain)
    {
        const int fontHeight = 10;
        g.setFont(fontHeight);

        // gain text
        float y = juce::jmap(gdb, -24.f, 24.f, float(bottom), float(top));

        juce::String str;
        if (gdb > 0)
            str << "+";
        str << gdb;

        auto textwidth = g.getCurrentFont().getStringWidth(str);

        juce::Rectangle<int> r;
        r.setSize(textwidth, fontHeight);
        r.setX(getWidth() - textwidth);
        r.setCentre(r.getCentreX(), y);

        g.setColour(juce::Colours::black);
        g.drawFittedText(str, r, juce::Justification::centred, 1);
    }
}
//******************************************
