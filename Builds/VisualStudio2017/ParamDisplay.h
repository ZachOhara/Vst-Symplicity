#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthModule.h"
#include "ParamControl.h"
#include "ParamControlChoice.h"

static const int sideBuffer = 10;
static const int titleHeight = 20;

class ParamDisplay :
	public Component
{
public:
	ParamDisplay(ModuleParameter&, bool);
	~ParamDisplay();

private:
	Label *title;
	ParamControl *controller;

	virtual void resized() override;
};

