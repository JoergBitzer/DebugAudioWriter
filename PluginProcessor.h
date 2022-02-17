/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MemPool.h"
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

    void setClick(bool clickstatus){m_withClick = clickstatus; };

	
  	double m_fs;
	  int m_nrofchans;

// Alternative with abstract FIFO  
    MemPool m_poolLeft;
    MemPool m_poolRight;
    AbstractFifo m_fifocontrol;
    size_t m_maxSizeofBlock;
    bool m_withClick;


private:

  	double m_len_s;



    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DebugAudioWriterAudioProcessor)
};
