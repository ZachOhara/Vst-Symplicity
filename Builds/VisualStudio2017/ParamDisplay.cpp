#include "ParamDisplay.h"

ParamDisplay::ParamDisplay(ModuleParameter &parameter, bool showTitle) :
	showTitle(showTitle)
{
	if (showTitle)
	{
		title.setText(parameter.paramName, dontSendNotification);
		title.setColour(Label::textColourId, Colours::black);
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
		controller = new ParamControlInt(static_cast<AudioParameterInt&>(parameter.parameter));
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
	topBuffer += outlineCorrection;
	int availableWidth = getWidth();
	controller->setSize(availableWidth, controller->CalculateHeight());
	if (showTitle)
	{
		topBuffer -= (titleHeight / 2);
		title.setTopLeftPosition(0 , topBuffer);
		title.setSize(availableWidth, titleHeight);
		controller->setTopLeftPosition(0, topBuffer + titleHeight);
	}
	else
	{
		controller->setTopLeftPosition(0, topBuffer);
	}
}

void ParamDisplay::paint(Graphics & g)
{
	// For debugging: outline the title and the control
	/*

	g.setColour(Colours::red);
	g.drawRect(title.getX(), title.getY(),
		title.getWidth(), title.getHeight(), 5);
	g.drawRect(controller->getX(),
		controller->getY(),
		controller->getWidth(),
		controller->getHeight(),
		5);
	
	*/
}
