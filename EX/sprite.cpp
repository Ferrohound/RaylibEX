#include "sprite.h"

/*void Sprite::Update(FrameParams* frame)
{
	
}*/

Sprite::Sprite(Texture2D tex, Vector3 pos) : Entity(pos), _tex(tex)
{}

void Sprite::Draw(FrameParams* frame)
{
	DrawBillboard(*frame->cam, _tex, _position, _scale, _tint);
}
