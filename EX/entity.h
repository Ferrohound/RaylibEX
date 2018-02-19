#pragma once
 
#include <vector>
#include <string>
//#include "component.h"
#include "raylib.h"
#include "raymath.h"
#include "GDUtilities.hpp"
#include "component.h"

using std::string;


class Entity
{
	public:
		Entity() = delete;
		Entity(Vector3 p);
		virtual ~Entity();
		
		virtual void Update(FrameParams* f);
		virtual void Draw(FrameParams* f);
		
		void addComponent(Component* c);
		void clearComponents() { _components.clear(); }
		
		
		//const Matrix& get_transform() const { return _transform; }
		//void set_transform(const Matrix& t) { _transform = t; }
		void setPosition(Vector3 p);
		
		void Rotate(Vector3 axis, float deg);
		void Translate(Vector3 dir);
		void setID(int newID);
		
		Matrix GetTransform();
		Vector3 GetPosition();
		
		//GET THIS OUT ASAP
		Component* GetComponent(int i)
		{
			return _components[i];
		}
	
	protected:
		std::vector<Component*> _components;
		Matrix _transform;
		Vector3 _position;
		int ID;
		//should probably have rotation in here too
};

//Reticule class, always drawn last
class Reticule: public Entity
{
	public:
		Reticule() = delete;
		Reticule(Vector3 p, bool l, Color c, int rot);
		~Reticule();
		
		void Update(FrameParams* f);
		
		void Draw(FrameParams* f);
		Texture2D* GetSprite()
		{
			return &_sprite;
		}		
		int GetX()
		{
			return _pos.x;
		}
		
		Vector2 GetPos()
		{
			return _pos;
		}
	private:
		Color _c;
		bool _left;
		Vector2 _pos;
		float _maxSpeed;
		Texture2D _sprite;
		int rotation;
};

//projectiles shot from the player or towards the player or an object
/*
 * Projectiles have dimensions, speed, damage, position
 * constructor: dimensions, speed, damage, start position, direction, lifespan
 */ 
 //cap the number of projectiles onscreen to like 10 or 20
class Projectile: public Entity
{
	public:
		Projectile() = delete;
		Projectile(Vector3 dimensions, float speed, float damage, 
			Vector3 position, Vector3 direction, Color col): 
			Entity(position),
			_dmg(damage), _spd(speed),  
			_dim(dimensions), _dir(direction), _c(col)
		{ }
		
		//move the projectile in the direction
		void Update(FrameParams* f);
		void Draw();
	
	protected:
		Vector3 _dir, _dim;
		float _spd, _dmg;
		Color _c;
};
