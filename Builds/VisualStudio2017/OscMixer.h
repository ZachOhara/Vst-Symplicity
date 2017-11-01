#pragma once

#include <string>

#include "../JuceLibraryCode/JuceHeader.h"
#include "Constants.h"
#include "SynthModule.h"

static const double MASTER_LIMIT = 0.2;

class OscMixer :
	public SynthModule
{
public:
	OscMixer();
	~OscMixer();

	double MixValues(double*);

private:
	std::vector<AudioParameterFloat*> oscLevels;

	double GetAdjustedOscLevel(int);
};

