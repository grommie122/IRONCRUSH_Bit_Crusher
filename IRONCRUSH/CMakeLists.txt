cmake_minimum_required(VERSION 3.22)
project(IRONCRUSH VERSION 1.0.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)
FetchContent_Declare(JUCE GIT_REPOSITORY https://github.com/juce-framework/JUCE.git GIT_TAG 8.0.10 GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(JUCE)
juce_add_plugin(IRONCRUSH
    COMPANY_NAME "Ironcrush Audio"
    BUNDLE_ID "audio.ironcrush.plugin"
    PLUGIN_MANUFACTURER_CODE IrAu
    PLUGIN_CODE IrCr
    FORMATS VST3
    PRODUCT_NAME "IRONCRUSH"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE)
juce_generate_juce_header(IRONCRUSH)
target_sources(IRONCRUSH PRIVATE Source/PluginProcessor.cpp Source/PluginEditor.cpp)
target_compile_definitions(IRONCRUSH PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)
target_link_libraries(IRONCRUSH PRIVATE juce::juce_audio_utils juce::juce_dsp juce::juce_recommended_config_flags juce::juce_recommended_warning_flags)
