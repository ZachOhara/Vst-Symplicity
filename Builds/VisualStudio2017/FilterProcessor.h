#pragma once

#include "SynthModule.h"

#define MAX_FILTER_CUTOFF 20000
#define MAX_FILTER_ORDER 30

struct FilterState
{
	double x0, x1, x2, y1, y2;
};

class FilterProcessor :
	public SynthModule
{
public:
	FilterProcessor();
	~FilterProcessor();

	double GetNextOutput(double);
	void ClearSamples();

	void RecalculateValues();

protected:
	virtual void HandleParameterChange(AudioProcessorParameter *) override;

private:
	//AudioParameterBool &enabledParam;
	AudioParameterFloat &cutoffParam;
	AudioParameterInt &orderParam;
	FilterState orderStates[MAX_FILTER_ORDER];

	// calculated for the current settings
	double b0, b1, b2, a1, a2;

	double GetOrderOutput(FilterState &, double);
	bool GetEnabled();
	float GetCutoff();
	int GetOrder();
};

