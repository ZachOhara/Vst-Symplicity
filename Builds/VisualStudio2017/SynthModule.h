#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

enum ParameterType {
	PARAM_BOOL,
	PARAM_CHOICE,
	PARAM_INT,
	PARAM_FLOAT,
};

struct ModuleParameter {
	AudioProcessorParameterWithID &parameter;
	ParameterType type;
	String paramName;
};

struct ModuleParameterSet {
	std::vector<ModuleParameter*> params;
	String moduleName;
};

class SynthModule
{
public:
	SynthModule(String, String);
	~SynthModule();

	void SetSampleRate(double);

	ModuleParameterSet & GetParameterSet();

protected:
	double GetSampleRate();
	double GetSecondsPerSample();

	//virtual AudioParameterBool ConstructParameterBool() = 0;
	//virtual AudioParameterFloat ConstructParameterFloat() = 0;
	
	AudioParameterChoice & ConstructParameterChoice(
		String, const char**, int);
	AudioParameterInt & ConstructParameterInt(
		String, int, int, int);

private:
	ModuleParameterSet paramSet;

	String fullName;
	String abbreviation;

	double sampleRate;
	double secondsPerSample;

	String GetFullName();
	String GetAbbreviation();

	void RegisterParameter(AudioProcessorParameterWithID &, ParameterType, String);
	String BuildParameterId(String);
	String BuildParameterName(String);
};
