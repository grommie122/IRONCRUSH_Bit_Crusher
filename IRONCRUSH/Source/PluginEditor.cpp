#include "PluginEditor.h"
#include <cmath>

void IroncrushAudioProcessorEditor::IndustrialLookAndFeel::drawRotarySlider(
    juce::Graphics& g, int x, int y, int width, int height, float position,
    float startAngle, float endAngle, juce::Slider&)
{
    auto bounds = juce::Rectangle<float>((float) x, (float) y, (float) width, (float) height).reduced(13.0f);
    const auto centre = bounds.getCentre();
    const float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.43f;
    g.setColour(juce::Colour::fromRGB(26, 31, 35));
    g.fillEllipse(centre.x - radius, centre.y - radius, radius * 2, radius * 2);
    g.setColour(juce::Colour::fromRGB(74, 82, 86));
    g.drawEllipse(centre.x - radius, centre.y - radius, radius * 2, radius * 2, 2.0f);
    juce::Path track;
    track.addCentredArc(centre.x, centre.y, radius + 5, radius + 5, 0, startAngle, endAngle, true);
    g.setColour(juce::Colour::fromRGB(64, 69, 72));
    g.strokePath(track, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    const float angle = startAngle + position * (endAngle - startAngle);
    juce::Path active;
    active.addCentredArc(centre.x, centre.y, radius + 5, radius + 5, 0, startAngle, angle, true);
    g.setColour(juce::Colour::fromRGB(246, 113, 43));
    g.strokePath(active, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    juce::Path indicator;
    indicator.addRectangle(-2.0f, -radius + 10.0f, 4.0f, radius * 0.45f);
    indicator.applyTransform(juce::AffineTransform::rotation(angle).translated(centre.x, centre.y));
    g.setColour(juce::Colours::white);
    g.fillPath(indicator);
}

IroncrushAudioProcessorEditor::IroncrushAudioProcessorEditor(IroncrushAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setLookAndFeel(&look);
    const char* ids[] = { "bits", "rate", "drive", "jitter", "cutoff", "mix", "output" };
    const char* names[] = { "BIT DEPTH", "SAMPLE CRUSH", "DRIVE", "JITTER", "FILTER", "DRY / WET", "OUTPUT" };
    for (size_t i = 0; i < knobs.size(); ++i)
    {
        auto& knob = knobs[i];
        knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 22);
        knob.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        knob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        knob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour::fromRGB(30, 34, 38));
        addAndMakeVisible(knob);
        auto& label = labels[i];
        label.setText(names[i], juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.setColour(juce::Label::textColourId, juce::Colour::fromRGB(190, 196, 198));
        label.setFont(juce::Font(juce::FontOptions(12.0f, juce::Font::bold)));
        addAndMakeVisible(label);
        attachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, ids[i], knob));
    }
    setSize(760, 390);
}

void IroncrushAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(17, 20, 23));
    g.setColour(juce::Colour::fromRGB(246, 113, 43));
    g.fillRect(0, 0, getWidth(), 6);
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(juce::FontOptions(38.0f, juce::Font::bold)));
    g.drawText("IRONCRUSH", 28, 22, 420, 54, juce::Justification::centredLeft);
    g.setColour(juce::Colour::fromRGB(246, 113, 43));
    g.setFont(juce::Font(juce::FontOptions(12.0f, juce::Font::bold)));
    g.drawText("DIGITAL DESTRUCTION UNIT // V1.0", 31, 74, 420, 22, juce::Justification::centredLeft);
    g.setColour(juce::Colour::fromRGB(58, 64, 68));
    g.drawLine(28.0f, 111.0f, (float) getWidth() - 28.0f, 111.0f, 1.0f);
    g.setColour(juce::Colour::fromRGB(90, 98, 101));
    g.setFont(juce::Font(juce::FontOptions(11.0f)));
    g.drawText("SIGNAL DAMAGE / INDUSTRIAL AUDIO PROCESSING", 28, 352, 500, 20, juce::Justification::centredLeft);
    g.drawText("IR // 001", 620, 352, 110, 20, juce::Justification::centredRight);
}

void IroncrushAudioProcessorEditor::resized()
{
    constexpr int margin = 30, cellWidth = 175;
    for (size_t i = 0; i < knobs.size(); ++i)
    {
        const int row = i < 4 ? 0 : 1;
        const int col = row == 0 ? (int) i : (int) i - 4;
        const int x = margin + col * cellWidth + (row == 1 ? 85 : 0);
        const int y = row == 0 ? 120 : 238;
        labels[i].setBounds(x, y, 150, 20);
        knobs[i].setBounds(x + 16, y + 18, 118, 96);
    }
}
