#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthModule.h"
#include "ParamDisplay.h"

class ControlGroup :
	public Component
{
public:
	ControlGroup(ModuleParameterSet &);
	~ControlGroup();

	void AddParameterControl(String, int, int, int, int, bool);

private:
	ModuleParameterSet parameterSet;
	GroupComponent outline;

	ModuleParameter * FindParameter(String);

	virtual void resized() override;
	virtual void paint(Graphics&) override;
};

