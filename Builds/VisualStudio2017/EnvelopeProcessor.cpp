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
	if (state.phase == ATTACK)
	{
		ProgressToDecay(state);
		// if the attack is still happening, ditch it and start decaying
		// this also sets up the pre-release phase, which skips from decay into release
	}
	state.phase = PRE_RELEASE;
	state.noteTime = -eventTime;
	// One issue with this system is the potential to jump to the release if the
	// key is released before the note finishes the decay phase. An old solution
	// to this problem involved complicated code to modify the timer, but this
	// didn't work very well. Now, the sustain phase is just blocked until the gain
	// gets down to sustain-level
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

	// if the gain hasn't changed this sample, don't rescale it (scaling is expensive)

	if (state.currGain != state.lastGain)
	{
		state.lastGain = state.currGain;
		state.scaledGain = GetScaledLevel(state.currGain);
	}

	return state.scaledGain;
}

double EnvelopeProcessor::GetScaledLevel(double level)
{
	return std::pow(level, EULER_NUM);
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
		if (state.noteTime > 0 && state.currGain <= GetSustainLevel())
			ProgressToRelease(state);
		break;
	case RELEASE:
		if (state.noteTime > GetReleaseTime() && state.currGain <= 0)
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

		if (VERBOSE_PHASE_TRANSITIONS)
		{
			std::cout << "Attack, " << state.currGain << "+" << state.gainDelta
				<< "  t:" << state.noteTime << "/" << GetAttackTime() << "\n";
			std::cout.flush();
		}
	}
	else
	{
		if (VERBOSE_PHASE_TRANSITIONS)
		{
			std::cout << "Attack skipped\n";
			std::cout.flush();
		}
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
		state.gainDelta = (GetSustainLevel() - state.currGain) / (GetDecayTime() / GetSecondsPerSample());
		
		if (VERBOSE_PHASE_TRANSITIONS)
		{
			std::cout << "Decay, " << state.currGain << "+" << state.gainDelta
				<< "  t:" << state.noteTime << "/" << GetAttackTime() + GetDecayTime() << "\n";
			std::cout.flush();
		}
	}
	else
	{
		if (VERBOSE_PHASE_TRANSITIONS)
		{
			std::cout << "Decay skipped\n";
			std::cout.flush();
		}
		ProgressToSustain(state);
	}
}

void EnvelopeProcessor::ProgressToSustain(EnvelopeNoteState &state)
{
	state.phase = SUSTAIN;
	state.currGain = GetSustainLevel();
	state.gainDelta = 0;

	if (VERBOSE_PHASE_TRANSITIONS)
	{
		std::cout << "Sustain, " << state.currGain << "+" << state.gainDelta
			<< "  t:" << state.noteTime << "\n";
		std::cout.flush();
	}
}

void EnvelopeProcessor::ProgressToRelease(EnvelopeNoteState &state)
{
	// Unlike the other timed phase, the release should never be skipped because
	// the timer at this point is unreliable. If the key is released before the note
	// finishes the decay phase, the end of the decay phase will be added to the timer
	// and any skipping mechanism has the ability to mis-fire and clip the end of the
	// note.

	state.phase = RELEASE;
	state.gainDelta = (0 - state.currGain) / (GetReleaseTime() / GetSecondsPerSample());

	if (VERBOSE_PHASE_TRANSITIONS)
	{
		std::cout << "Release, " << state.currGain << "+" << state.gainDelta
			<< "  t:" << state.noteTime << "/" << GetReleaseTime() << "\n";
		std::cout.flush();
	}
}

void EnvelopeProcessor::ProgressToSilence(EnvelopeNoteState &state)
{
	state.phase = SILENCE;
	state.currGain = 0;
	state.gainDelta = 0;

	if (VERBOSE_PHASE_TRANSITIONS)
	{
		std::cout << "Silence, " << state.currGain << "+" << state.gainDelta
			<< "  t:" << state.noteTime << "\n";
		std::cout.flush();
	}
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
