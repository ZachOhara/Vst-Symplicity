#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthModule.h"
#include "ParamControl.h"
#include "ParamControlChoice.h"
#include "ParamControlInt.h"
#include "ParamControlFloat.h"

static const int outlineCorrection = 4; // determined experimentally to correct for the outline
static const int titleHeight = 20;

class ParamDisplay :
	public Component
{
public:
	ParamDisplay(ModuleParameter &, bool);
	~ParamDisplay();

private:
	bool showTitle;
	Label title;
	ParamControl *controller;

	virtual void resized() override;

	virtual void paint(Graphics &g) override;
};

