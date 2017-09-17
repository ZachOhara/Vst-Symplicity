#include "ParamDisplay.h"

ParamDisplay::ParamDisplay(ModuleParameter &parameter, bool showTitle)
{
	if (showTitle)
	{
		title.setText(parameter.paramName, dontSendNotification);
		addAndMakeVisible(title);
	}
	switch (parameter.type)
	{
	case PARAM_BOOL:
		break; // TODO
	case PARAM_CHOICE:
		controller = new ParamControlChoice(static_cast<AudioParameterChoice&>(parameter.parameter));
		break;
	case PARAM_FLOAT:
	case PARAM_INT:
		break;
	}
	addAndMakeVisible(*controller);
}

ParamDisplay::~ParamDisplay()
{
}

void ParamDisplay::resized()
{
	int topBuffer = getHeight() - titleHeight
		- controller->CalculateHeight();
	topBuffer /= 2;
	int availableWidth = getWidth() - (sideBuffer * 2);
	title.setTopLeftPosition(sideBuffer, topBuffer);
	title.setSize(availableWidth, titleHeight);
	controller->setTopLeftPosition(sideBuffer, topBuffer + titleHeight);
	controller->setSize(availableWidth, controller->CalculateHeight());
}
