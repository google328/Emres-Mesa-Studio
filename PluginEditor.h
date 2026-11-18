#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class BoogieLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                           juce::Slider& slider) override;

    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const juce::Slider::SliderStyle style, juce::Slider& slider) override;

    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
};

class StudioPreampAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    StudioPreampAudioProcessorEditor (StudioPreampAudioProcessor&);
    ~StudioPreampAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    StudioPreampAudioProcessor& audioProcessor;
    BoogieLookAndFeel boogieLookAndFeel;

    juce::Slider inputSlider, volumeSlider, leadDriveSlider, trebleSlider, midSlider, bassSlider, cabThumpSlider, masterSlider;
    juce::Label inputLabel, volumeLabel, leadDriveLabel, trebleLabel, midLabel, bassLabel, cabThumpLabel, masterLabel;

    juce::Slider geq80Slider, geq240Slider, geq750Slider, geq2200Slider, geq6600Slider;
    juce::Label geq80Label, geq240Label, geq750Label, geq2200Label, geq6600Label;

    juce::ToggleButton leadModeButton { "LEAD CHANNEL" };
    juce::ToggleButton cabSimButton   { "4x12 EVM12L" };
    juce::TextButton   loadIRButton   { "LOAD WAV IR" };

    std::unique_ptr<juce::FileChooser> fileChooser;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<SliderAttachment> inputAtt, volumeAtt, leadDriveAtt, trebleAtt, midAtt, bassAtt, cabThumpAtt, masterAtt;
    std::unique_ptr<SliderAttachment> geq80Att, geq240Att, geq750Att, geq2200Att, geq6600Att;
    std::unique_ptr<ButtonAttachment> leadModeAtt, cabSimAtt;

    void setupKnob (juce::Slider& slider, juce::Label& label, const juce::String& text);
    void setupFader (juce::Slider& slider, juce::Label& label, const juce::String& text);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StudioPreampAudioProcessorEditor)
};
