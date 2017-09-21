#include "TuningProcessor.h"

TuningProcessor::TuningProcessor() :
	SynthModule("Tuning", "Tuning"),
	modeParam(ConstructParameterChoice("Temperament", tuningOptions, 0)),
	keyParam(ConstructParameterInt("Key", 0, KEYS - 1, 3))
{
	InitializeStandardMap();
	for (int i = 0; i < KEYS; i++) {
		InitializeJustifiedMap(i);
	}
}

TuningProcessor::~TuningProcessor()
{
}

double TuningProcessor::GetFrequency(int noteID) {
	switch (GetTuningMode()) {
	case EQUAL_TEMPERAMENT:
		return standardEqualMap[noteID];
	case JUST_TEMPERAMENT:
		return justifiedKeyMaps[GetKey()][noteID];
	default:
		return 0; // error
	}
}

TuningMode TuningProcessor::GetTuningMode()
{
	return (TuningMode)(int)modeParam;
}

int TuningProcessor::GetKey()
{
	return 3; // TODO get from parameter
}

void TuningProcessor::InitializeStandardMap()
{
	standardEqualMap[MIDDLE_A] = MIDDLE_A_TUNING; // set A4 = 440hz

	for (int i = MIDDLE_A - 1; i >= 0; i--) {
		standardEqualMap[i] = standardEqualMap[57] / std::pow(2, (57 - i) / 12.0);
	}
	for (int i = MIDDLE_A + 1; i <= 127; i++) {
		standardEqualMap[i] = standardEqualMap[57] * std::pow(2, (i - 57) / 12.0);
	}
}

void TuningProcessor::InitializeJustifiedMap(int key)
{
	const int keyCenter = MIDDLE_A + key;
	// base the tonic off of the equal temperament tuning
	justifiedKeyMaps[key][keyCenter] = standardEqualMap[keyCenter];

	for (int i = 0; i < 11; i++) {
		justifiedKeyMaps[key][keyCenter + i + 1] = justifiedKeyMaps[key][keyCenter]
			* JUSTIFIED_RATIOS[i][0] / JUSTIFIED_RATIOS[i][1];
	}

	int octaveOffset = 0;
	for (int i = keyCenter - 1; i >= 0; i--) {
		while (i + (12 * octaveOffset) < keyCenter) {
			octaveOffset += 1;
		}
		justifiedKeyMaps[key][i] = justifiedKeyMaps[key][i + (12 * octaveOffset)] / std::pow(2, octaveOffset);
	}
	octaveOffset = 0;
	for (int i = keyCenter + 12; i <= 127; i++) {
		while (i - (12 * octaveOffset) > keyCenter + 11) {
			octaveOffset += 1;
		}
		justifiedKeyMaps[key][i] = justifiedKeyMaps[key][i - (12 * octaveOffset)] * std::pow(2, octaveOffset);
	}
}
