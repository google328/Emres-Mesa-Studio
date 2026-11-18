#include "PluginProcessor.h"
#include "PluginEditor.h"

void BoogieLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                          juce::Slider&)
{
    auto radius = (float) juce::jmin (width / 2, height / 2) - 4.0f;
    auto centreX = (float) x + (float) width  * 0.5f;
    auto centreY = (float) y + (float) height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    g.setColour (juce::Colour (0xff181818));
    g.fillEllipse (rx, ry, rw, rw);
    g.setColour (juce::Colour (0xff444444));
    g.drawEllipse (rx, ry, rw, rw, 2.0f);

    auto innerR = radius * 0.65f;
    g.setColour (juce::Colour (0xffcccccc));
    g.fillEllipse (centreX - innerR, centreY - innerR, innerR * 2.0f, innerR * 2.0f);

    juce::Path p;
    auto pointerLength = radius * 0.85f;
    auto pointerThickness = 3.0f;
    p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
    g.setColour (juce::Colours::white);
    g.fillPath (p);
}

void BoogieLookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                                          float sliderPos, float, float,
                                          const juce::Slider::SliderStyle, juce::Slider&)
{
    auto trackWidth = 4.0f;
    auto centreX = (float) x + (float) width * 0.5f;

    g.setColour (juce::Colour (0xff0a0a0a));
    g.fillRoundedRectangle (centreX - trackWidth * 0.5f, (float) y, trackWidth, (float) height, 2.0f);

    g.setColour (juce::Colour (0xff666666));
    g.drawLine ((float) x + 4.0f, (float) y + height * 0.5f, (float) x + width - 4.0f, (float) y + height * 0.5f, 1.0f);

    auto thumbW = 22.0f;
    auto thumbH = 12.0f;
    g.setColour (juce::Colour (0xff252525));
    g.fillRoundedRectangle (centreX - thumbW * 0.5f, sliderPos - thumbH * 0.5f, thumbW, thumbH, 2.0f);
    g.setColour (juce::Colours::white);
    g.drawLine (centreX - thumbW * 0.4f, sliderPos, centreX + thumbW * 0.4f, sliderPos, 2.0f);
}

StudioPreampAudioProcessorEditor::StudioPreampAudioProcessorEditor (StudioPreampAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel (&boogieLookAndFeel);

    setupKnob (volumeSlider,    volumeLabel,    "VOLUME");
    setupKnob (trebleSlider,    trebleLabel,    "TREBLE");
    setupKnob (midSlider,       midLabel,       "MIDDLE");
    setupKnob (bassSlider,      bassLabel,      "BASS");
    setupKnob (leadDriveSlider, leadDriveLabel, "LEAD DRIVE");
    setupKnob (masterSlider,    masterLabel,    "MASTER");

    setupFader (geq80Slider,   geq80Label,   "80");
    setupFader (geq240Slider,  geq240Label,  "240");
    setupFader (geq750Slider,  geq750Label,  "750");
    setupFader (geq2200Slider, geq2200Label, "2200");
    setupFader (geq6600Slider, geq6600Label, "6600");

    addAndMakeVisible (leadModeButton);
    addAndMakeVisible (cabSimButton);

    volumeAtt    = std::make_unique<SliderAttachment> (audioProcessor.apvts, "VOLUME",     volumeSlider);
    trebleAtt    = std::make_unique<SliderAttachment> (audioProcessor.apvts, "TREBLE",     trebleSlider);
    midAtt       = std::make_unique<SliderAttachment> (audioProcessor.apvts, "MID",        midSlider);
    bassAtt      = std::make_unique<SliderAttachment> (audioProcessor.apvts, "BASS",       bassSlider);
    leadDriveAtt = std::make_unique<SliderAttachment> (audioProcessor.apvts, "LEAD_DRIVE", leadDriveSlider);
    masterAtt    = std::make_unique<SliderAttachment> (audioProcessor.apvts, "MASTER",     masterSlider);

    geq80Att     = std::make_unique<SliderAttachment> (audioProcessor.apvts, "GEQ_80",     geq80Slider);
    geq240Att    = std::make_unique<SliderAttachment> (audioProcessor.apvts, "GEQ_240",    geq240Slider);
    geq750Att    = std::make_unique<SliderAttachment> (audioProcessor.apvts, "GEQ_750",    geq750Slider);
    geq2200Att   = std::make_unique<SliderAttachment> (audioProcessor.apvts, "GEQ_2200",   geq2200Slider);
    geq6600Att   = std::make_unique<SliderAttachment> (audioProcessor.apvts, "GEQ_6600",   geq6600Slider);

    leadModeAtt  = std::make_unique<ButtonAttachment> (audioProcessor.apvts, "LEAD_MODE",  leadModeButton);
    cabSimAtt    = std::make_unique<ButtonAttachment> (audioProcessor.apvts, "CAB_SIM",    cabSimButton);

    setSize (860, 260);
}

StudioPreampAudioProcessorEditor::~StudioPreampAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

void StudioPreampAudioProcessorEditor::setupKnob (juce::Slider& slider, juce::Label& label, const juce::String& text)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible (slider);

    label.setText (text, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setFont (juce::FontOptions (12.0f, juce::Font::bold));
    label.setColour (juce::Label::textColourId, juce::Colour (0xffe0e0e0));
    addAndMakeVisible (label);
}

void StudioPreampAudioProcessorEditor::setupFader (juce::Slider& slider, juce::Label& label, const juce::String& text)
{
    slider.setSliderStyle (juce::Slider::LinearVertical);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible (slider);

    label.setText (text, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setFont (juce::FontOptions (11.0f, juce::Font::bold));
    label.setColour (juce::Label::textColourId, juce::Colour (0xffcccccc));
    addAndMakeVisible (label);
}

void StudioPreampAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff141414));

    g.setColour (juce::Colour (0xff3a3a3a));
    g.drawRect (getLocalBounds().reduced (6), 2);

    g.setColour (juce::Colour (0xff080808));
    g.fillEllipse (14, 24, 14, 14);
    g.fillEllipse (14, getHeight() - 38, 14, 14);
    g.fillEllipse (getWidth() - 28, 24, 14, 14);
    g.fillEllipse (getWidth() - 28, getHeight() - 38, 14, 14);

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (20.0f, juce::Font::bold));
    g.drawText ("BOOGIE STUDIO PREAMP", 42, 16, 320, 28, juce::Justification::left);

    g.setColour (juce::Colour (0xff888888));
    g.setFont (juce::FontOptions (11.0f, juce::Font::plain));
    g.drawText ("CALIBER SERIES / MARK IIC+ CIRCUIT TOPOLOGY", 42, 38, 320, 18, juce::Justification::left);

    auto eqBox = juce::Rectangle<int> (565, 22, 245, 216);
    g.setColour (juce::Colour (0xff1d1d1d));
    g.fillRoundedRectangle (eqBox.toFloat(), 4.0f);
    g.setColour (juce::Colour (0xff555555));
    g.drawRoundedRectangle (eqBox.toFloat(), 4.0f, 1.5f);

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (12.0f, juce::Font::bold));
    g.drawText ("5-BAND GRAPHIC EQ", eqBox.getX(), eqBox.getY() + 6, eqBox.getWidth(), 18, juce::Justification::centred);

    g.setColour (juce::Colour (0xff660000));
    g.fillEllipse (495, 24, 22, 22);
    g.setColour (juce::Colour (0xffff2222));
    g.fillEllipse (499, 28, 14, 14);
    g.setColour (juce::Colour (0xffffaaaa));
    g.fillEllipse (502, 31, 5, 5);
}

void StudioPreampAudioProcessorEditor::resized()
{
    int knobY = 85;
    int knobSize = 72;
    int labelY = 162;

    volumeSlider.setBounds    (45,  knobY, knobSize, knobSize);
    volumeLabel.setBounds     (45,  labelY, knobSize, 20);

    trebleSlider.setBounds    (125, knobY, knobSize, knobSize);
    trebleLabel.setBounds     (125, labelY, knobSize, 20);

    midSlider.setBounds       (205, knobY, knobSize, knobSize);
    midLabel.setBounds        (205, labelY, knobSize, 20);

    bassSlider.setBounds      (285, knobY, knobSize, knobSize);
    bassLabel.setBounds       (285, labelY, knobSize, 20);

    leadDriveSlider.setBounds (375, knobY, knobSize, knobSize);
    leadDriveLabel.setBounds  (375, labelY, knobSize, 20);

    masterSlider.setBounds    (460, knobY, knobSize, knobSize);
    masterLabel.setBounds     (460, labelY, knobSize, 20);

    leadModeButton.setBounds  (45,  200, 140, 26);
    cabSimButton.setBounds    (205, 200, 140, 26);

    int eqStartX = 580;
    int eqSpacing = 44;
    int faderY = 52;
    int faderH = 155;

    geq80Slider.setBounds   (eqStartX,                 faderY, 36, faderH);
    geq80Label.setBounds    (eqStartX,                 210,    36, 18);

    geq240Slider.setBounds  (eqStartX + eqSpacing,     faderY, 36, faderH);
    geq240Label.setBounds   (eqStartX + eqSpacing,     210,    36, 18);

    geq750Slider.setBounds  (eqStartX + eqSpacing * 2, faderY, 36, faderH);
    geq750Label.setBounds   (eqStartX + eqSpacing * 2, 210,    36, 18);

    geq2200Slider.setBounds (eqStartX + eqSpacing * 3, faderY, 36, faderH);
    geq2200Label.setBounds  (eqStartX + eqSpacing * 3, 210,    36, 18);

    geq6600Slider.setBounds (eqStartX + eqSpacing * 4, faderY, 36, faderH);
    geq6600Label.setBounds  (eqStartX + eqSpacing * 4, 210,    36, 18);
}
