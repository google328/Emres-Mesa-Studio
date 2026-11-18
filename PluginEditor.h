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

    juce::Slider volumeSlider, leadDriveSlider, trebleSlider, midSlider, bassSlider, masterSlider;
    juce::Label volumeLabel, leadDriveLabel, trebleLabel, midLabel, bassLabel, masterLabel;

    juce::Slider geq80Slider, geq240Slider, geq750Slider, geq2200Slider, geq6600Slider;
    juce::Label geq80Label, geq240Label, geq750Label, geq2200Label, geq6600Label;

    juce::ToggleButton leadModeButton { "LEAD CHANNEL" };
    juce::ToggleButton cabSimButton   { "4x12 CAB SIM" };

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<SliderAttachment> volumeAtt, leadDriveAtt, trebleAtt, midAtt, bassAtt, masterAtt;
    std::unique_ptr<SliderAttachment> geq80Att, geq240Att, geq750Att, geq2200Att, geq6600Att;
    std::unique_ptr<ButtonAttachment> leadModeAtt, cabSimAtt;

    void setupKnob (juce::Slider& slider, juce::Label& label, const juce::String& text);
    void setupFader (juce::Slider& slider, juce::Label& label, const juce::String& text);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StudioPreampAudioProcessorEditor)
};
