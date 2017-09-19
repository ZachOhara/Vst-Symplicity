#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParamControl.h"

static const int buttonGap = 0; // gap between each row
static const int buttonSize = 20; // 1d size of the button
static const int labelSpace = 5; // gap between button and label

class ParamControlChoice :
	public ParamControl
{
public:
	ParamControlChoice(AudioParameterChoice &);
	~ParamControlChoice();

	virtual int CalculateHeight() override;

	virtual void mouseDown(const MouseEvent &) override;

private:
	AudioParameterChoice &parameter;

	std::vector<ToggleButton*> buttons;
	std::vector<Label*> labels;

	void ChangeSelection(int);

	virtual void resized() override;
};
