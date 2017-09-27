#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define DEFAULT_SKEW 1.0

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
	double skewFactor;
};

struct ModuleParameterSet {
	std::vector<ModuleParameter*> params;
	String moduleName;
};

class SynthModule :
	public AudioProcessorListener
{
public:
	SynthModule(String, String);
	~SynthModule();

	void SetSampleRate(double);

	ModuleParameterSet & GetParameterSet();
	
	virtual void audioProcessorChanged(AudioProcessor *) override;
	virtual void audioProcessorParameterChanged(AudioProcessor *, int, float) override;

protected:
	double GetSampleRate();
	double GetSecondsPerSample();

	virtual void HandleParameterChange(AudioProcessorParameter *);

	//virtual AudioParameterBool ConstructParameterBool() = 0;
	
	AudioParameterChoice & ConstructParameterChoice(
		String, const char**, int);
	AudioParameterInt & ConstructParameterInt(
		String, int, int, int, double=DEFAULT_SKEW);
	AudioParameterFloat & ConstructParameterFloat(
		String, float, float, float, double=DEFAULT_SKEW);

private:
	ModuleParameterSet paramSet;

	String fullName;
	String abbreviation;

	double sampleRate;
	double secondsPerSample;

	String GetFullName();
	String GetAbbreviation();

	void RegisterParameter(
		AudioProcessorParameterWithID &, ParameterType,
		String, double=DEFAULT_SKEW);
	String BuildParameterId(String);
	String BuildParameterName(String);
};
