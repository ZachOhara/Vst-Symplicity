#include "SynthModule.h"

SynthModule::SynthModule(String name, String abbrev) :
	paramSet(ModuleParameterSet{ std::vector<ModuleParameter*>(), name }),
	fullName(name),
	abbreviation(abbrev)
{
}

SynthModule::~SynthModule()
{
}

void SynthModule::SetSampleRate(double newSampleRate)
{
	sampleRate = newSampleRate;
	secondsPerSample = 1 / sampleRate;
}

ModuleParameterSet & SynthModule::GetParameterSet()
{
	return paramSet;
}

double SynthModule::GetSampleRate()
{
	return sampleRate;
}

double SynthModule::GetSecondsPerSample()
{
	return secondsPerSample;
}

AudioParameterChoice & SynthModule::ConstructParameterChoice(
	String name,
	const char **options,
	int initial
)
{
	AudioParameterChoice &newParam = *(new AudioParameterChoice(
		BuildParameterId(name),
		BuildParameterName(name),
		StringArray(options),
		initial
	));
	RegisterParameter(newParam, PARAM_CHOICE, name);
	return newParam;
}

AudioParameterInt & SynthModule::ConstructParameterInt(
	String name,
	int minValue,
	int maxValue,
	int initialValue
)
{
	AudioParameterInt &newParam = *(new AudioParameterInt (
		BuildParameterId(name),
		BuildParameterName(name),
		minValue,
		maxValue,
		initialValue
	));
	RegisterParameter(newParam, PARAM_INT, name);
	return newParam;
}

void SynthModule::RegisterParameter(
	AudioProcessorParameterWithID &newParam,
	ParameterType type,
	String name)
{
	ModuleParameter *data = new ModuleParameter{ newParam, type, name };
	paramSet.params.push_back(data);
}

String SynthModule::BuildParameterId(String name)
{
	return GetAbbreviation() + "_" + name;
}

String SynthModule::BuildParameterName(String name)
{
	return GetFullName() + " " + name;
}

String SynthModule::GetFullName()
{
	return fullName;
}

String SynthModule::GetAbbreviation()
{
	return abbreviation;
}
