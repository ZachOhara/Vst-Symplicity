#pragma once

#include <string>
#include <chrono>

#include "../JuceLibraryCode/JuceHeader.h"
#include "Constants.h"
#include "DebugLogger.h"
#include "TuningProcessor.h"
#include "Oscilator.h"
#include "OscMixer.h"
#include "EnvelopeProcessor.h"
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
	EnvelopeNoteState envelopeState;
	FilterNoteState filterState;
	double phase[NUM_OSCILATORS];

	void Begin(EnvelopeProcessor &envProc, double eventTime)
	{
		pedalState = KEY_PRESSED;
		envProc.BeginNote(envelopeState, eventTime);
		// TODO: come up with a better method for resetting phase
		// the ideal system would ensure everything is always in phase
		// with everything else
		if (!isPlaying)
		{
			for (int i = 0; i < NUM_OSCILATORS; i++)
			{
				phase[i] = 0;
			}
		}
		isPlaying = true;
	}

	void Release(EnvelopeProcessor &envProc, double eventTime)
	{
		pedalState = INACTIVE;
		envProc.ReleaseNote(envelopeState, eventTime);
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
	EnvelopeProcessor envProcessor;
	FilterProcessor filterProcessor;

	std::vector<SynthModule*> modules;
	
	void ProcessMidiMessages(MidiBuffer &);
	void SynthesizeAudio();
};
