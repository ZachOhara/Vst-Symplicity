#include "SymplicityEditor.h"

SymplicityEditor::SymplicityEditor(AudioProcessor &parent, std::vector<ModuleParameterSet*> &parameters) :
	AudioProcessorEditor(&parent),
	modules(parameters)
{
	setSize(1150, 475);

	ControlGroup &tuning = *(new ControlGroup(*FindModule("Tuning")));
	tuning.setTopLeftPosition(25, 25);
	tuning.setSize(200, 150);
	tuning.AddParameterControl("Temperament", 10, 5, 180, 75, true);
	tuning.AddParameterControl("Key", 10, 80, 180, 50, true);
	addAndMakeVisible(tuning);

	ControlGroup &mixer = *(new ControlGroup(*FindModule("Mix")));
	mixer.setTopLeftPosition(475, 25);
	mixer.setSize(200, 425);
	addAndMakeVisible(mixer);

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

		String mixname = "Osc " + std::to_string(i + 1);
		mixer.AddParameterControl(mixname, 10, 50 + (i * 150), 180, 40, true);
	}

	ControlGroup &filter = *(new ControlGroup(*FindModule("Filter")));
	filter.setTopLeftPosition(700, 25);
	filter.setSize(200, 425);
	filter.AddParameterControl("Cutoff", 10, 25, 180, 50, true);
	filter.AddParameterControl("Resonance", 10, 75, 180, 50, true);
	addAndMakeVisible(filter);

	ControlGroup &envelope = *(new ControlGroup(*FindModule("Envelope")));
	envelope.setTopLeftPosition(925, 25);
	envelope.setSize(200, 425);
	envelope.AddParameterControl("Attack", 10, 25, 180, 50, true);
	envelope.AddParameterControl("Decay", 10, 75, 180, 50, true);
	envelope.AddParameterControl("Sustain", 10, 125, 180, 50, true);
	envelope.AddParameterControl("Release", 10, 175, 180, 50, true);
	addAndMakeVisible(envelope);
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
