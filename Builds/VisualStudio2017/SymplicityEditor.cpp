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

		oscilator.AddParameterControl("Octave", 0, 0, 200, 125, false);

		//debugLog.stream << name << "\n";
		//debugLog.stream << (oscilator->FindParameter("Octave")) << "\n";

		addAndMakeVisible(oscilator);
	}
}

SymplicityEditor::~SymplicityEditor()
{
}

ModuleParameterSet * SymplicityEditor::FindModule(String &name)
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
	g.fillAll(Colours::darkcyan);
}
