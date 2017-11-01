#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthModule.h"

static float ENV_MIN_TIME = 0.05f;
static float ENV_MAX_TIME = 2.0f;
static float ENV_TIME_SKEW = 0.5f;

enum EnvelopePhase {
	SILENCE,
	PRE_ATTACK,
	ATTACK,
	DECAY,
	SUSTAIN,
	PRE_RELEASE,
	RELEASE,
};

struct EnvelopeNoteState {
	EnvelopePhase phase = SILENCE;
	double currGain = 0;
	double gainDelta = 0; // per sample
	double noteTime = 0;
};

class EnvelopeProcessor :
	public SynthModule
{
public:
	EnvelopeProcessor();
	EnvelopeProcessor(String, String);
	~EnvelopeProcessor();

	void BeginNote(EnvelopeNoteState &, double);
	void ReleaseNote(EnvelopeNoteState &, double);
	bool IsFinishedReleasing(EnvelopeNoteState &);

	double GetVolume(EnvelopeNoteState &);

private:
	AudioParameterFloat &attackParam;
	AudioParameterFloat &decayParam;
	AudioParameterFloat &sustainParam;
	AudioParameterFloat &releaseParam;

	double GetScaledLevel(double level);
	void ProgressIfReady(EnvelopeNoteState &);
	void ProgressToAttack(EnvelopeNoteState &);
	void ProgressToDecay(EnvelopeNoteState &);
	void ProgressToSustain(EnvelopeNoteState &);
	void ProgressToRelease(EnvelopeNoteState &);
	void ProgressToSilence(EnvelopeNoteState &);

	float GetAttackTime();
	float GetDecayTime();
	float GetSustainLevel();
	float GetReleaseTime();
};
