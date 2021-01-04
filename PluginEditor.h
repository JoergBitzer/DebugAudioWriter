/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DebugAudioWriterAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    DebugAudioWriterAudioProcessorEditor (DebugAudioWriterAudioProcessor&);
    ~DebugAudioWriterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DebugAudioWriterAudioProcessor& processor;

	TextButton m_button;
	void saveAudio();

	TextButton m_insertButton;
	void insertClick();
	bool m_insertclick;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DebugAudioWriterAudioProcessorEditor)
};
