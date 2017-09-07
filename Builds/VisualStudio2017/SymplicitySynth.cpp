#include "SymplicitySynth.h"

SymplicitySynth::SymplicitySynth() :
	//osc1(1)
	oscilators{ Oscilator(1), Oscilator(2), Oscilator(3) }
{
}

SymplicitySynth::~SymplicitySynth()
{
}

void SymplicitySynth::PrepareToPlay(double newSampleRate, int newBlockSize)
{
	secondsPerSample = 1 / newSampleRate;

	blockSize = newBlockSize;
	sampleBuffer = (double*)malloc(sizeof(double) * blockSize);

	//osc1.SetSampleRate(newSampleRate);
	for (int i = 0; i < NUM_OSCILATORS; i++)
	{
		oscilators[i].SetSampleRate(newSampleRate);
	}
	/*
	oscilators = (Oscilator*)malloc(sizeof(Oscilator) * NUM_OSCILATORS);
	for (int i = 0; i < NUM_OSCILATORS; i++) {
	String name(std::to_string(i + 1));
	Oscilator osc(name);
	oscilators[i] = osc;
	}
	*/
	debugLog.OpenDebugLogFile();
	debugLog.stream << "Starting debug log\n";
}

void SymplicitySynth::ReleaseResources()
{
	free(sampleBuffer);
	//free(oscilators);
	debugLog.CloseDebugLogFile();
}

void SymplicitySynth::ProcessBlock(AudioSampleBuffer &audioBuffer, MidiBuffer &midiBuffer)
{
	// Maybe TODO tick the clock? is this necessary
	ProcessMidiMessages(midiBuffer);
	SynthesizeAudio();

	float *bufferLeft = audioBuffer.getWritePointer(0);
	float *bufferRight = audioBuffer.getWritePointer(1);

	for (int i = 0; i < blockSize; i++) {
		bufferLeft[i] = (float)sampleBuffer[i];
		bufferRight[i] = (float)sampleBuffer[i];
		sampleBuffer[i] = 0; // free this for the next block
	}
}

void SymplicitySynth::ProcessMidiMessages(MidiBuffer &midiBuffer)
{
	MidiMessage message;
	int frame;
	for (MidiBuffer::Iterator iterator(midiBuffer); iterator.getNextEvent(message, frame);)
	{
		int noteID = message.getNoteNumber();
		double eventTime = frame * secondsPerSample;
		switch (GetMessageType(message)) {
		case NOTE_ON:
			// TODO if monophonic mode, release other notes
			debugLog.stream << "Note on: " << noteID << "\n";
			keyboard[noteID].Begin(eventTime);
			break;
		case NOTE_OFF:
			debugLog.stream << "Note off: " << noteID << "\n";
			keyboard[noteID].Release(eventTime);
			break;
		case PEDAL_ON:
			isPedalOn = true;
			break;
		case PEDAL_OFF:
			isPedalOn = false;
			// TODO release all notes that are currently PEDALED_ONLY
		}
	}
}

void SymplicitySynth::SynthesizeAudio()
{
	for (int frame = 0; frame < blockSize; frame++)
	{
		for (int noteID = 0; noteID < NUM_NOTES; noteID++)
		{
			NoteStatus &note = keyboard[noteID];
			if (note.isPlaying)
			{
				double frequency = tuningProcessor.GetFrequency(noteID);

				double oscValues[NUM_OSCILATORS];
				for (int i = 0; i < NUM_OSCILATORS; i++) {
					oscValues[i] = oscilators[i].GetSample(&note.phase[i], frequency);
				}

				// TODO mix the values
				double sample = oscValues[0] + oscValues[1] + oscValues[2];
				sample /= 3;

				// TODO apply envelope

				sampleBuffer[frame] += sample;
			}
		}
		// TODO add noise
		// TODO apply filter
		// TODO apply master volume
		sampleBuffer[frame] *= 0.25; // stop clipping

		// TODO tick the progressive pitch bend
	}
}

static MidiMessageType GetMessageType(MidiMessage &message) {
	if (message.isNoteOn())
		return NOTE_ON;
	else if (message.isNoteOff())
		return NOTE_OFF;
	else if (message.isSustainPedalOn())
		return PEDAL_ON;
	else if (message.isSustainPedalOff())
		return PEDAL_OFF;
	else
		return INVALID;
}