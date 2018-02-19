#include "entity.h"

Entity::Entity(Vector3 p) : _position(p) 
{
	_transform = MatrixIdentity();
	ID = -1;
};

Entity::~Entity()
{
	for(int i=0; i<_components.size(); i++)
	{
		delete(_components[i]);
	}
}

void Entity::Update(FrameParams* f)
{
	for(int i = 0; i < _components.size(); i++)
	{
		_components[i]->Update(f);
	}
}

void Entity::Draw(FrameParams* frame)
{
	for(int i = 0; i < _components.size(); i++)
	{
		_components[i]->Draw(frame);
	}
}

void Entity::addComponent(Component* c)
{
	_components.push_back(c);
}

void Entity::setPosition(Vector3 p)
{
	_position = p;
}

Vector3 Entity::GetPosition()
{
	return _position;
}

Matrix Entity::GetTransform()
{
	return _transform;
}

void Entity::Translate(Vector3 dir)
{
	//might not even need this
	/*_transform = MatrixMultiply(_transform, 
		MatrixTranslate(float dir.x, float dir.y, float dir.z));*/
		
	_position.x += dir.x;
	_position.y += dir.y;
	_position.z += dir.z;
}


void Entity::setID(int newID)
{
	ID = newID;
}

void Entity::Rotate(Vector3 axis, float deg)
{
	_transform = MatrixMultiply( _transform, MatrixRotate(axis, DEG2RAD * deg));
	
}



//========================================================================

Reticule::Reticule(Vector3 p, bool l, Color c, int rot): Entity(p)
	{
		_left = l;
		_c = c;
		rotation = rot;
		_maxSpeed = 3.5; 
		_pos = Vector2 { p.x, p.y }; 
		_sprite = LoadTexture("res/2D/Reticule.png");
	}

Reticule::~Reticule()
{
	UnloadTexture(_sprite);
}

void Reticule::Draw(FrameParams* f)
{
	//for now just a circle
	//DrawCircleV(_pos, 40, _c);
	DrawTextureEx(_sprite, Vector2{_pos.x - (_sprite.width/2), _pos.y - (_sprite.height/2) -15}, rotation, 1, Fade(_c, 0.5));
	DrawCircleV(_pos, 10, BLACK);
}
		
void Reticule::Update(FrameParams* f)
{
	//rotation++;
	
	float dx, dy;
	//update the reticule's position
	if(_left)
	{
		dx = f->input->dx1;
		dy = f->input->dy1;
	}
	else
	{
		dx = f->input->dx2;
		dy = f->input->dy2;
	}
			
	//smooth dy and dx
	//dx = GDUtility::smoothInput(dx, _maxSpeed);
	//dy = GDUtility::smoothInput(dy, _maxSpeed);
			
	//add dy and dx
	_pos.x += (dx);
	_pos.y += (dy);		
}

//==========================================================================

void Projectile::Update(FrameParams* f)
{
	//guess i need to make a vector libraryYYYyyYY
	//_position += (_dir*_spd);
}

void Projectile::Draw()
{
	DrawCube(_position, _dim.x, _dim.y, _dim.z, _c);
}
