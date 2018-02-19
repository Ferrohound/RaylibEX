/*
cpp file for particle
*/

#include "particle.h"

Emitter::Emitter(Entity* e) : Component(e)
{
	_pos = _entity->GetPosition();
}

void Emitter::Update(FrameParams* frame)
{
	//update each particle's position, spawn new ones, remove old ones, etc...
	_pos = _entity->GetPosition();
	
}

//emitter's draw function
void Emitter::Draw(FrameParams* frame)
{
	for(int i = 0; i < _particles.size(); i++)
	{
		//billboard draw the particles
		//Texture2D bill = LoadTexture("resources/billboard.png");     // Our texture billboard
		//Vector3 billPosition = { 0.0f, 2.0f, 0.0f };      
		DrawBillboard(*frame->cam, _particles[i]._tex, _particles[i]._pos, 2.0f, _particles[i]._c);
	}
	
}


void Emitter::SpawnParticle()
{
	
	
}
