/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DEFXAudioProcessor::DEFXAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

DEFXAudioProcessor::~DEFXAudioProcessor()
{
}

//==============================================================================
const juce::String DEFXAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DEFXAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DEFXAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DEFXAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DEFXAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DEFXAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DEFXAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DEFXAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DEFXAudioProcessor::getProgramName (int index)
{
    return {};
}

void DEFXAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DEFXAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
     

    // Set the spec for each chain

    // Spec for delay
    juce::dsp::ProcessSpec eqSpec;
    eqSpec.maximumBlockSize = samplesPerBlock;
    eqSpec.sampleRate = sampleRate;
    eqSpec.numChannels = 1;

    eq.prepareToPlay(eqSpec, apvts);


    // Spec for delay
    juce::dsp::ProcessSpec delaySpec;
    delaySpec.maximumBlockSize = samplesPerBlock;
    delaySpec.sampleRate = sampleRate;
    delaySpec.numChannels = 2;

    delay.prepareToPlay(delaySpec, apvts);

    // Spec for reverb
    juce::dsp::ProcessSpec reverbSpec;
    reverbSpec.maximumBlockSize = samplesPerBlock;
    reverbSpec.sampleRate = sampleRate;
    reverbSpec.numChannels = 1;

    reverb.prepareToPlay(reverbSpec);

}

void DEFXAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DEFXAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DEFXAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
    //    auto* channelData = buffer.getWritePointer (channel);

    //    // ..do something to the data...

    //EQ
    if(*apvts.getRawParameterValue("EQ_ENABLED"))
        eq.processBlock(buffer, apvts);

    //DELAY
    if (*apvts.getRawParameterValue("DELAY_ENABLED"))
        delay.processBlock(buffer, totalNumInputChannels, totalNumOutputChannels, apvts);

    // REVERB
    if (*apvts.getRawParameterValue("REVERB_ENABLED"))
        reverb.processBlock(buffer, apvts);
     
         
}

//==================================fix========================================
bool DEFXAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DEFXAudioProcessor::createEditor()
{
    return new DEFXAudioProcessorEditor (*this);
}

//=================================fix==========================================
void DEFXAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    //*******************
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
    //*******************
}

void DEFXAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    //*******************
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if ((tree.isValid()) && (eq.getSpec().sampleRate > 0) && (delay.getSpec().sampleRate > 0) && (reverb.getSpec().sampleRate > 0))
    {
        apvts.replaceState(tree);
        eq.updateFilters(apvts);
    }
    //*******************
}

//==============================================================================

//******************

// This will create the parameters layour for the AudioProcessorValueTreeState
// 3 Bands: Low, High, Parametric/Peak
// Cut Bands: Controllable Frequencies/Slope
// Parametric Band: Controllable Frequency, Gain, Quality   
juce::AudioProcessorValueTreeState::ParameterLayout DEFXAudioProcessor::createParameterLayout()
{
    juce::StringArray stringArr;
    for (int i = 0; i < 4; i++)
    {
        juce::String str;
        str << (12 + i * 12);
        str << " db/Oct";
        stringArr.add(str);
    }

    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Low cut
    layout.add(std::make_unique<juce::AudioParameterFloat>("EQ_LOW_CUT_FREQ", "LowCut Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f), 20.f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("EQ_LOW_CUT_SLOPE", "LowCut Slope", stringArr, 0));
    layout.add(std::make_unique<juce::AudioParameterBool>("EQ_LOW_CUT_ENABLED", "LowCut Enabled", true));

    // Peak
    layout.add(std::make_unique<juce::AudioParameterFloat>("EQ_PEAK_FREQ", "Peak Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f), 750.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("EQ_PEAK_GAIN", "Peak Gain", juce::NormalisableRange<float>(-24.f, 24.f, 0.5f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("EQ_PEAK_QUALITY", "Peak Quality", juce::NormalisableRange<float>(0.1f, 10.f, 0.05f, 1.f), 1.f));
    layout.add(std::make_unique<juce::AudioParameterBool>("EQ_PEAK_ENABLED", "Peak Enabled", true));
     
    // High cut
    layout.add(std::make_unique<juce::AudioParameterFloat>("EQ_HIGH_CUT_FREQ", "HighCut Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f), 20000.f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("EQ_HIGH_CUT_SLOPE", "HighCut Slope", stringArr, 0));
    layout.add(std::make_unique<juce::AudioParameterBool>("EQ_HIGH_CUT_ENABLED", "HighCut Enabled", true));
    // EQ pedal board
    layout.add(std::make_unique<juce::AudioParameterBool>("EQ_ENABLED", "EQ Enabled", true));
    layout.add(std::make_unique<juce::AudioParameterBool>("EQ_DISPLAY_ENABLED", "EQ Display Enabled", true));


    // Delay
    layout.add(std::make_unique<juce::AudioParameterFloat>("DELAY_FEEDBACK", "Feedback", -6.f, 0.f, -1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("DELAY_RATE", "Rate",0.48f, 1000.f, 340.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("DELAY_MIX", "Mix", juce::NormalisableRange<float>{0.f, 1.f, 0.01f}, 0.5f));
    // Delay pedal board
    layout.add(std::make_unique<juce::AudioParameterBool>("DELAY_ENABLED", "Delay Enabled", true));
    layout.add(std::make_unique<juce::AudioParameterBool>("DELAY_DISPLAY_ENABLED", "Delay Display Enabled", false));


    // Reverb
    layout.add(std::make_unique<juce::AudioParameterFloat>("REVERB_ROOM_SIZE", "Room Size", juce::NormalisableRange<float>{0.f, 1.f, 0.01f}, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("REVERB_DAMPING", "Damping", juce::NormalisableRange<float>{0.f, 1.f, 0.01f}, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("REVERB_WIDTH", "Width", juce::NormalisableRange<float>{0.f, 1.f, 0.01f}, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("REVERB_FREEZE_MODE", "Freeze Mode", juce::NormalisableRange<float>{0.f, 1.f, 1.f}, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("REVERB_MIX", "Mix", juce::NormalisableRange<float>{0.f, 1.f, 0.01f}, 0.5f));
    // Reverb pedal board
    layout.add(std::make_unique<juce::AudioParameterBool>("REVERB_ENABLED", "Reverb Enabled", true));
    layout.add(std::make_unique<juce::AudioParameterBool>("REVERB_DISPLAY_ENABLED", "Reverb Display Enabled", false));

    // Visible
    juce::StringArray fxNameArr{ "Eq" , "Delay", "Reverb" };
    layout.add(std::make_unique<juce::AudioParameterChoice>("CURRENT_VISIBLE_FX", "Current Visible FX", fxNameArr, 0));


    return layout;
}

//******************




//******************

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DEFXAudioProcessor();
}


