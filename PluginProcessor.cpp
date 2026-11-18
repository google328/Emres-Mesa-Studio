#include "PluginProcessor.h"
#include "PluginEditor.h"

StudioPreampAudioProcessor::StudioPreampAudioProcessor()
    : AudioProcessor (BusesProperties()
                      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "Parameters", createParameterLayout())
{
}

StudioPreampAudioProcessor::~StudioPreampAudioProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout StudioPreampAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterFloat> ("VOLUME", "Volume (Clean Gain)", 0.1f, 10.0f, 5.0f));
    params.push_back (std::make_unique<juce::AudioParameterBool>  ("LEAD_MODE", "Lead Channel", true));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("LEAD_DRIVE", "Lead Drive", 0.0f, 10.0f, 6.5f));
    
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("TREBLE", "Treble", -12.0f, 12.0f, 3.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("MID", "Mid", -12.0f, 12.0f, -2.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("BASS", "Bass", -12.0f, 12.0f, -4.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat> ("GEQ_80",   "80 Hz",   -12.0f, 12.0f,  6.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("GEQ_240",  "240 Hz",  -12.0f, 12.0f,  2.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("GEQ_750",  "750 Hz",  -12.0f, 12.0f, -8.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("GEQ_2200", "2200 Hz", -12.0f, 12.0f,  3.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("GEQ_6600", "6600 Hz", -12.0f, 12.0f,  5.0f));

    params.push_back (std::make_unique<juce::AudioParameterBool>  ("CAB_SIM", "4x12 Cab Sim", true));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("MASTER", "Output Master", -24.0f, 12.0f, 0.0f));

    return { params.begin(), params.end() };
}

void StudioPreampAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    oversampling.initProcessing (samplesPerBlock);

    preBassFilter.prepare (spec);
    preMidFilter.prepare (spec);
    preTrebleFilter.prepare (spec);

    geq80.prepare (spec);
    geq240.prepare (spec);
    geq750.prepare (spec);
    geq2200.prepare (spec);
    geq6600.prepare (spec);

    cabLowPass.prepare (spec);
    cabHighPass.prepare (spec);

    updateFilters();
}

void StudioPreampAudioProcessor::releaseResources() {}

void StudioPreampAudioProcessor::updateFilters()
{
    float bassDb   = *apvts.getRawParameterValue ("BASS");
    float midDb    = *apvts.getRawParameterValue ("MID");
    float trebleDb = *apvts.getRawParameterValue ("TREBLE");

    *preBassFilter.state   = *juce::dsp::IIR::Coefficients<float>::makeLowShelf  (currentSampleRate, 120.0f, 0.7f, juce::Decibels::decibelsToGain (bassDb));
    *preMidFilter.state    = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(currentSampleRate, 450.0f, 0.8f, juce::Decibels::decibelsToGain (midDb));
    *preTrebleFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf (currentSampleRate, 2000.0f, 0.7f, juce::Decibels::decibelsToGain (trebleDb));

    *geq80.state   = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (currentSampleRate, 80.0f,   1.4f, juce::Decibels::decibelsToGain (*apvts.getRawParameterValue ("GEQ_80")));
    *geq240.state  = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (currentSampleRate, 240.0f,  1.4f, juce::Decibels::decibelsToGain (*apvts.getRawParameterValue ("GEQ_240")));
    *geq750.state  = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (currentSampleRate, 750.0f,  1.6f, juce::Decibels::decibelsToGain (*apvts.getRawParameterValue ("GEQ_750")));
    *geq2200.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (currentSampleRate, 2200.0f, 1.4f, juce::Decibels::decibelsToGain (*apvts.getRawParameterValue ("GEQ_2200")));
    *geq6600.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (currentSampleRate, 6600.0f, 1.2f, juce::Decibels::decibelsToGain (*apvts.getRawParameterValue ("GEQ_6600")));

    *cabHighPass.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass (currentSampleRate, 85.0f, 0.707f);
    *cabLowPass.state  = *juce::dsp::IIR::Coefficients<float>::makeLowPass  (currentSampleRate, 5200.0f, 0.85f);
}

float StudioPreampAudioProcessor::processTubeStage (float x, float drive, float asymmetry)
{
    float in = x * drive + asymmetry;
    float out = 0.0f;

    if (in > 0.0f)
        out = std::tanh (in);
    else
        out = in / (1.0f + std::abs (in * 1.2f));

    return out - std::tanh (asymmetry);
}

void StudioPreampAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    updateFilters();

    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);

    preBassFilter.process (context);
    preMidFilter.process (context);
    preTrebleFilter.process (context);

    float volume    = *apvts.getRawParameterValue ("VOLUME");
    bool leadMode   = *apvts.getRawParameterValue ("LEAD_MODE") > 0.5f;
    float leadDrive = *apvts.getRawParameterValue ("LEAD_DRIVE");
    bool cabSim     = *apvts.getRawParameterValue ("CAB_SIM") > 0.5f;
    float master    = juce::Decibels::decibelsToGain (*apvts.getRawParameterValue ("MASTER"));

    auto oversampledBlock = oversampling.processSamplesUp (block);

    for (size_t ch = 0; ch < oversampledBlock.getNumChannels(); ++ch)
    {
        auto* data = oversampledBlock.getChannelPointer (ch);
        for (size_t i = 0; i < oversampledBlock.getNumSamples(); ++i)
        {
            float s = data[i];
            s = processTubeStage (s, volume * 0.8f, 0.08f);

            if (leadMode)
            {
                s = processTubeStage (s, 1.0f + (leadDrive * 0.65f), -0.12f);
                s = processTubeStage (s, 1.0f + (leadDrive * 0.45f),  0.10f);
            }

            data[i] = s * 0.6f;
        }
    }

    oversampling.processSamplesDown (block);

    geq80.process (context);
    geq240.process (context);
    geq750.process (context);
    geq2200.process (context);
    geq6600.process (context);

    if (cabSim)
    {
        cabHighPass.process (context);
        cabLowPass.process (context);
    }

    buffer.applyGain (master);
}

juce::AudioProcessorEditor* StudioPreampAudioProcessor::createEditor()
{
    return new StudioPreampAudioProcessorEditor (*this);
}

void StudioPreampAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void StudioPreampAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr && xmlState->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StudioPreampAudioProcessor();
}
