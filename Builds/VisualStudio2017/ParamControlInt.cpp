#include "ParamControlInt.h"

ParamControlInt::ParamControlInt(AudioParameterInt &param) :
	parameter(param)
{
	Slider &newslider = *(new Slider());
	
	newslider.setRange(parameter.getRange().getStart(),
		parameter.getRange().getEnd(), 1);
	newslider.setValue(parameter, dontSendNotification);
	
	SetSlider(newslider);
}

ParamControlInt::~ParamControlInt()
{
}

void ParamControlInt::RecieveNewValue(float newvalue)
{
	parameter = (int)newvalue;
}
