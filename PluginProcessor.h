/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class DebugAudioWriterAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DebugAudioWriterAudioProcessor();
    ~DebugAudioWriterAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	std::queue<std::vector<float>> m_dataLeft;
	std::queue<std::vector<float>> m_dataRight;
	int m_datachunks;
	int m_counter;
	CriticalSection m_writer;
	double m_fs;
	int m_nrofchans;
private:
	std::vector<float> m_data;

	double m_len_s;


/*	File m_outfiledir;
	File m_outfilename;
	std::unique_ptr<FileOutputStream> m_outstream;
	AudioFormatWriter *m_wavewriter;
	WavAudioFormat wav;//*/

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DebugAudioWriterAudioProcessor)
};
