cmake_minimum_required(VERSION 3.15)
project(BoogieStudioPreamp VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(JUCE)

juce_add_plugin(BoogieStudioPreamp
    COMPANY_NAME "CustomAudio"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    PLUGIN_MANUFACTURER_CODE Manu
    PLUGIN_CODE BgSp
    FORMATS VST3 Standalone
    PRODUCT_NAME "Boogie Studio Preamp"
)

juce_generate_juce_header(BoogieStudioPreamp)

target_sources(BoogieStudioPreamp
    PRIVATE
        PluginProcessor.cpp
        PluginProcessor.h
        PluginEditor.cpp
        PluginEditor.h
)

target_compile_definitions(BoogieStudioPreamp
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(BoogieStudioPreamp
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
