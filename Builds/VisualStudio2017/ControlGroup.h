#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthModule.h"
#include "ParamDisplay.h"

class ControlGroup :
	public Component
{
public:
	ControlGroup(ModuleParameterSet&);
	~ControlGroup();

	void AddParameterControl(String&, int, int, int, int, bool);

private:
	ModuleParameterSet &parameters;
	//GroupComponent &outline;

	ModuleParameter &FindParameter(String &name);
};

