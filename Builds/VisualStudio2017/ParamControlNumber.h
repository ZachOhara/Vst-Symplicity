#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParamControl.h"

static const int sliderHeight = 40;

class ParamControlNumber :
	public ParamControl,
	Slider::Listener
{
public:
	ParamControlNumber();
	~ParamControlNumber();

	virtual void sliderValueChanged(Slider *) override;

	virtual int CalculateHeight() override;

protected:
	void SetSlider(Slider &);

	virtual void RecieveNewValue(float) = 0;

private:
	Slider * slider;

	virtual void resized() override;
};
