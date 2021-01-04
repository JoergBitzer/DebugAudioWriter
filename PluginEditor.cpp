/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DebugAudioWriterAudioProcessorEditor::DebugAudioWriterAudioProcessorEditor (DebugAudioWriterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), m_insertclick(false)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

	m_button.setButtonText("Save");
	m_button.onClick = [this]() {saveAudio(); };
	addAndMakeVisible(m_button);

	m_insertButton.setButtonText("set Endmark");
	m_insertButton.onClick = [this]() {insertClick(); };
	addAndMakeVisible(m_insertButton);

    setSize (300, 100);
}

DebugAudioWriterAudioProcessorEditor::~DebugAudioWriterAudioProcessorEditor()
{
}

//==============================================================================
void DebugAudioWriterAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
	if (!m_insertclick)
	{
		m_insertButton.setColour(TextButton::ColourIds::buttonColourId, Colours::darkgrey);
		m_insertButton.setButtonText("set Endmark");
	}
	else
	{
		m_insertButton.setColour(TextButton::ColourIds::buttonColourId, Colours::darkred);
		m_insertButton.setButtonText("unset");
	}

}

void DebugAudioWriterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

	m_button.setBounds(10, 20, 180, 60);
	m_insertButton.setBounds(200, 20, 90, 60);
}

void DebugAudioWriterAudioProcessorEditor::saveAudio()
{
//	ScopedLock(processor.m_writer);

	Time timeobj;

	auto tt = timeobj.getCurrentTime();

	auto ms = tt.toMilliseconds();
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
   //define something for Windows (32-bit and 64-bit, this part is common)
   String  dirname= "c:\\temp\\";
#elif __APPLE__
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
         // iOS Simulator
    #elif TARGET_OS_IPHONE
        // iOS device
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
		String  dirname= "~/Temp/";
    #else
    #   error "Unknown Apple platform"
    #endif
#elif __linux__
	String  dirname= "~/Temp/";
    // linux
#elif __unix__ // all unices not caught above
	String  dirname= "~/Temp/";
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#   error "Unknown compiler"
#endif

	
	File myFolder(dirname);
	if (myFolder.isDirectory() == false)
	{
		bool b = myFolder.createDirectory();
		if (b == false)
			return;
	}

	auto outfilename = String(dirname + String(ms) + ".wav");
	File outfile(outfilename);

	auto fileStream = std::unique_ptr<FileOutputStream>(outfile.createOutputStream());
	if (fileStream == nullptr)
		return;

	WavAudioFormat wavFormat;
		
	AudioFormatWriter *writer;
	if (processor.m_nrofchans == 1)
		writer = wavFormat.createWriterFor(fileStream.get(), processor.m_fs, 1, 16, {}, 0);
	
	if (processor.m_nrofchans == 2)
		writer = wavFormat.createWriterFor(fileStream.get(), processor.m_fs, 2, 16, {}, 0);

	fileStream.release();

	std::vector<float> dataL;
	std::vector<float> dataR;

	processor.m_writer.enter();
	while(processor.m_dataLeft.size() > 0)
	{
		float* pData[2];
		dataL = processor.m_dataLeft.front();
		if (m_insertclick)
			dataL.at(dataL.size()-1) = 1.0;
		
		processor.m_dataLeft.pop();
		pData[0] = &dataL[0];
		if (processor.m_nrofchans == 2)
		{
			dataR = processor.m_dataRight.front();
			pData[1] = &dataR[0];
			processor.m_dataRight.pop();
		}
		writer->writeFromFloatArrays(pData, processor.m_nrofchans, dataL.size());
	}

	processor.m_counter = 0;
	writer->flush();
	delete writer;
	processor.m_writer.exit();
}

void DebugAudioWriterAudioProcessorEditor::insertClick()
{
	m_insertclick = !m_insertclick;
	repaint();
}
