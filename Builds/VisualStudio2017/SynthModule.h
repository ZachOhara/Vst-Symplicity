#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class SynthModule
{
public:
	SynthModule(std::string, std::string);
	~SynthModule();

	void SetSampleRate(double);

	virtual std::vector<AudioProcessorParameter*> GetParameters() = 0;

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

