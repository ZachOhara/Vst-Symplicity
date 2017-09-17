#include "ParamControlChoice.h"

ParamControlChoice::ParamControlChoice(AudioParameterChoice &param) :
	parameter(param)
{
	StringArray options = param.choices;
	for (int i = 0; i < options.size(); i++)
	{
		ToggleButton &button = *(new ToggleButton());
		button.addListener(this);
		button.setRadioGroupId(1, dontSendNotification);
		buttons.push_back(&button);
		addAndMakeVisible(button);

		Label &label = *(new Label());
		label.setText(options[i], dontSendNotification);
		labels.push_back(&label);
		addAndMakeVisible(label);
	}

	int currentIndex = param.getIndex();
	buttons[currentIndex]->triggerClick();
}

ParamControlChoice::~ParamControlChoice()
{
}

int ParamControlChoice::CalculateHeight() {
	return (buttonGap * (buttons.size() - 1))
		+ (buttonSize * buttons.size());
}

void ParamControlChoice::buttonClicked(Button *button)
{
	for (int i = 0; i < buttons.size(); i++)
	{
		if (button == buttons[i])
		{
			parameter = i; // this is what it says in the docs, idk
		}
	}
}

void ParamControlChoice::resized()
{
	int yPos = 0; // incremented during the loop
	const int labelWidth = getWidth() - buttonSize;
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->setTopLeftPosition(0, yPos);
		buttons[i]->setSize(buttonSize, buttonSize);
		labels[i]->setTopLeftPosition(buttonSize, yPos);
		labels[i]->setSize(labelWidth, buttonSize);
	}
}
