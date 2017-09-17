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
	//const char * csName,
	String name,
	const char **options,
	int initial
)
{
	//String &name = *(new String(csName));
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
	//const char * csName,
	String name,
	int minValue,
	int maxValue,
	int initialValue
)
{

	//String &name = *(new String(csName));
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

	std::cout << "--Registering '" << data->paramName << "' (" << data->parameter.name << ")\n";
	std::cout.flush();
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
