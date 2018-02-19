/*
	Raylib particle system


*/
#include <vector>

#include "frame.h"
#include "raylib.h"
#include "entity.h"

using std::vector;

#define MAXPARTICLES 20;
//particles have a velocity
//rectangles for positions and whatnot
struct Particle
{
	Vector3 _v;
	Vector3 _pos;
	Color _c;
	float _life;
	Texture2D _tex;
	
	//edit this later
	Particle() 
      : /*_pos(0.0f), _v(0.0f), _c(1.0f),*/ _life(0.0f) { }
};


//Emitter class, inherits from entity for update function and draw function
class Emitter : public Component
{
	public:
		Emitter(Entity* e);
		
		//draw all the particles
		void Draw(FrameParams* frame);
		void Update(FrameParams* frame);
		void SpawnParticle();
	
	
	private:
		vector<Particle> _particles;
		int _numParticles;
		Vector3 _pos;
};
