#include "DebugLogger.h"

DebugLogger::DebugLogger()
{
}

DebugLogger::~DebugLogger()
{
}

void DebugLogger::OpenDebugLogFile()
{
	stream.open(filepath);
}

void DebugLogger::CloseDebugLogFile()
{
	stream.close();
}
