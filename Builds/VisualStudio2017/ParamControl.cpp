#include "ParamControl.h"

ParamControl::ParamControl()
{
}

ParamControl::~ParamControl()
{
}

void ParamControl::SetSkewFactor(double skewFactor)
{
	// overridable
}

int ParamControl::CalculateWidth()
{
	return 0;
}

int ParamControl::CalculateHeight()
{
	return 0;
}
