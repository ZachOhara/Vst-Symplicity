#pragma once

#include <string>

#include "../JuceLibraryCode/JuceHeader.h"
#include "Constants.h"
#include "SynthModule.h"

static const double MASTER_LIMIT = 0.05;

class OscMixer :
	public SynthModule
{
public:
	OscMixer();
	~OscMixer();

	double MixValues(double*);

private:
	std::vector<AudioParameterFloat*> oscLevels;

	double totalLevel;
	float lastOscLevels[NUM_OSCILATORS];
	double adjustedOscLevels[NUM_OSCILATORS];

	bool IsUpdateNecessary();
	void DoUpdate();

	double GetAdjustedOscLevel(int);
	float GetOscLevel(int);
};

