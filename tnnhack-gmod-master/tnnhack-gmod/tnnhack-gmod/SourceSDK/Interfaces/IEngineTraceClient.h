#pragma once

#include "../trace.h"

class IEngineTraceClient
{
private:
	virtual void* PAD_000() = 0;
	virtual void* PAD_001() = 0;
	virtual void* PAD_002() = 0;
	virtual void* PAD_003() = 0;
public:
	virtual void TraceLine(const Ray_t& ray, unsigned int mask, ITraceFilter* filter, trace_t* outTrace) = 0;
};