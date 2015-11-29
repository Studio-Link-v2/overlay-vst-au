/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <pthread.h>
#include "re/re.h"
#include "baresip.h"

pthread_t tid;
bool running = false;




//==============================================================================
StudioLinkAudioProcessor::StudioLinkAudioProcessor()
{

	(void)re_fprintf(stderr, "activate baresip v%s"
			" Copyright (C) 2010 - 2015"
			" Alfred E. Heggestad et al.\n",
			BARESIP_VERSION);

        if (!running) {
                
                (void)re_fprintf(stderr, "activate baresip v%s"
                                " Copyright (C) 2010 - 2015"
                                " Alfred E. Heggestad et al.\n",
                                BARESIP_VERSION);
                (void)sys_coredump_set(true);
                libre_init();
                conf_configure();
                ua_init("baresip v" BARESIP_VERSION " (" ARCH "/" OS ")",
                                true, true, true, false);
                conf_modules();
                pthread_create(&tid, NULL, (void*(*)(void*))&re_main, NULL);
                running = true;
        }

}

StudioLinkAudioProcessor::~StudioLinkAudioProcessor()
{
	if (running) {
		re_cancel();
		(void)pthread_join(tid, NULL);
		//ua_stop_all(false);
		ua_close();
		mod_close();
		libre_close();
		running = false;
	}
}

//==============================================================================
const String StudioLinkAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int StudioLinkAudioProcessor::getNumParameters()
{
    return 0;
}

float StudioLinkAudioProcessor::getParameter (int index)
{
    return 0.0f;
}

void StudioLinkAudioProcessor::setParameter (int index, float newValue)
{
}

const String StudioLinkAudioProcessor::getParameterName (int index)
{
    return String();
}

const String StudioLinkAudioProcessor::getParameterText (int index)
{
    return String();
}

const String StudioLinkAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String StudioLinkAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool StudioLinkAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool StudioLinkAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool StudioLinkAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StudioLinkAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StudioLinkAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double StudioLinkAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StudioLinkAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StudioLinkAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StudioLinkAudioProcessor::setCurrentProgram (int index)
{
}

const String StudioLinkAudioProcessor::getProgramName (int index)
{
    return String();
}

void StudioLinkAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void StudioLinkAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void StudioLinkAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void StudioLinkAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // I've added this to avoid people getting screaming feedback
    // when they first compile the plugin, but obviously you don't need to
    // this code if your algorithm already fills all the output channels.
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    const float* input0 = buffer.getReadPointer(0);
    float* output0 = buffer.getWritePointer(0);
    const float* input1 = input0;
    float* output1 = output0;

    if(getNumInputChannels() > 1) {
        input1 = buffer.getReadPointer(1);
        output1 = buffer.getWritePointer(1);
    } 

    effect_src(input0, input1, buffer.getNumSamples());
    effect_play(output0, output1, buffer.getNumSamples());

}

//==============================================================================
bool StudioLinkAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* StudioLinkAudioProcessor::createEditor()
{
    return new StudioLinkAudioProcessorEditor (*this);
}

//==============================================================================
void StudioLinkAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void StudioLinkAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StudioLinkAudioProcessor();
}
