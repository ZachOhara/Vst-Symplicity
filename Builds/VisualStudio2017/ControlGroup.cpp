#include "ControlGroup.h"

ControlGroup::ControlGroup(ModuleParameterSet &params) :
	parameterSet(params),
	outline(parameterSet.moduleName, parameterSet.moduleName)
{
}

ControlGroup::~ControlGroup()
{
}

void ControlGroup::AddParameterControl(
	String name,
	int x, int y, int width, int height,
	bool displayTitle
)
{
	ModuleParameter parameter = *FindParameter(name);
	if (&parameter == NULL)
	{
		// Reporting and handling should be done from within FindParameter()
		return;
	}
	ParamDisplay &control = *(new ParamDisplay(parameter, displayTitle));
	control.setTopLeftPosition(x, y);
	control.setSize(width, height);
	addAndMakeVisible(control);
}

ModuleParameter * ControlGroup::FindParameter(String name)
{
	for (int i = 0; i < parameterSet.params.size(); i++)
	{
		if (parameterSet.params[i]->paramName == name)
		{
			return parameterSet.params[i];
		}
	}
	// Parameter does not exist
	std::cerr << "ERR cannot find param '" << name
		<< "' in module '" << parameterSet.moduleName << "'\n";
	std::cerr.flush();
	return NULL;
}

void ControlGroup::paint(Graphics &g)
{
	g.fillAll(Colours::red);
}
