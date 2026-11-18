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

    // 1. Skalenpunkte rund um den Drehregler (0 bis 10 Optik)
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

    // 2. Weicher Schlagschatten unter dem Knopf
    g.setColour (juce::Colours::black.withAlpha (0.65f));
    g.fillEllipse (centreX - radius + 2.0f, centreY - radius + 3.5f, radius * 2.0f, radius * 2.0f);

    // 3. Schwarzer geriffelter Außenkörper
    juce::ColourGradient outerGrad (juce::Colour (0xff2b2b2b), centreX - radius, centreY - radius,
                                    juce::Colour (0xff0d0d0d), centreX + radius, centreY + radius, false);
    g.setGradientFill (outerGrad);
    g.fillEllipse (centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f);

    // Feine Riffelung am Rand
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

    // 4. Gebürstete Silber-Kappe mit Metallic-Lichtverlauf
    auto capR = radius * 0.68f;
    juce::ColourGradient metalGrad (juce::Colour (0xfff2f2f2), centreX - capR, centreY - capR,
                                    juce::Colour (0xff8a8a8a), centreX + capR, centreY + capR, false);
    g.setGradientFill (metalGrad);
    g.fillEllipse (centreX - capR, centreY - capR, capR * 2.0f, capR * 2.0f);

    // Glanzrand der Silberkappe
    g.setColour (juce::Colour (0xffdddddd));
    g.drawEllipse (centreX - capR, centreY - capR, capR * 2.0f, capR * 2.0f, 1.0f);

    // 5. Weißer Zeigerstrich
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

    // Skalenstriche links und rechts vom Fader (+12, +6, 0, -6, -12 dB)
    for (int i = 0; i <= 4; ++i)
    {
        float lineY = (float) y + (i / 4.0f) * (float) height;
        bool isCenter = (i == 2);
        g.setColour (isCenter ? juce::Colour (0xff999999) : juce::Colour (0xff4a4a4a));
        float lineW = isCenter ? 11.0f : 7.0f;
        g.drawLine (centreX - lineW, lineY, centreX - 4.0f, lineY, isCenter ? 1.5f : 1.0f);
        g.drawLine (centreX + 4.0f, lineY, centreX + lineW, lineY, isCenter ? 1.5f : 1.0f);
    }

    // Versenkter Fader-Schlitz
    auto trackWidth = 4.5f;
    g.setColour (juce::Colour (0xff050505));
    g.fillRoundedRectangle (centreX - trackWidth * 0.5f, (float) y - 2.0f, trackWidth, (float) height + 4.0f, 2.0f);
    g.setColour (juce::Colour (0xff333333));
    g.drawRoundedRectangle (centreX - trackWidth * 0.5f, (float) y - 2.0f, trackWidth, (float) height + 4.0f, 2.0f, 0.8f);

    // 3D Boogie EQ-Schiebeknopf mit Schatten
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

    // Leuchtend weiße Mittelkerbe im Schieber
    g.setColour (juce::Colours::white);
    g.drawLine (centreX - thumbW * 0.38f, sliderPos, centreX + thumbW * 0.38f, sliderPos, 2.0f);
}

void BoogieLookAndFeel::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                                          bool, bool)
{
    auto bounds = button.getLocalBounds().toFloat();
    bool isOn = button.getToggleState();

    // Kleiner Verchromter Kippschalter (Toggle Switch) links
    float swX = 12.0f;
    float swY = bounds.getCentreY();

    // Sechskant-Mutter des Kippschalters
    g.setColour (juce::Colour (0xffaaaaaa));
    g.fillEllipse (swX - 7.0f, swY - 7.0f, 14.0f, 14.0f);
    g.setColour (juce::Colour (0xff222222));
    g.fillEllipse (swX - 4.0f, swY - 4.0f, 8.0f, 8.0f);

    // Metall-Hebel (zeigt nach oben wenn AN, nach unten wenn AUS)
    float leverEndY = isOn ? (swY - 10.0f) : (swY + 10.0f);
    juce::ColourGradient chromeGrad (juce::Colours::white, swX - 2.0f, swY,
                                     juce::Colour (0xff777777), swX + 3.0f, leverEndY, false);
    g.setGradientFill (chromeGrad);
    g.drawLine (swX, swY, swX, leverEndY, 4.0f);
    g.fillEllipse (swX - 2.5f, leverEndY - 2.5f, 5.0f, 5.0f);

    // Status-LED neben dem Schalter
    float ledX = swX + 16.0f;
    juce::Colour ledColour = isOn ? juce::Colour (0xffff3333) : juce::Colour (0xff22aa44);
    if (button.getButtonText().contains ("EVM12L"))
        ledColour = isOn ? juce::Colour (0xff2299ff) : juce::Colour (0xff333333);

    g.setColour (ledColour.withAlpha (0.35f));
    g.fillEllipse (ledX - 6.0f, swY - 6.0f, 12.0f, 12.0f);
    g.setColour (ledColour);
    g.fillEllipse (ledX - 3.5f, swY - 3.5f, 7.0f, 7.0f);

    // Beschriftung
    g.setColour (juce::Colour (0xffe0e0e0));
    g.setFont (juce::FontOptions (11.0f, juce::Font::bold));
    g.drawText (button.getButtonText(), (int) (ledX + 10.0f), 0, (int) (bounds.getWidth() - ledX - 10.0f), (int) bounds.getHeight(), juce::Justification::centredLeft);
}

StudioPreampAudioProcessorEditor::StudioPreampAudioProcessorEditor (StudioPreampAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel (&boogieLookAndFeel);

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

    volumeAtt    = std::make_unique<SliderAttachment> (audioProcessor.apvts, "VOLUME",        volumeSlider);
    trebleAtt    = std::make_unique<SliderAttachment> (audioProcessor.apvts, "TREBLE",        trebleSlider);
    midAtt       = std::make_unique<SliderAttachment> (audioProcessor.apvts, "MID",           midSlider);
    bassAtt      = std::make_unique<SliderAttachment> (audioProcessor.apvts, "BASS",          bassSlider);
    leadDriveAtt = std::make_unique<SliderAttachment> (audioProcessor.apvts, "LEAD_DRIVE",    leadDriveSlider);
    cabThumpAtt  = std::make_unique<SliderAttachment> (audioProcessor.apvts, "CAB_RESONANCE", cabThumpSlider);
