#pragma once

#include "SynthModule.h"
#include "EnvelopeProcessor.h"

#define MAX_FILTER_CUTOFF 20000
#define MAX_FILTER_ORDER 15

struct FilterLayerState
{
	double x0, x1, x2, y1, y2;
};

struct FilterNoteSetting
{
	// calculated for the current settings
	double b0, b1, b2, a1, a2;

	double lastCutoff = 0;
};

struct FilterNoteState
{
	FilterLayerState orderStates[MAX_FILTER_ORDER];
	FilterNoteSetting setting;
};

class FilterProcessor :
	public SynthModule
{
public:
	FilterProcessor();
	~FilterProcessor();

	double GetNextOutput(FilterNoteState &, double);

	void RecalculateValues(FilterNoteSetting &);

protected:
	virtual void HandleParameterChange(AudioProcessorParameter *) override;

private:
	EnvelopeProcessor &contour;

	AudioParameterFloat &cutoffParam;
	AudioParameterInt &orderParam;

	double GetOrderOutput(FilterNoteSetting &, FilterLayerState &, double);
	bool GetEnabled();
	float GetCutoff();
	int GetOrder();
};

