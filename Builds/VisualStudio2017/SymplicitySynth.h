#pragma once

#include <string>

#include "../JuceLibraryCode/JuceHeader.h"
#include "Constants.h"
#include "DebugLogger.h"
#include "TuningProcessor.h"
#include "Oscilator.h"
#include "OscMixer.h"
#include "FilterProcessor.h"
#include "SymplicityEditor.h"

enum PedalState {
	INACTIVE,
	KEY_PRESSED,
	PEDALED_ONLY,
};

enum MidiMessageType {
	INVALID,
	NOTE_ON,
	NOTE_OFF,
	PEDAL_ON,
	PEDAL_OFF,
};

static MidiMessageType GetMessageType(MidiMessage&);

struct NoteStatus
{
	bool isPlaying = false;
	PedalState pedalState = INACTIVE;
	//EnvelopeState envelopeState;
	double phase[NUM_OSCILATORS];

	void Begin(double eventTime)
	{
		eventTime++; // TODO remove this
		isPlaying = true;
		for (int i = 0; i < NUM_OSCILATORS; i++)
		{
			phase[i] = 0;
		}
	}

	void Release(double eventTime)
	{
		isPlaying = false;
	}
};

class SymplicitySynth
{
public:
	SymplicitySynth(AudioProcessor &);
	~SymplicitySynth();

	void PrepareToPlay(double, int);
	void ReleaseResources();
	void ProcessBlock(AudioSampleBuffer &, MidiBuffer &);

	AudioProcessorEditor & ConstructEditor(AudioProcessor &);

private:
	NoteStatus keyboard[NUM_NOTES];
	double *sampleBuffer;
	int blockSize;

	double secondsPerSample;
	bool isPedalOn = false;

	TuningProcessor tuningProcessor;
	std::vector<Oscilator*> oscilators;
	OscMixer oscMixer;
	FilterProcessor filterProcessor;

	std::vector<SynthModule*> modules;
	
	void ProcessMidiMessages(MidiBuffer &);
	void SynthesizeAudio();
};
