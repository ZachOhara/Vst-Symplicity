#include "SymplicityEditor.h"

SymplicityEditor::SymplicityEditor(AudioProcessor &parent, std::vector<ModuleParameterSet*> parameters) :
	AudioProcessorEditor(&parent),
	modules(parameters)
{
	setSize(1150, 475);
	for (int i = 0; i < NUM_OSCILATORS; i++)
	{
		std::string name = "Oscilator " + std::to_string(i);
		ControlGroup oscilator(FindModule(String(name)));
		addAndMakeVisible(oscilator);
		oscilator.setTopLeftPosition(250, i * 150);
		oscilator.setSize(200, 125);
	}
}

SymplicityEditor::~SymplicityEditor()
{
}

ModuleParameterSet & SymplicityEditor::FindModule(String &name)
{
	for (int i = 0; i < modules.size(); i++)
	{
		if (modules[i]->sectionName == name)
		{
			return *(modules[i]);
		}
	}
}

void SymplicityEditor::paint(Graphics &g)
{
	g.fillAll(Colours::white);
}
