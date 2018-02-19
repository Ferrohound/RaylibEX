/*
	Sprite class
	Billboard

*/
#include <vector>

#include "raylib.h"
#include "entity.h"

using std::vector;

class Sprite: public Entity
{
	public:
		Sprite(Texture2D tex, Vector3 pos);
		//void Update(FrameParams* frame);
		//billboard it!
		void Draw(FrameParams* frame);
		
	
	private:
		Texture2D _tex;
		//Rectangle _rect;
		float _scale;
		Color _tint;
};
