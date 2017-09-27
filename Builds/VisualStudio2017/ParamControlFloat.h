#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParamControlNumber.h"

class ParamControlFloat :
	public ParamControlNumber
{
public:
	ParamControlFloat(AudioParameterFloat &);
	~ParamControlFloat();

protected:
	virtual void RecieveNewValue(float) override;

private:
	AudioParameterFloat &parameter;
};

