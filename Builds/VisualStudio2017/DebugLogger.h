#pragma once

#include <iostream>
#include <fstream>

static const char* filepath = "C:\\Users\\zjoha\\Documents\\Coding\\Vst-Symplicity\\runtime.log";

static class DebugLogger
{
public:
	DebugLogger();
	~DebugLogger();

	void OpenDebugLogFile();
	void CloseDebugLogFile();

	std::ofstream stream;
} debugLog;
