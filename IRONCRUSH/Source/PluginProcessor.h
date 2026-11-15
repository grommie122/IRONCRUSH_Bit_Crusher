#pragma once
#include <JuceHeader.h>
#include <random>

class IroncrushAudioProcessor final : public juce::AudioProcessor
{
public:
    IroncrushAudioProcessor();
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return "IRONCRUSH"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;
    bool isBusesLayoutSupported(const BusesLayout&) const override;
    juce::AudioProcessorValueTreeState parameters;
private:
    static juce::AudioProcessorValueTreeState::ParameterLayout makeLayout();
    struct ChannelState { float held = 0.0f, lowpass = 0.0f; int countdown = 0; };
    std::array<ChannelState, 2> channels {};
    std::minstd_rand random { 0x1A0C }; // deterministic, no allocation in audio thread
    std::uniform_real_distribution<float> uniform { -1.0f, 1.0f };
    double currentSampleRate = 44100.0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IroncrushAudioProcessor)
};
