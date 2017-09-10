#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParamControl.h"

static const int buttonGap = 0;
static const int buttonSize = 20;

class ParamControlChoice :
	public ParamControl,
	Button::Listener
{
public:
	ParamControlChoice(AudioParameterChoice&);
	~ParamControlChoice();

	virtual int CalculateHeight() override;

	void buttonClicked(Button *button) override;

private:
	AudioParameterChoice &parameter;

	std::vector<ToggleButton*> buttons;
	std::vector<Label*> labels;

	void resized() override;
};
