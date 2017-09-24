#include "ParamControlNumber.h"

ParamControlNumber::ParamControlNumber()
{
}

ParamControlNumber::~ParamControlNumber()
{
}

void ParamControlNumber::sliderValueChanged(Slider *changedSlider)
{
	RecieveNewValue(slider->getValue());
}

void ParamControlNumber::SetSlider(Slider &newSlider)
{
	slider = &newSlider;
	slider->setTopLeftPosition(0, 0);
	slider->addListener(this);
	slider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
	addAndMakeVisible(slider);
}

int ParamControlNumber::CalculateHeight()
{
	return sliderHeight;// *1.5;
}

void ParamControlNumber::resized()
{
	slider->setSize(getWidth(), sliderHeight);
}
