#pragma once

#include <string>

#include "../JuceLibraryCode/JuceHeader.h"
#include "DebugLogger.h"
#include "SynthModule.h"

static const char* octaveOptions[6] = {
	"32' (-2)",
	"16' (-1)",
	" 8' (+0)",
	" 4' (+1)",
	" 2' (+2)",
	NULL,
};

static const int defaultOctave = 2;

static const char* waveformOptions[5] = {
	"Sine",
	"Triangle",
	"Sawtooth",
	"Square",
	NULL,
};

enum Waveform {
	SINE,
	TRIANGLE,
	SAWTOOTH,
	SQUARE,
};

class Oscilator :
	public SynthModule
{
public:
	Oscilator(String);
	~Oscilator();

	double GetSample(double &, double);
	
private:
	AudioParameterChoice &octaveParam;
	AudioParameterChoice &waveformParam;

	double GetUnalteredSample(double);

	int GetOctave();
	Waveform GetWaveform();
};

