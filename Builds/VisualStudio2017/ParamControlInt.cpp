#include "ParamControlInt.h"

ParamControlInt::ParamControlInt(AudioParameterInt &param) :
	parameter(param)
{
	Slider &slider = *(new Slider());
	
	slider.setMinValue(parameter.getRange().getStart());
	slider.setMaxValue(parameter.getRange().getEnd());
	slider.setRange(parameter.getRange().getStart(),
	parameter.getRange().getEnd(), 1);
	slider.setValue(parameter, dontSendNotification);
	
	SetSlider(slider);
}

ParamControlInt::~ParamControlInt()
{
}

void ParamControlInt::RecieveNewValue(float newvalue)
{
	parameter = (int)newvalue;
}
