/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DebugAudioWriterAudioProcessor::DebugAudioWriterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     :m_fifocontrol(1), AudioProcessor (BusesProperties()
//     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif

{
}

DebugAudioWriterAudioProcessor::~DebugAudioWriterAudioProcessor()
{
}

//==============================================================================
const String DebugAudioWriterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DebugAudioWriterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DebugAudioWriterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DebugAudioWriterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DebugAudioWriterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DebugAudioWriterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DebugAudioWriterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DebugAudioWriterAudioProcessor::setCurrentProgram (int index)
{
}

const String DebugAudioWriterAudioProcessor::getProgramName (int index)
{
    return {};
}

void DebugAudioWriterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DebugAudioWriterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    m_withClick = 0;
	m_fs = sampleRate;
    m_maxSizeofBlock = samplesPerBlock;
	m_len_s = 10;

    int nrofelements = (m_len_s)*m_fs + samplesPerBlock + 1; 
    m_poolLeft.setMemSize(nrofelements*sizeof(float));
    m_poolRight.setMemSize(nrofelements*sizeof(float));
    m_fifocontrol.setTotalSize(nrofelements);
    m_fifocontrol.reset();


	// Use this method as the place to do any pre-playback
    // initialisation that you need..
/*	Time timeobj;

	auto tt = timeobj.getCurrentTime();

	String timestr = tt.toString(true,true,true,true);
	String  dirname= "c:\\temp\\";
	m_outfilename = String(dirname + "test" + ".wav");
	m_outstream = std::make_unique<FileOutputStream> (m_outfilename);
	

	m_wavewriter = wav.createWriterFor(m_outstream.get(), sampleRate, 2, 16, {}, 0);
*/	
}


void DebugAudioWriterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DebugAudioWriterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DebugAudioWriterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

	//ScopedLock sp(m_writer);
//	m_writer.enter();


	int nrofsamples = buffer.getNumSamples();
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    m_nrofchans = totalNumOutputChannels;
    if (m_nrofchans>2)
        m_nrofchans = 2;

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
   
    //auto fifo = m_fifocontrol.write(nrofsamples);
    int startindex1,startindex2,blockSize1, blockSize2;
    m_fifocontrol.prepareToWrite(nrofsamples,startindex1,blockSize1,startindex2,blockSize2);
    
	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);
        if(m_withClick) channelData[0] = 0.5f;
        if (channel == 0)
        {
            memcpy(m_poolLeft.getDirectAccessAt(startindex1*sizeof(float)),channelData,blockSize1*sizeof(float));
            memcpy(m_poolLeft.getDirectAccessAt(startindex2*sizeof(float)),channelData,blockSize2*sizeof(float));
        }
        if (channel == 1)
        {
            memcpy(m_poolRight.getDirectAccessAt(startindex1*sizeof(float)),channelData,blockSize1*sizeof(float));
            memcpy(m_poolRight.getDirectAccessAt(startindex2*sizeof(float)),channelData,blockSize2*sizeof(float));
        }

	}
    m_fifocontrol.finishedWrite (nrofsamples);

    auto remaining = m_fifocontrol.getFreeSpace();
    if (remaining < m_maxSizeofBlock)
        m_fifocontrol.read(m_maxSizeofBlock);

//	m_writer.exit();
}

//==============================================================================
bool DebugAudioWriterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DebugAudioWriterAudioProcessor::createEditor()
{
    return new DebugAudioWriterAudioProcessorEditor (*this);
}

//==============================================================================
void DebugAudioWriterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DebugAudioWriterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DebugAudioWriterAudioProcessor();
}
