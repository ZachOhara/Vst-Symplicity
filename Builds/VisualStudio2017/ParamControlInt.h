#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParamControlNumber.h"

class ParamControlInt :
	public ParamControlNumber
{
public:
	ParamControlInt(AudioParameterInt &);
	~ParamControlInt();

protected:
	virtual void RecieveNewValue(float) override;

private:
	AudioParameterInt &parameter;
};
