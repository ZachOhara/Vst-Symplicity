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
	if (IsUpdateNecessary())
	{
		DoUpdate();
	}

	double finalValue = 0;
	for (int i = 0; i < NUM_OSCILATORS; i++)
	{
		double ratio = adjustedOscLevels[i] / totalLevel;
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

bool OscMixer::IsUpdateNecessary()
{
	for (int i = 0; i < NUM_OSCILATORS; i++)
	{
		if (lastOscLevels[i] != (float)*oscLevels[i])
		{
			return true;
		}
	}
	return false;
}

void OscMixer::DoUpdate()
{
	totalLevel = 0;
	for (int i = 0; i < NUM_OSCILATORS; i++)
	{
		if (lastOscLevels[i] != GetOscLevel(i))
		{
			lastOscLevels[i] = GetOscLevel(i);
			adjustedOscLevels[i] = GetAdjustedOscLevel(i);
		}
		totalLevel += adjustedOscLevels[i];
	}
}

double OscMixer::GetAdjustedOscLevel(int index)
{
	return std::pow(GetOscLevel(index), EULER_NUM);
}

float OscMixer::GetOscLevel(int index)
{
	return (float)*oscLevels[index];
}
