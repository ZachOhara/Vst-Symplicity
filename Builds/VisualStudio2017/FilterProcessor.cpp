#include "FilterProcessor.h"

FilterProcessor::FilterProcessor() :
	SynthModule("Filter", "Ftr"),
	cutoffParam(ConstructParameterFloat("Cutoff", 0.0, MAX_FILTER_CUTOFF, MAX_FILTER_CUTOFF, 0.4)),
	orderParam(ConstructParameterInt("Resonance", 1, MAX_FILTER_ORDER, 1)),
	contour(*(new EnvelopeProcessor("Contour", "Con")))
{
}

FilterProcessor::~FilterProcessor()
{
}

double FilterProcessor::GetNextOutput(FilterNoteState &state, double currentInput)
{
	RecalculateValues(state.setting);
	double output = currentInput;
	double deadOutput;
	// the dead output is used to keep all the layers updated
	// thus removing the artifacts when changing resonance
	for (int i = 0; i < GetOrder(); i++)
	{
		if (i < GetOrder())
		{
			output = GetOrderOutput(state.setting, state.orderStates[i], output);
			deadOutput = output;
		}
		else
		{
			deadOutput = GetOrderOutput(state.setting, state.orderStates[i], deadOutput);
		}
	}
	if (GetEnabled())
	{
		if (GetCutoff() == 0)
		{
			return 0;
		}
		else
		{
			return output;
		}
	}
	else
	{
		return currentInput;
	}
}

double FilterProcessor::GetOrderOutput(FilterNoteSetting &setting, FilterLayerState &state, double currentInput)
{
	// first, calculate the output
	state.x0 = currentInput;
	double y0 = (setting.b0 * state.x0) + (setting.b1 * state.x1) + (setting.b2 * state.x2)
		- (setting.a1 * state.y1) - (setting.a2 * state.y2);
	// next, shift the samples
	state.x2 = state.x1;
	state.x1 = state.x0;
	state.y2 = state.y1;
	state.y1 = y0;
	// finally, return
	return y0;
}

void FilterProcessor::RecalculateValues(FilterNoteSetting &setting)
{
	double cutoff = GetCutoff();
	double sampleRate = GetSampleRate();
	if (setting.lastCutoff != cutoff && cutoff != 0)
	{
		double c = 1 / std::tan(double_Pi * (cutoff / sampleRate));
		if (cutoff * 2 > sampleRate)
		{
			c = 1 / std::tan(double_Pi / 2);
			// same formula, but cutoff is capped at the Nyquist frequency;
		}
		double c_squared = c * c;
		double d = c_squared + (c * ROOT_TWO) + 1;
		setting.b0 = 1 / d;
		setting.b1 = 2 / d;
		setting.b2 = 1 / d;
		setting.a1 = 2 * (1 - c_squared) / d;
		setting.a2 = (c_squared - (c * ROOT_TWO) + 1) / d;
		setting.lastCutoff = cutoff;
	}
}

void FilterProcessor::HandleParameterChange(AudioProcessorParameter *param)
{
	if (param == &cutoffParam)
	{
		//RecalculateValues();
	}
}

bool FilterProcessor::GetEnabled()
{
	return true;
}

float FilterProcessor::GetCutoff()
{
	return (float)cutoffParam;
}

int FilterProcessor::GetOrder()
{
	return (int)orderParam;
}
