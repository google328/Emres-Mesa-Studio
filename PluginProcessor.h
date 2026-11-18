#pragma once

#include <JuceHeader.h>

class StudioPreampAudioProcessor : public juce::AudioProcessor
{
public:
    StudioPreampAudioProcessor();
    ~StudioPreampAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Boogie Studio Preamp"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int index) override {}
    const juce::String getProgramName (int index) override { return {}; }
    void changeProgramName (int index, const juce::String& newName) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Lädt eigene .wav Impulse Responses in die Boxen-Simulation
    void loadImpulseResponse (const juce::File& file);
    bool isCustomIRLoaded() const { return customIRLoaded; }

    juce::AudioProcessorValueTreeState apvts;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    juce::dsp::Oversampling<float> oversampling { 2, 2, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };
    
    using IIRFilter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    IIRFilter preBassFilter, preMidFilter, preTrebleFilter;
    IIRFilter geq80, geq240, geq750, geq2200, geq6600;

    // Mesa Oversized 4x12 EVM12L Speaker & Cabinet Modeling Netzwerk
    IIRFilter cabSubCut;         // Sub-Bass Schutz unterhalb 65 Hz
    IIRFilter cabBigResonance;   // Typische Gehäuseresonanz des großen Mesa-Cabinets (~98 Hz)
    IIRFilter evmLowMidPunch;    // Straffer Tiefmitten-Druck des EVM12L (~210 Hz)
    IIRFilter evmMidFlatness;    // Kontrollierte Mitten-Absenkung (~850 Hz)
    IIRFilter evmPresencePeak;   // Charakteristischer EVM12L Aluminium-Dustcap Peak (~4200 Hz)
    IIRFilter evmConeRollOff;    // Steiler Hochton-Abfall des 12-Zoll-Speakers (~6200 Hz)

    // Optionaler Faltungshall (Convolution) für eigene IR-Dateien
    juce::dsp::Convolution irLoader;
    bool customIRLoaded = false;

    void updateFilters();
    float processTubeStage (float sample, float drive, float asymmetry);

    double currentSampleRate = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StudioPreampAudioProcessor)
};
