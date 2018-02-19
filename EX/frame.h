#pragma once

#include <chrono>
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#include "raylib.h"

using std::string;
using std::cout;

//maybe have framaParams and that holds input, player position, time,
//etc...

struct Input
{
	float dx1;
	float dx2;
	float dy1;
	float dy2;
	float pedal;
	
	bool fire1;
	bool fire2;
	//store the previous fire state
	bool prevFire1;
	bool prevFire2;
	float cooldown1, cooldown2;
	
	bool spec1;
	bool spec2;
	//store previous button state
	bool prevSpec1;
	bool prevSpec2;

	
};

//camera, input, player position...
struct FrameParams
{
	Input* input;
	Camera* cam;
	Vector3 playerPosition;
	Vector2 R1Pos;
	Vector2 R2Pos;
	Vector3 PlayerPos;
	
	//previous time for time.delta time? Debatable, but sure
	std::chrono::high_resolution_clock::time_point lastTime;
	double deltaTime;
	bool dialogueRunning;
};


