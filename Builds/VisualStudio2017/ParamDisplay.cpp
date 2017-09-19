#include "ParamDisplay.h"

ParamDisplay::ParamDisplay(ModuleParameter &parameter, bool showTitle) :
	showTitle(showTitle)
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
	int topBuffer = getHeight() - controller->CalculateHeight();
	topBuffer /= 2;
	int availableWidth = getWidth();

	// Titles are not yet supported
	/*
	if (showTitle)
	{
		topBuffer -= (titleHeight / 2);
		title.setTopLeftPosition(sideBuffer, topBuffer);
		title.setSize(availableWidth, titleHeight);
		controller->setTopLeftPosition(sideBuffer, topBuffer + titleHeight);
		controller->setSize(availableWidth, controller->CalculateHeight());
	}
	*/
	controller->setTopLeftPosition(0, topBuffer + outlineCorrection);
	controller->setSize(availableWidth, controller->CalculateHeight());
}
