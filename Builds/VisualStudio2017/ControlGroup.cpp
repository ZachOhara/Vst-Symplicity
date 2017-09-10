#include "ControlGroup.h"

ControlGroup::ControlGroup(ModuleParameterSet &parameterSet) :
	parameters(parameterSet)//,
	//outline(GroupComponent(parameterSet.sectionName, parameterSet.sectionName))
{
}

ControlGroup::~ControlGroup()
{
}

void ControlGroup::AddParameterControl(
	String &name,
	int x, int y, int width, int height,
	bool displayTitle
)
{
	ModuleParameter &parameter = FindParameter(name);
	ParamDisplay &control = ParamDisplay(parameter, displayTitle);
	control.setTopLeftPosition(x, y);
	control.setSize(width, height);
	addAndMakeVisible(control);
}

ModuleParameter &ControlGroup::FindParameter(String &name)
{
	for (int i = 0; i < parameters.parameters.size(); i++)
	{
		if (parameters.parameters[i].parameter->getName(name.length()) == name)
		{
			return parameters.parameters[i];
		}
	}
}
