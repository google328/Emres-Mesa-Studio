#include "PluginProcessor.h"
#include "PluginEditor.h"

void BoogieLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                          juce::Slider&)
{
    auto radius = (float) juce::jmin (width / 2, height / 2) - 5.0f;
    auto centreX = (float) x + (float) width  * 0.5f;
    auto centreY = (float) y + (float) height * 0.5f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    g.setColour (juce::Colour (0xff777777));
    for (int i = 0; i <= 10; ++i)
    {
        float tickAngle = rotaryStartAngle + (i / 10.0f) * (rotaryEndAngle - rotaryStartAngle);
        float innerTick = radius + 2.0f;
        float outerTick = (i % 5 == 0) ? radius + 5.5f : radius + 3.8f;
        float x1 = centreX + std::sin (tickAngle) * innerTick;
        float y1 = centreY - std::cos (tickAngle) * innerTick;
        float x2 = centreX + std::sin (tickAngle) * outerTick;
        float y2 = centreY - std::cos (tickAngle) * outerTick;
        g.drawLine (x1, y1, x2, y2, (i % 5 == 0) ? 1.5f : 1.0f);
    }

    g.setColour (juce::Colours::black.withAlpha (0.65f));
    g.fillEllipse (centreX - radius + 2.0f, centreY - radius + 3.5f, radius * 2.0f, radius * 2.0f);

    juce::ColourGradient outerGrad (juce::Colour (0xff2b2b2b), centreX - radius, centreY - radius,
                                    juce::Colour (0xff0d0d0d), centreX + radius, centreY + radius, false);
    g.setGradientFill (outerGrad);
    g.fillEllipse (centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f);

    g.setColour (juce::Colour (0xff151515));
    for (int i = 0; i < 24; ++i)
    {
        float notchAngle = angle + (i * juce::MathConstants<float>::twoPi / 24.0f);
        float nx1 = centreX + std::sin (notchAngle) * (radius * 0.76f);
        float ny1 = centreY - std::cos (notchAngle) * (radius * 0.76f);
        float nx2 = centreX + std::sin (notchAngle) * (radius * 0.98f);
        float ny2 = centreY - std::cos (notchAngle) * (radius * 0.98f);
        g.drawLine (nx1, ny1, nx2, ny2, 1.2f);
    }

    auto capR = radius * 0.68f;
    juce::ColourGradient metalGrad (juce::Colour (0xfff2f2f2), centreX - capR, centreY - capR,
                                    juce::Colour (0xff8a8a8a), centreX + capR, centreY + capR, false);
    g.setGradientFill (metalGrad);
    g.fillEllipse (centreX - capR, centreY - capR, capR * 2.0f, capR * 2.0f);

    g.setColour (juce::Colour (0xffdddddd));
    g.drawEllipse (centreX - capR, centreY - capR, capR * 2.0f, capR * 2.0f, 1.0f);

    juce::Path p;
    auto pointerLength = radius * 0.92f;
    auto pointerThickness = 2.8f;
    p.addRoundedRectangle (-pointerThickness * 0.5f, -radius + 1.0f, pointerThickness, pointerLength * 0.55f, 1.0f);
    p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
    g.setColour (juce::Colours::white);
    g.fillPath (p);
}

void BoogieLookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                                          float sliderPos, float, float,
                                          const juce::Slider::SliderStyle, juce::Slider&)
{
    auto centreX = (float) x + (float) width * 0.5f;

    for (int i = 0; i <= 4; ++i)
    {
        float lineY = (float) y + (i / 4.0f) * (float) height;
        bool isCenter = (i == 2);
        g.setColour (isCenter ? juce::Colour (0xff999999) : juce::Colour (0xff4a4a4a));
        float lineW = isCenter ? 11.0f : 7.0f;
        g.drawLine (centreX - lineW, lineY, centreX - 4.0f, lineY, isCenter ? 1.5f : 1.0f);
        g.drawLine (centreX + 4.0f, lineY, centreX + lineW, lineY, isCenter ? 1.5f : 1.0f);
    }

    auto trackWidth = 4.5f;
    g.setColour (juce::Colour (0xff050505));
    g.fillRoundedRectangle (centreX - trackWidth * 0.5f, (float) y - 2.0f, trackWidth, (float) height + 4.0f, 2.0f);
    g.setColour (juce::Colour (0xff333333));
    g.drawRoundedRectangle (centreX - trackWidth * 0.5f, (float) y - 2.0f, trackWidth, (float) height + 4.0f, 2.0f, 0.8f);

    auto thumbW = 22.0f;
    auto thumbH = 13.0f;
    g.setColour (juce::Colours::black.withAlpha (0.7f));
    g.fillRoundedRectangle (centreX - thumbW * 0.5f + 1.5f, sliderPos - thumbH * 0.5f + 2.0f, thumbW, thumbH, 2.5f);

    juce::ColourGradient thumbGrad (juce::Colour (0xff3c3c3c), centreX, sliderPos - thumbH * 0.5f,
                                    juce::Colour (0xff181818), centreX, sliderPos + thumbH * 0.5f, false);
    g.setGradientFill (thumbGrad);
    g.fillRoundedRectangle (centreX - thumbW * 0.5f, sliderPos - thumbH * 0.5f, thumbW, thumbH, 2.5f);

    g.setColour (juce::Colour (0xff666666));
    g.drawRoundedRectangle (centreX - thumbW * 0.5f, sliderPos - thumbH * 0.5f, thumbW, thumbH, 2.5f, 1.0f);

    g.setColour (juce::Colours::white);
    g.drawLine (centreX - thumbW * 0.38f, sliderPos, centreX + thumbW * 0.38f, sliderPos, 2.0f);
}

void BoogieLookAndFeel::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                                          bool, bool)
{
    auto bounds = button.getLocalBounds().toFloat();
    bool isOn = button.getToggleState();

    float swX = 12.0f;
    float swY = bounds.getCentreY();

    g.setColour (juce::Colour (0xffaaaaaa));
    g.fillEllipse (swX - 7.0f, swY - 7.0f, 14.0f, 14.0f);
    g.setColour (juce::Colour (0xff222222));
    g.fillEllipse (swX - 4.0f, swY - 4.0f, 8.0f, 8.0f);

    float leverEndY = isOn ? (swY - 10.0f) : (swY + 10.0f);
    juce::ColourGradient chromeGrad (juce::Colours::white, swX - 2.0f, swY,
                                     juce::Colour (0xff777777), swX + 3.0f, leverEndY, false);
    g.setGradientFill (chromeGrad);
    g.drawLine (swX, swY, swX, leverEndY, 4.0f);
    g.fillEllipse (swX - 2.5f, leverEndY - 2.5f, 5.0f, 5.0f);

    float ledX = swX + 16.0f;
    juce::Colour ledColour = isOn ? juce::Colour (0xffff3333) : juce::Colour (0xff22aa44);
    if (button.getButtonText().contains ("EVM12L"))
        ledColour = isOn ? juce::Colour (0xff2299ff) : juce::Colour (0xff333333);

    g.setColour (ledColour.withAlpha (0.35f));
    g.fillEllipse (ledX - 6.0f, swY - 6.0f, 12.0f, 12.0f);
    g.setColour (ledColour);
    g.fillEllipse (ledX - 3.5f, swY - 3.5f, 7.0f, 7.0f);

    g.setColour (juce::Colour (0xffe0e0e0));
    g.setFont (juce::FontOptions (11.0f, juce::Font::bold));
    g.drawText (button.getButtonText(), (int) (ledX + 10.0f), 0, (int) (bounds.getWidth() - ledX - 10.0f), (int) bounds.getHeight(), juce::Justification::centredLeft);
}

StudioPreampAudioProcessorEditor::StudioPreampAudioProcessorEditor (StudioPreampAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel (&boogieLookAndFeel);

    setupKnob (inputSlider,     inputLabel,     "INPUT");
    setupKnob (volumeSlider,    volumeLabel,    "VOLUME");
    setupKnob (trebleSlider,    trebleLabel,    "TREBLE");
    setupKnob (midSlider,       midLabel,       "MIDDLE");
    setupKnob (bassSlider,      bassLabel,      "BASS");
    setupKnob (leadDriveSlider, leadDriveLabel, "DRIVE");
    setupKnob (cabThumpSlider,  cabThumpLabel,  "THUMP");
    setupKnob (masterSlider,    masterLabel,    "MASTER");

    setupFader (geq80Slider,   geq80Label,   "80");
    setupFader (geq240Slider,  geq240Label,  "240");
    setupFader (geq750Slider,  geq750Label,  "750");
    setupFader (geq2200Slider, geq2200Label, "2200");
    setupFader (geq6600Slider, geq6600Label, "6600");

    addAndMakeVisible (leadModeButton);
    addAndMakeVisible (cabSimButton);

    loadIRButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xff262626));
    loadIRButton.setColour (juce::TextButton::textColourOffId, juce::Colour (0xffe0e0e0));
    addAndMakeVisible (loadIRButton);

    loadIRButton.onClick = [this]()
    {
        fileChooser = std::make_unique<juce::FileChooser> ("Waehle eine Mesa 4x12 EVM12L Impulse Response (.wav)...",
                                                           juce::File{},
                                                           "*.wav");
        auto flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        fileChooser->launchAsync (flags, [this] (const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file.existsAsFile())
            {
                audioProcessor.loadImpulseResponse (file);
                loadIRButton.setButtonText (file.getFileNameWithoutExtension());
            }
        });
    };

    inputAtt     = std::make_unique<SliderAttachment> (audioProcessor.apvts, "INPUT_GAIN",    inputSlider);
    volumeAtt    = std::make_unique<SliderAttachment> (audioProcessor.apvts, "VOLUME",        volumeSlider);
    trebleAtt    = std::make_unique<SliderAttachment> (audioProcessor.apvts, "TREBLE",        trebleSlider);
    midAtt       = std::make_unique<SliderAttachment> (audioProcessor.apvts, "MID",           midSlider);
    bassAtt      = std::make_unique<SliderAttachment> (audioProcessor.apvts, "BASS",          bassSlider);
    leadDriveAtt = std::make_unique<SliderAttachment> (audioProcessor.apvts, "LEAD_DRIVE",    leadDriveSlider);
    cabThumpAtt  = std::make_unique<SliderAttachment> (audioProcessor.apvts, "CAB_RESONANCE", cabThumpSlider);
    masterAtt    = std::make_unique<SliderAttachment> (audioProcessor.apvts, "MASTER",        masterSlider);

    geq80Att     = std::make_unique<SliderAttachment> (audioProcessor.apvts, "GEQ_80",        geq80Slider);
    geq240Att    = std::make_unique<SliderAttachment> (audioProcessor.apvts, "GEQ_240",       geq240Slider);
    geq750Att    = std::make_unique<SliderAttachment> (audioProcessor.apvts, "GEQ_750",       geq750Slider);
    geq2200Att   = std::make_unique<SliderAttachment> (audioProcessor.apvts, "GEQ_2200",      geq2200Slider);
    geq6600Att   = std::make_unique<SliderAttachment> (audioProcessor.apvts, "GEQ_6600",      geq6600Slider);

    leadModeAtt  = std::make_unique<ButtonAttachment> (audioProcessor.apvts, "LEAD_MODE",     leadModeButton);
    cabSimAtt    = std::make_unique<ButtonAttachment> (audioProcessor.apvts, "CAB_SIM",       cabSimButton);

    setSize (840, 260);
    setResizable (true, true);
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
    label.setFont (juce::FontOptions (10.5f, juce::Font::bold));
    label.setColour (juce::Label::textColourId, juce::Colour (0xffdddddd));
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
    juce::ColourGradient chassisGrad (juce::Colour (0xff1c1c1c), 0.0f, 0.0f,
                                      juce::Colour (0xff0f0f0f), 0.0f, (float) getHeight(), false);
    g.setGradientFill (chassisGrad);
    g.fillAll();

    g.setColour (juce::Colours::white.withAlpha (0.015f));
    for (int y = 0; y < getHeight(); y += 2)
        g.drawHorizontalLine (y, 0.0f, (float) getWidth());

    g.setColour (juce::Colour (0xff484848));
    g.drawRect (getLocalBounds().reduced (5), 1.5f);
    g.setColour (juce::Colour (0xff262626));
    g.drawRect (getLocalBounds().reduced (8), 1.0f);

    auto drawScrew = [&g] (float sx, float sy)
    {
        g.setColour (juce::Colour (0xff080808));
        g.fillEllipse (sx - 1.0f, sy - 1.0f, 14.0f, 14.0f);
        juce::ColourGradient screwGrad (juce::Colour (0xff555555), sx, sy,
                                        juce::Colour (0xff1c1c1c), sx + 12.0f, sy + 12.0f, false);
        g.setGradientFill (screwGrad);
        g.fillEllipse (sx, sy, 12.0f, 12.0f);
        g.setColour (juce::Colour (0xff0a0a0a));
        g.drawLine (sx + 3.0f, sy + 6.0f, sx + 9.0f, sy + 6.0f, 1.5f);
        g.drawLine (sx + 6.0f, sy + 3.0f, sx + 6.0f, sy + 9.0f, 1.5f);
    };

    drawScrew (12.0f, 20.0f);
    drawScrew (12.0f, (float) getHeight() - 32.0f);
    drawScrew ((float) getWidth() - 24.0f, 20.0f);
    drawScrew ((float) getWidth() - 24.0f, (float) getHeight() - 32.0f);

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (19.0f, juce::Font::bold));
    g.drawText ("BOOGIE STUDIO PREAMP", 34, 14, 340, 26, juce::Justification::left);

    g.setColour (juce::Colour (0xff999999));
    g.setFont (juce::FontOptions (10.5f, juce::Font::bold));
    g.drawText ("MARK IIC+ PREAMP  //  MESA BIG CAB 4x12 EVM12L", 34, 36, 380, 16, juce::Justification::left);

    g.setColour (juce::Colour (0xff333333));
    g.drawVerticalLine (94, 78.0f, 165.0f);
    g.drawVerticalLine (418, 78.0f, 165.0f);

    auto eqBox = juce::Rectangle<int> (575, 20, 230, 220);
    g.setColour (juce::Colour (0xff161616));
    g.fillRoundedRectangle (eqBox.toFloat(), 5.0f);
    g.setColour (juce::Colour (0xff4d4d4d));
    g.drawRoundedRectangle (eqBox.toFloat(), 5.0f, 1.5f);

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (11.5f, juce::Font::bold));
    g.drawText ("5-BAND GRAPHIC EQ", eqBox.getX(), eqBox.getY() + 6, eqBox.getWidth(), 18, juce::Justification::centred);

    float jx = 525.0f, jy = 22.0f;
    g.setColour (juce::Colour (0xffff1111).withAlpha (0.25f));
    g.fillEllipse (jx - 5.0f, jy - 5.0f, 30.0f, 30.0f);
    g.setColour (juce::Colour (0xff888888));
    g.drawEllipse (jx - 1.0f, jy - 1.0f, 22.0f, 22.0f, 1.5f);
    juce::ColourGradient jewelGrad (juce::Colour (0xffff6666), jx + 5.0f, jy + 5.0f,
                                    juce::Colour (0xff770000), jx + 18.0f, jy + 18.0f, true);
    g.setGradientFill (jewelGrad);
    g.fillEllipse (jx, jy, 20.0f, 20.0f);
}

void StudioPreampAudioProcessorEditor::resized()
{
    int knobY = 82;
    int knobSize = 54;
    int labelY = 140;
    int startX = 30;
    int spacing = 62;

    inputSlider.setBounds     (startX, knobY, knobSize, knobSize);
    inputLabel.setBounds      (startX, labelY, knobSize, 18);

       int preStartX = startX + spacing + 8;
    volumeSlider.setBounds    (preStartX,               knobY, knobSize, knobSize);
    volumeLabel.setBounds     (preStartX,               labelY, knobSize, 18);

    trebleSlider.setBounds    (preStartX + spacing,     knobY, knobSize, knobSize);
    trebleLabel.setBounds     (preStartX + spacing,     labelY, knobSize, 18);

    midSlider.setBounds       (preStartX + spacing * 2, knobY, knobSize, knobSize);
    midLabel.setBounds        (preStartX + spacing * 2, labelY, knobSize, 18);

    bassSlider.setBounds      (preStartX + spacing * 3, knobY, knobSize, knobSize);
    bassLabel.setBounds       (preStartX + spacing * 3, labelY, knobSize, 18);

    leadDriveSlider.setBounds (preStartX + spacing * 4, knobY, knobSize, knobSize);
    leadDriveLabel.setBounds  (preStartX + spacing * 4, labelY, knobSize, 18);

    int outStartX = preStartX + spacing * 5 + 8;
    cabThumpSlider.setBounds  (outStartX,               knobY, knobSize, knobSize);
    cabThumpLabel.setBounds   (outStartX,               labelY, knobSize, 18);

    masterSlider.setBounds    (outStartX + spacing,     knobY, knobSize, knobSize);
    masterLabel.setBounds     (outStartX + spacing,     labelY, knobSize, 18);

    leadModeButton.setBounds  (30,  195, 145, 28);
    cabSimButton.setBounds    (185, 195, 140, 28);
    loadIRButton.setBounds    (340, 195, 185, 26);

    int eqStartX = 588;
    int eqSpacing = 42;
    int faderY = 50;
    int faderH = 158;

    geq80Slider.setBounds   (eqStartX,                 faderY, 34, faderH);
    geq80Label.setBounds    (eqStartX,                 212,    34, 18);

    geq240Slider.setBounds  (eqStartX + eqSpacing,     faderY, 34, faderH);
    geq240Label.setBounds   (eqStartX + eqSpacing,     212,    34, 18);

    geq750Slider.setBounds  (eqStartX + eqSpacing * 2, faderY, 34, faderH);
    geq750Label.setBounds   (eqStartX + eqSpacing * 2, 212,    34, 18);

    geq2200Slider.setBounds (eqStartX + eqSpacing * 3, faderY, 34, faderH);
    geq2200Label.setBounds  (eqStartX + eqSpacing * 3, 212,    34, 18);

    geq6600Slider.setBounds (eqStartX + eqSpacing * 4, faderY, 34, faderH);
    geq6600Label.setBounds  (eqStartX + eqSpacing * 4, 212,    34, 18);
}
