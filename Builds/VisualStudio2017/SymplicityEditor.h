#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SymplicitySynth.h"
#include "SynthModule.h"
#include "ControlGroup.h"

class SymplicityEditor :
	public AudioProcessorEditor
{
public:
	SymplicityEditor(AudioProcessor &, std::vector<ModuleParameterSet*> &);
	~SymplicityEditor();

private:
	std::vector<ModuleParameterSet*> modules;

	ModuleParameterSet * FindModule(String &name);

	virtual void paint(Graphics&) override;
};
