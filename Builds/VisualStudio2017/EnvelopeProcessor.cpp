#include "EnvelopeProcessor.h"

EnvelopeProcessor::EnvelopeProcessor() :
	EnvelopeProcessor("Envelope", "Env")
{
}

EnvelopeProcessor::EnvelopeProcessor(String longName, String shortName) :
	SynthModule(longName, shortName),
	attackParam(ConstructParameterFloat("Attack", ENV_MIN_TIME, ENV_MAX_TIME, ENV_MIN_TIME, ENV_TIME_SKEW)),
	decayParam(ConstructParameterFloat("Decay", ENV_MIN_TIME, ENV_MAX_TIME, ENV_MIN_TIME, ENV_TIME_SKEW)),
	sustainParam(ConstructParameterFloat("Sustain", 0.0, 1.0, 1.0)),
	releaseParam(ConstructParameterFloat("Release", ENV_MIN_TIME, ENV_MAX_TIME, ENV_MIN_TIME, ENV_TIME_SKEW))
{
}

EnvelopeProcessor::~EnvelopeProcessor()
{
}

void EnvelopeProcessor::BeginNote(EnvelopeNoteState &state, double eventTime)
{
	state.phase = PRE_ATTACK;
	state.noteTime = -eventTime;
}

void EnvelopeProcessor::ReleaseNote(EnvelopeNoteState &state, double eventTime)
{
	state.phase = PRE_RELEASE;
	// If the note is released before the decay phase has finished, then the
	// release must be put off until the decay is done. The following code
	// accounts for this.
	if (state.noteTime + eventTime < GetAttackTime() + GetDecayTime())
	{
		state.noteTime -= GetAttackTime() + GetDecayTime();
		// this resets the timer to a negative value that will hit
		// zero as soon as the decay completes
	}
	else
	{
		state.noteTime = -eventTime;
	}
}

bool EnvelopeProcessor::IsFinishedReleasing(EnvelopeNoteState &state)
{
	return state.phase == SILENCE;
}

double EnvelopeProcessor::GetVolume(EnvelopeNoteState &state)
{
	ProgressIfReady(state);

	state.currGain += state.gainDelta;
	state.noteTime += GetSecondsPerSample();

	// Correct for edge cases here
	// (probably caused by precision errors in timing)
	if (state.currGain > 1)
	{
		state.currGain = 1;
	}
	if (state.currGain < 0)
	{
		state.currGain = 0;
	}

	return GetScaledLevel(state.currGain);
}

double EnvelopeProcessor::GetScaledLevel(double level)
{
	return std::pow(level, 2.7182818); // Euler's number (e)
}

void EnvelopeProcessor::ProgressIfReady(EnvelopeNoteState &state)
{
	switch (state.phase) {
	case SILENCE:
		break; // progression is triggered by a call to beginNote(), not here
	case PRE_ATTACK:
		if (state.noteTime > 0)
			ProgressToAttack(state);
		break;
	case ATTACK:
		if (state.noteTime > GetAttackTime())
			ProgressToDecay(state);
		break;
	case DECAY:
		if (state.noteTime > GetAttackTime() + GetDecayTime())
			ProgressToSustain(state);
		break;
	case SUSTAIN:
		break; // progression is triggered by a call to releaseNote(), not here
	case PRE_RELEASE:
		if (state.noteTime > 0)
			ProgressToRelease(state);
		break;
	case RELEASE:
		if (state.noteTime > GetReleaseTime())
			ProgressToSilence(state);
		break;
	}
}

void EnvelopeProcessor::ProgressToAttack(EnvelopeNoteState &state)
{
	// Check if the attack time is negligible (less than one sample)
	// If it is, skip to the decay
	if (state.noteTime <= GetAttackTime())
	{
		state.phase = ATTACK;
		state.gainDelta = (1 - state.currGain) / (GetAttackTime() / GetSecondsPerSample());
	}
	else
	{
		state.currGain = 1;
		ProgressToDecay(state);
	}
}

void EnvelopeProcessor::ProgressToDecay(EnvelopeNoteState &state)
{
	// Check if the decay time is negligible (less than one sample)
	// If it is, skip to the sustain
	if (state.noteTime <= GetAttackTime() + GetDecayTime())
	{
		state.phase = DECAY;
		state.currGain = 1;
		state.gainDelta = (GetSustainLevel() - state.currGain) / (GetDecayTime() / GetSecondsPerSample());
	}
	else
	{
		ProgressToSustain(state);
	}
}

void EnvelopeProcessor::ProgressToSustain(EnvelopeNoteState &state)
{
	state.phase = SUSTAIN;
	state.currGain = GetSustainLevel();
	state.gainDelta = 0;
}

void EnvelopeProcessor::ProgressToRelease(EnvelopeNoteState &state)
{
	// Check if the release time is negligible (less than one frame)
	// If it is, skip to silence
	if (state.noteTime <= GetReleaseTime())
	{
		state.phase = RELEASE;
		state.gainDelta = (0 - state.currGain) / (GetReleaseTime() / GetSecondsPerSample());
	}
	else
	{
		ProgressToSilence(state);
	}
}

void EnvelopeProcessor::ProgressToSilence(EnvelopeNoteState &state)
{
	state.phase = SILENCE;
	state.currGain = 0;
	state.gainDelta = 0;
}

float EnvelopeProcessor::GetAttackTime()
{
	return (float)attackParam;
}

float EnvelopeProcessor::GetDecayTime()
{
	return (float)decayParam;
}

float EnvelopeProcessor::GetSustainLevel()
{
	return (float)sustainParam;
}

float EnvelopeProcessor::GetReleaseTime()
{
	return (float)releaseParam;
}
