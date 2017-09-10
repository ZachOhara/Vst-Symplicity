#include "Oscilator.h"

Oscilator::Oscilator(int oscID) :
	SynthModule("Oscilator " + std::to_string(oscID), "Osc" + std::to_string(oscID)),
	octaveParam(ConstructParameterChoice(" Octave", "_8ve", octaveOptions, defaultOctave)),
	waveformParam(ConstructParameterChoice(" Waveform", "_Wave", waveformOptions, 0))
{
}

Oscilator::~Oscilator()
{
}

double Oscilator::GetSample(double* phase, double frequency)
{
	frequency *= std::pow(2, GetOctave());
	*phase += frequency * GetSecondsPerSample();
	*phase = *phase - ((int)*phase);
	return GetUnalteredSample(*phase);
}

double Oscilator::GetUnalteredSample(double phase)
{
	switch (GetWaveform()) {
	case SINE:
		return std::sin(double_Pi * 2 * phase);
	case TRIANGLE:
		return (4 * std::abs(phase - 0.5)) - 1;
	case SAWTOOTH:
		return 2 * (phase - 0.5);
	case SQUARE:
		return phase < 0.5 ? 1 : -1;
	default:
		return 0; // error
	}
}

int Oscilator::GetOctave() {
	return 0;
	// return octaveParam->getIndex() - defaultOctave;
}

Waveform Oscilator::GetWaveform() {
	return SINE;
	// return (Waveform)waveformParam->getIndex();
}

ModuleParameterSet Oscilator::GetParameters()
{
	std::vector<ModuleParameter> paramList;
	paramList.push_back({ &octaveParam, PARAM_CHOICE, "Octave" });
	paramList.push_back({ &waveformParam, PARAM_CHOICE, "Waveform" });
	return { paramList, GetLongName() };
}
