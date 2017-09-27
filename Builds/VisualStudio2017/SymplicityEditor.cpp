#include "SymplicityEditor.h"

SymplicityEditor::SymplicityEditor(AudioProcessor &parent, std::vector<ModuleParameterSet*> &parameters) :
	AudioProcessorEditor(&parent),
	modules(parameters)
{
	setSize(1150, 475);

	for (int i = 0; i < NUM_OSCILATORS; i++)
	{
		String name = "Oscilator ";
		name += (i + 1);

		ControlGroup &oscilator = *(new ControlGroup(*FindModule(name)));
		oscilator.setTopLeftPosition(250, 25 + (i * 150));
		oscilator.setSize(200, 125);

		oscilator.AddParameterControl("Octave", 5, 0, 85, 125, false);
		oscilator.AddParameterControl("Waveform", 90, 0, 110, 125, false);

		addAndMakeVisible(oscilator);
	}

	ControlGroup &tuning = *(new ControlGroup(*FindModule("Tuning")));
	tuning.setTopLeftPosition(25, 25);
	tuning.setSize(200, 150);
	tuning.AddParameterControl("Temperament", 10, 5, 180, 75, true);
	tuning.AddParameterControl("Key", 10, 80, 180, 50, true);
	addAndMakeVisible(tuning);

	ControlGroup &filter = *(new ControlGroup(*FindModule("Filter")));
	filter.setTopLeftPosition(500, 25);
	filter.setSize(200, 450);
	filter.AddParameterControl("Cutoff", 10, 25, 180, 50, true);
	filter.AddParameterControl("Resonance", 10, 75, 180, 50, true);
	addAndMakeVisible(filter);
}

SymplicityEditor::~SymplicityEditor()
{
}

ModuleParameterSet * SymplicityEditor::FindModule(String name)
{
	for (int i = 0; i < modules.size(); i++)
	{
		if (modules[i]->moduleName == name)
		{
			return modules[i];
		}
	}
	// Module does not exist
	std::cerr << "ERR cannot find module '" << name << "'\n";
	std::cerr.flush();
	return NULL;
}

void SymplicityEditor::paint(Graphics &g)
{
	g.fillAll(Colours::white);
}
