#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class ParamControl :
	public Component
{
public:
	ParamControl();
	~ParamControl();

	virtual int CalculateWidth();
	virtual int CalculateHeight();
};

