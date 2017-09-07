#include "SynthModule.h"

SynthModule::SynthModule(std::string newLongName, std::string newShortName)
{
	longName = new String(newLongName);
	shortName = new String(newShortName);
}

SynthModule::~SynthModule()
{
}

void SynthModule::SetSampleRate(double newSampleRate)
{
	sampleRate = newSampleRate;
	secondsPerSample = 1 / sampleRate;
}

/*
void SynthModule::SetLongName(String &newLongName)
{
	longName = &newLongName;
}

void SynthModule::SetShortName(String &newShortName)
{
	shortName = &newShortName;
}
*/

String & SynthModule::GetLongName()
{
	return *longName;
}

String & SynthModule::GetShortName()
{
	return *shortName;
}

double SynthModule::GetSampleRate()
{
	return sampleRate;
}

double SynthModule::GetSecondsPerSample()
{
	return secondsPerSample;
}

AudioParameterChoice &SynthModule::ConstructParameterChoice(
	const char *suffixLong,
	const char *suffixShort,
	const char **options,
	int initial
)
{
	return AudioParameterChoice(
		GetShortName() + suffixShort,
		GetLongName() + suffixLong,
		StringArray(options),
		initial
	);
}

AudioParameterInt & SynthModule::ConstructParameterInt(
	const char *suffixLong,
	const char *suffixShort,
	int minValue,
	int maxValue,
	int initialValue
)
{
	return AudioParameterInt(
		GetShortName() + suffixShort,
		GetLongName() + suffixLong,
		minValue,
		maxValue,
		initialValue
	);
}
