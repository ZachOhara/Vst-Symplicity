#pragma once

#include <string>

#include "../JuceLibraryCode/JuceHeader.h"
#include "DebugLogger.h"
#include "TuningProcessor.h"
#include "Oscilator.h"

// Note that NUM_NOTES is also defined in TuningProcessor
static const int NUM_NOTES = 128;
static const int NUM_OSCILATORS = 3;

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
	SymplicitySynth();
	~SymplicitySynth();

	void PrepareToPlay(double, int);
	void ReleaseResources();
	void ProcessBlock(AudioSampleBuffer&, MidiBuffer&);

private:
	NoteStatus keyboard[NUM_NOTES];
	double *sampleBuffer;
	int blockSize;

	double secondsPerSample;
	bool isPedalOn = false;

	TuningProcessor tuningProcessor;
	Oscilator oscilators[NUM_OSCILATORS];

	void ProcessMidiMessages(MidiBuffer&);
	void SynthesizeAudio();
};

