/*
 * Utilities for 
 * 
 * */
#pragma once

#include "frame.h"
#include <string.h>

namespace GDUtility
{
	float smoothInput(float in, float max);
	void ParseInput(char* msg, Input* I);
	void DefaultInput(Input* I);
}
