#include "FilterProcessor.h"

FilterProcessor::FilterProcessor() :
	SynthModule("Filter", "Ftr"),
	cutoffParam(ConstructParameterFloat("Cutoff", 0.0, MAX_FILTER_CUTOFF, MAX_FILTER_CUTOFF, 0.4)),
	orderParam(ConstructParameterInt("Resonance", 1, MAX_FILTER_ORDER, 1))
{
}

FilterProcessor::~FilterProcessor()
{
}

double FilterProcessor::GetNextOutput(double currentInput)
{
	double output = currentInput;
	double deadOutput;
	// the dead output is used to keep all the layers current
	// thus removing the artifacts when changing resonance
	for (int i = 0; i < GetOrder(); i++)
	{
		if (i < GetOrder())
		{
			output = GetOrderOutput(orderStates[i], output);
			deadOutput = output;
		}
		else
		{
			deadOutput = GetOrderOutput(orderStates[i], deadOutput);
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

double FilterProcessor::GetOrderOutput(FilterState &state, double currentInput)
{
	// first, calculate the output
	state.x0 = currentInput;
	double y0 = (b0 * state.x0) + (b1 * state.x1) + (b2 * state.x2) - (a1 * state.y1) - (a2 * state.y2);
	// next, shift the samples
	state.x2 = state.x1;
	state.x1 = state.x0;
	state.y2 = state.y1;
	state.y1 = y0;
	// finally, return
	return y0;
}

void FilterProcessor::ClearSamples()
{
	for (int i = 0; i < MAX_FILTER_ORDER; i++)
	{
		orderStates[i].x0 = 0;
		orderStates[i].x1 = 0;
		orderStates[i].x2 = 0;
		orderStates[i].y1 = 0;
		orderStates[i].y2 = 0;
	}
}

void FilterProcessor::RecalculateValues()
{
	double cutoff = GetCutoff();
	double sampleRate = GetSampleRate();
	if (cutoff != 0)
	{
		double c = 1 / std::tan(double_Pi * (cutoff / sampleRate));
		if (cutoff * 2 > sampleRate)
		{
			c = 1 / std::tan(double_Pi / 2);
			// same formula, but cutoff = rate / 2;
		}
		double c_squared = c * c;
		double d = c_squared + (c * std::pow(2, 0.5)) + 1;
		b0 = 1 / d;
		b1 = 2 / d;
		b2 = 1 / d;
		a1 = 2 * (1 - c_squared) / d;
		a2 = (c_squared - (c * std::pow(2, 0.5)) + 1) / d;
	}
}

void FilterProcessor::HandleParameterChange(AudioProcessorParameter *param)
{
	if (param == &cutoffParam)
	{
		RecalculateValues();
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
