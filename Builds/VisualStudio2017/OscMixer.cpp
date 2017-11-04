#include "OscMixer.h"

OscMixer::OscMixer() :
	SynthModule("Mix", "Mix")
{
	for (int i = 1; i <= NUM_OSCILATORS; i++)
	{
		String name = "Osc " + std::to_string(i);
		oscLevels.push_back(&ConstructParameterFloat(name, 0.0, 1.0, 1.0));
	}
}

OscMixer::~OscMixer()
{
}

double OscMixer::MixValues(double *oscValues)
{
	double totalLevel = 0;
	for (int i = 0; i < NUM_OSCILATORS; i++)
	{
		totalLevel += GetAdjustedOscLevel(i);
	}
	double finalValue = 0;
	for (int i = 0; i < NUM_OSCILATORS; i++)
	{
		double ratio = GetAdjustedOscLevel(i) / totalLevel;
		finalValue += ratio * oscValues[i];
	}
	finalValue *= MASTER_LIMIT;
	if (std::abs(finalValue) >= 1)
	{
		std::cerr << "Clipped!!\n";
		std::cerr.flush();
	}
	return finalValue;
}

double OscMixer::GetAdjustedOscLevel(int index)
{
	return std::pow((float)*oscLevels[index], EULER_NUM);
}
