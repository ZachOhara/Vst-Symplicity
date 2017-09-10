#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

enum ParameterType {
	PARAM_BOOL,
	PARAM_CHOICE,
	PARAM_INT,
	PARAM_FLOAT,
};

struct ModuleParameter {
	AudioProcessorParameter *parameter;
	ParameterType type;
	String displayName;
};

struct ModuleParameterSet {
	std::vector<ModuleParameter> &parameters;
	String &sectionName;
};

class SynthModule
{
public:
	SynthModule(std::string, std::string);
	~SynthModule();

	void SetSampleRate(double);

	virtual ModuleParameterSet GetParameters() = 0;

protected:
	String& GetLongName();
	String& GetShortName();

	double GetSampleRate();
	double GetSecondsPerSample();

	//virtual AudioParameterBool ConstructParameterBool() = 0;
	//virtual AudioParameterFloat ConstructParameterFloat() = 0;
	AudioParameterChoice &ConstructParameterChoice(
		const char*, const char*, const char**, int);
	AudioParameterInt &ConstructParameterInt(
		const char*, const char*, int, int, int);

private:
	String *longName;
	String *shortName;

	double sampleRate;
	double secondsPerSample;
};
