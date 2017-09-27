#include "ParamControlNumber.h"

ParamControlNumber::ParamControlNumber()
{
}

ParamControlNumber::~ParamControlNumber()
{
}

void ParamControlNumber::sliderValueChanged(Slider *changedSlider)
{
	if (changedSlider == slider)
	{
		RecieveNewValue((float)slider->getValue());
	}
}

void ParamControlNumber::SetSlider(Slider &newSlider)
{
	slider = &newSlider;
	slider->setTopLeftPosition(0, 0);
	slider->addListener(this);
	slider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
	addAndMakeVisible(slider);
}

void ParamControlNumber::SetSkewFactor(double skewFactor)
{
	slider->setSkewFactor(skewFactor);
}

int ParamControlNumber::CalculateHeight()
{
	return sliderHeight;
}

void ParamControlNumber::resized()
{
	slider->setSize(getWidth(), sliderHeight);
}
