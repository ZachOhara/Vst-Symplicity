#include "SymplicitySynth.h"

SymplicitySynth::SymplicitySynth(AudioProcessor &parent)
{
	modules.push_back(&tuningProcessor);
	modules.push_back(&oscMixer);
	modules.push_back(&envProcessor);
	modules.push_back(&filterProcessor);
	for (int i = 0; i < NUM_OSCILATORS; i++)
	{
		oscilators.push_back(new Oscilator(String(std::to_string(i + 1))));
		modules.push_back(oscilators[i]);
	}
	
	// Now that all modules have been created, register the parameters
	for (int i = 0; i < modules.size(); i++)
	{
		parent.addListener(modules[i]);
		std::vector<ModuleParameter *> parameters = modules[i]->GetParameterSet().params;
		for (int j = 0; j < parameters.size(); j++)
		{
			parent.addParameter(&parameters[j]->parameter);
		}
	}
}

SymplicitySynth::~SymplicitySynth()
{
}

void SymplicitySynth::PrepareToPlay(double newSampleRate, int newBlockSize)
{
	// Initialize this state
	secondsPerSample = 1 / newSampleRate;
	blockSize = newBlockSize;
	sampleBuffer = (double*)malloc(sizeof(double) * blockSize);

	// Send the sample rate to the modules
	for (int i = 0; i < modules.size(); i++)
	{
		modules[i]->SetSampleRate(newSampleRate);
	}

	debugLog.OpenDebugLogFile();
	//debugLog.stream << "Starting debug log\n";
}

void SymplicitySynth::ReleaseResources()
{
	free(sampleBuffer);
	debugLog.CloseDebugLogFile();
}

AudioProcessorEditor & SymplicitySynth::ConstructEditor(AudioProcessor &processor)
{
	std::vector<ModuleParameterSet*> parameters;
	for (int i = 0; i < modules.size(); i++)
	{
		parameters.push_back(&(modules[i]->GetParameterSet()));
	}
	return *(new SymplicityEditor(processor, parameters));
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
			keyboard[noteID].Begin(envProcessor, eventTime);
			break;
		case NOTE_OFF:
			if (!isPedalOn)
			{
				keyboard[noteID].Release(envProcessor, eventTime);
			}
			else
			{
				keyboard[noteID].pedalState = PEDALED_ONLY;
			}
			break;
		case PEDAL_ON:
			isPedalOn = true;
			break;
		case PEDAL_OFF:
			isPedalOn = false;
			for (int i = 0; i < NUM_NOTES; i++)
			{
				if (keyboard[i].pedalState == PEDALED_ONLY)
				{
					keyboard[i].Release(envProcessor, eventTime);
				}
			}
			break;
		}
	}
}

void SymplicitySynth::SynthesizeAudio()
{
	long totalOscDuration = 0;
	long totalEnvDuration = 0;
	long totalFilDuration = 0;

	for (int frame = 0; frame < blockSize; frame++)
	{
		for (int noteID = 0; noteID < NUM_NOTES; noteID++)
		{
			NoteStatus &note = keyboard[noteID];
			if (note.isPlaying)
			{
				auto startTime = std::chrono::steady_clock::now();

				// Calculate frequency and simulate oscilators
				double frequency = tuningProcessor.GetFrequency(noteID);

				double oscValues[NUM_OSCILATORS];
				for (int i = 0; i < NUM_OSCILATORS; i++) {
					oscValues[i] = oscilators[i]->GetSample(note.phase[i], frequency);
				}

				// Mix the oscilators
				double sample = oscMixer.MixValues(oscValues);

				auto oscTime = std::chrono::steady_clock::now();
				std::chrono::nanoseconds oscDuration = oscTime - startTime;

				// Apply the envelope
				sample *= envProcessor.GetVolume(note.envelopeState);
				if (envProcessor.IsFinishedReleasing(note.envelopeState))
				{
					note.isPlaying = false;
				}

				auto envTime = std::chrono::steady_clock::now();
				std::chrono::nanoseconds envDuration = envTime - oscTime;

				// Apply the filter to the note
				sample = filterProcessor.GetNextOutput(note.filterState, sample);

				auto filTime = std::chrono::steady_clock::now();
				std::chrono::nanoseconds filDuration = filTime - envTime;

				sampleBuffer[frame] += sample;

				totalOscDuration += oscDuration.count();
				totalEnvDuration += envDuration.count();
				totalFilDuration += filDuration.count();
			}
		}
		// TODO add noise
		
		// TODO apply master volume

		// TODO tick the progressive pitch bend
	}

	// Processing time information. Uncomment this code to see how much cpu
	// time the diffeent components of the synth are taking.
	/*
	std::cout << (totalOscDuration / blockSize) << "  "
		<< (totalEnvDuration / blockSize) << "  "
		<< (totalFilDuration / blockSize) << "\n";
	std::cout.flush();
	*/
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
