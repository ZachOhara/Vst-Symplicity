#include "Oscilator.h"

Oscilator::Oscilator(String oscID) :
	SynthModule("Oscilator " + oscID, "Osc" + oscID),
	octaveParam(ConstructParameterChoice("Octave", octaveOptions, defaultOctave)),
	waveformParam(ConstructParameterChoice("Waveform", waveformOptions, 0))
{
}

Oscilator::~Oscilator()
{
}

double Oscilator::GetSample(double &phase, double frequency)
{
	if (octave != GetOctave())
	{
		octave = GetOctave();
		octaveFactor = std::pow(2, octave);
	}
	frequency *= octaveFactor;
	phase += frequency * GetSecondsPerSample();
	phase = phase - ((int)phase);
	return GetUnalteredSample(phase);
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
	return octaveParam.getIndex() - defaultOctave;
}

Waveform Oscilator::GetWaveform() {
	return (Waveform)waveformParam.getIndex();
}
