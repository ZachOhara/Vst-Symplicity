#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthModule.h"

static const int KEYS = 12;
static const int NOTES = 128;
static const int MIDDLE_A = 57;
static const double MIDDLE_A_TUNING = 440.0;

static const int JUSTIFIED_RATIOS[11][2] = {
	{ 16, 15 },{ 9, 8 },{ 6, 5 },{ 5, 4 },{ 4, 3 },
	{ 45, 32 },{ 3, 2 },{ 8, 5 },{ 5, 3 },{ 9, 5 },{ 15, 8 }
};

static const char *tuningOptions[3] = {
	"Equal Temperament",
	"Just Temperament",
	NULL,
};

enum TuningMode {
	EQUAL_TEMPERAMENT,
	JUST_TEMPERAMENT
};

class TuningProcessor :
	public SynthModule
{
public:
	TuningProcessor();
	~TuningProcessor();

	double GetFrequency(int);

	ModuleParameterSet GetParameters() override;

private:
	AudioParameterChoice &modeParam;
	AudioParameterInt &keyParam;

	double standardEqualMap[NOTES];
	double justifiedKeyMaps[KEYS][NOTES];

	TuningMode GetTuningMode();
	int GetKey();

	void InitializeStandardMap();
	void InitializeJustifiedMap(int);
};

