#include "ParamControlFloat.h"

ParamControlFloat::ParamControlFloat(AudioParameterFloat &param) :
	parameter(param)
{
	Slider &newslider = *(new Slider());

	newslider.setRange(parameter.range.getRange().getStart(),
		parameter.range.getRange().getEnd(), 0.01);
	newslider.setValue((float)parameter, dontSendNotification);

	SetSlider(newslider);
}

ParamControlFloat::~ParamControlFloat()
{
}

void ParamControlFloat::RecieveNewValue(float newvalue)
{
	parameter = (float)newvalue;
}
