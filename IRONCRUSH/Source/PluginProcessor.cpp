#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

IroncrushAudioProcessor::IroncrushAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                      .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "IRONCRUSH_STATE", makeLayout()) {}

juce::AudioProcessorValueTreeState::ParameterLayout IroncrushAudioProcessor::makeLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    auto add = [&layout](const char* id, const char* name, float lo, float hi, float initial, float step)
    {
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID { id, 1 }, name, juce::NormalisableRange<float>(lo, hi, step), initial));
    };
    add("bits", "Bit Depth", 1.0f, 16.0f, 8.0f, 1.0f);
    add("rate", "Sample Rate", 500.0f, 44100.0f, 8000.0f, 1.0f);
    add("drive", "Drive", 0.0f, 30.0f, 6.0f, 0.1f);
    add("jitter", "Jitter", 0.0f, 100.0f, 0.0f, 0.1f);
    add("cutoff", "Lowpass", 300.0f, 20000.0f, 12000.0f, 1.0f);
    add("mix", "Dry / Wet", 0.0f, 100.0f, 100.0f, 0.1f);
    add("output", "Output", -24.0f, 12.0f, 0.0f, 0.1f);
    return layout;
}

bool IroncrushAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto input = layouts.getMainInputChannelSet();
    return (input == juce::AudioChannelSet::mono() || input == juce::AudioChannelSet::stereo())
        && layouts.getMainOutputChannelSet() == input;
}

void IroncrushAudioProcessor::prepareToPlay(double sampleRate, int)
{
    currentSampleRate = sampleRate;
    channels = {};
}

void IroncrushAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int n = buffer.getNumSamples();
    const int numChannels = juce::jmin(buffer.getNumChannels(), 2);
    const int bits = juce::jlimit(1, 16, (int) std::round(parameters.getRawParameterValue("bits")->load()));
    const float rate = parameters.getRawParameterValue("rate")->load();
    const float drive = juce::Decibels::decibelsToGain(parameters.getRawParameterValue("drive")->load());
    const float jitter = parameters.getRawParameterValue("jitter")->load() / 100.0f;
    const float cutoff = parameters.getRawParameterValue("cutoff")->load();
    const float mix = parameters.getRawParameterValue("mix")->load() / 100.0f;
    const float gain = juce::Decibels::decibelsToGain(parameters.getRawParameterValue("output")->load());
    const float levels = (float) (1 << (bits - 1));
    const float baseHold = (float) (currentSampleRate / juce::jmin(rate, (float) currentSampleRate));
    const float alpha = 1.0f - std::exp(-juce::MathConstants<float>::twoPi *
                          juce::jmin(cutoff, (float) currentSampleRate * 0.49f) / (float) currentSampleRate);
    for (int c = 0; c < numChannels; ++c)
    {
        auto* samples = buffer.getWritePointer(c);
        auto& state = channels[(size_t) c];
        for (int i = 0; i < n; ++i)
        {
            const float dry = samples[i];
            if (state.countdown <= 0)
            {
                const float driven = std::tanh(dry * drive);
                state.held = juce::jlimit(-1.0f, 1.0f - 1.0f / levels,
                                         std::round(driven * levels) / levels);
                const float randomizedHold = baseHold * (1.0f + jitter * uniform(random) * 0.9f);
                state.countdown = juce::jmax(1, (int) std::round(randomizedHold));
            }
            --state.countdown;
            state.lowpass += alpha * (state.held - state.lowpass);
            samples[i] = (dry * (1.0f - mix) + state.lowpass * mix) * gain;
        }
    }
}

void IroncrushAudioProcessor::getStateInformation(juce::MemoryBlock& dest)
{
    if (auto xml = parameters.copyState().createXml()) copyXmlToBinary(*xml, dest);
}

void IroncrushAudioProcessor::setStateInformation(const void* data, int size)
{
    if (auto xml = getXmlFromBinary(data, size))
        if (xml->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessorEditor*
IroncrushAudioProcessor::createEditor()
{
    return new IroncrushAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new IroncrushAudioProcessor(); }
