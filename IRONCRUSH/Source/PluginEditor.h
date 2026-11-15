#pragma once
#include "PluginProcessor.h"

class IroncrushAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit IroncrushAudioProcessorEditor(IroncrushAudioProcessor&);
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    class IndustrialLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        void drawRotarySlider(juce::Graphics&, int, int, int, int, float, float, float, juce::Slider&) override;
    } look;
    IroncrushAudioProcessor& processor;
    std::array<juce::Slider, 7> knobs;
    std::array<juce::Label, 7> labels;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> attachments;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IroncrushAudioProcessorEditor)
};
