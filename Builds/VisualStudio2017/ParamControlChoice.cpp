#include "ParamControlChoice.h"

ParamControlChoice::ParamControlChoice(AudioParameterChoice &param) :
	parameter(param)
{
	StringArray options = param.choices;
	for (int i = 0; i < options.size(); i++)
	{
		ToggleButton &button = *(new ToggleButton());
		button.setRadioGroupId(1, dontSendNotification);
		button.setColour(button.tickColourId, Colours::black);
		buttons.push_back(&button);
		addAndMakeVisible(button);

		Label &label = *(new Label());
		label.setText(options[i], dontSendNotification);
		label.setColour(label.textColourId, Colours::black);
		labels.push_back(&label);
		addAndMakeVisible(label);
	}

	int currentIndex = param.getIndex();
	buttons[currentIndex]->triggerClick();

	addMouseListener(this, true);
}

ParamControlChoice::~ParamControlChoice()
{
}

int ParamControlChoice::CalculateHeight() {
	return (buttonGap * ((int)buttons.size() - 1))
		+ (buttonSize * (int)buttons.size());
}

void ParamControlChoice::mouseDown(const MouseEvent &event)
{
	// Warning: this is probably not going to get extensively tested
	// Errors are likely
	// If there is undefined behavior later, this is a prime suspect
	if (event.eventComponent == this)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			int buttonY = buttons[i]->getY();
			if (buttonY <= event.y && event.y < buttonY + buttonSize)
			{
				ChangeSelection(i);
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			if (event.eventComponent == buttons[i] || event.eventComponent == labels[i])
			{
				ChangeSelection(i);
			}
		}
	}
}

void ParamControlChoice::ChangeSelection(int index)
{
	parameter = index;
	buttons[index]->triggerClick();
}

void ParamControlChoice::resized()
{
	int yPos = 0; // incremented during the loop
	const int labelWidth = getWidth() - buttonSize;
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->setTopLeftPosition(0, yPos);
		buttons[i]->setSize(buttonSize + labelSpace, buttonSize);
		labels[i]->setTopLeftPosition(buttonSize + labelSpace, yPos);
		labels[i]->setSize(labelWidth, buttonSize);
		yPos += buttonSize;
	}
}
