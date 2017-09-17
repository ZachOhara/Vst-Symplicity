#include "Oscilator.h"

Oscilator::Oscilator(String &oscID) :
	SynthModule(*(new String("Oscilator ")) + oscID, *(new String("Osc")) + oscID),
	octaveParam(ConstructParameterChoice("Octave", octaveOptions, defaultOctave)),
	waveformParam(ConstructParameterChoice("Waveform", waveformOptions, 0))
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
