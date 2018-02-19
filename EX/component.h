#pragma once

/*
** Base class component object.
** All entity functionality is expected to derive from this object.
*/

#include <map>

#include "frame.h"
#include "raylib.h"
#include "raymath.h"

using std::map;

class Entity;

class Component
{
	public:
		Component() = delete;
		//use this constructor
		Component(Entity* e);
		//virtual ~Component();
		virtual ~Component();
		
		//one to update any simulation and draw the result
		virtual void Update(FrameParams* frame) { return; }
		virtual void Draw(FrameParams* frame) { return; }
		
		const Entity* getEntity() const { return _entity; }
		Entity* getEntity() { return _entity; }
		
	protected:
		Entity* _entity;
};


//===================================================================================
/*
	need a rotate component
	Rotate(axis, speed);
	Move(Vector3* path);
	Move(playerInput);
	onCollisionDestroy();
*/

//class for handling meshes
//literally just a map
class MeshManager
{
	public:
		static map<string, Model> Models;
};

class MeshComponent : public Component
{
	public:
		MeshComponent() = delete;
		MeshComponent(Entity* e, Model* m, float scale, Color col = BLUE);
		MeshComponent(Entity* e, string m, float scale, Color col = BLUE);
		~MeshComponent();
		
		//draw mesh with model's rotation, etx...
		void Update(struct FrameParams* frame);
		void Draw(FrameParams* frame);
		BoundingBox GetBoundingBox();
	
		bool draw;
		
	protected:
		Model* _model;
		string _m;
		float _s;
		Color _c;
};

//press button to do a thing
class Button : public Component
{
	public:
		Button() = delete;
		//add a callback function to this?
		Button(Entity* e, Texture2D button, Color tint, Color HoverTint, float scale, void (*fun) (int), int arg );
		~Button();
		
		void Draw(FrameParams* frame);
		void Update(FrameParams* frame);
		bool Clicked(Vector2 mousePos);
		void Exec();
	
	private:
		Texture2D _b;
		Vector2 _pos;
		Rectangle hitbox;
		//function pointer
		void (*f) (int);
		int _arg;
		Color _c, _c2;
		float _scale;
};

//dialogue advancing
class ClickAdvance : public Component
{
	public:
		ClickAdvance() = delete;
		ClickAdvance(Entity* e, void (*foo) ());
		
		void Update(FrameParams* frame);
	
	private:
		void (*f) ();
		int _arg;
	
};

class SceneAdvance : public Component
{
	public:
		SceneAdvance() = delete;
		SceneAdvance(Entity* e, void (*foo) (int), int arg);
		
		void Update(FrameParams* frame);
		void Exec();
	
	private:
		void (*f) (int);
		int _arg;
};


//goes to precursor where the first click jumps the dialogue to the end and changes the callback
//function to AdvanceDialogue ready for the 2nd click
class DialogueAdvance : public Component
{
	public:
		DialogueAdvance() = delete;
		DialogueAdvance(Entity* e, void (*foo) (DialogueAdvance*));
		void Exec();
		void setCallback(void (*foo) (DialogueAdvance*)) { f = foo; }
		void Update(FrameParams* frame);
		
	private:
		void (*f) (DialogueAdvance*);
	
};

//bark component, similar to killswitch but less bleh

class BARK : public Component
{
	public:
		BARK() = delete;
		BARK(Entity* e, float d, int id, void (*foo) (int));
		void Update(FrameParams* frame);
		void Reset(int id);
	private:
		float duration, counter, ID;
		void (*f) (int);
	
};

//player controls
//also handles the camera movements as well
class ControllerMovement : public Component
{
	public:
		ControllerMovement() = delete;
		ControllerMovement(Entity* e);
		
		//update's the entity's position
		void Update(FrameParams* frame);
	
	private:
	
	
};

//constant rotateion
class Rotate : public Component
{
	public:
		Rotate() = delete;
		Rotate(Entity* e, Vector3 axis, float speed);
		
		//spin around at a constant rate
		void Update(FrameParams* frame);
	
	private:
		Vector3 _axis;
		float _rot, dr;
};

//oscillate between two positions
class Oscillate : public Component
{
	public:
		Oscillate() = delete;
		Oscillate(Entity* e, Vector3 A, Vector3 B, double frequency);
		
		void Update(FrameParams* frame);
	
	private:
		Vector3 _A, _B;
		double _f, _current;
		bool _back;
};

class Shot : public Component
{
	public:
		Shot() = delete;
		Shot(Entity* e, MeshComponent* m, void (*foo) (Entity*));
		
		void Update(FrameParams* frame);
		
	private:
		MeshComponent* _m;
		void (*f) (Entity*);
};

//self-destruct after X seconds
class KillSwitch : public Component
{
	public:
		KillSwitch() = delete;
		KillSwitch(Entity* e, float d);
		void Update(FrameParams* frame);
	private:
		float duration, counter;
	
};

class HoverGlow : public Component
{
	public:
		HoverGlow() = delete;
		HoverGlow(Entity* e, MeshComponent* m);
		void Update(FrameParams* frame);
		void Draw(FrameParams* frame);
		
	private:
		bool highlight;
		BoundingBox b;
		MeshComponent* _m; 
};

//orbit on the horizontal plane
class Orbit : public Component
{
	public:
		Orbit() = delete;
		Orbit(Entity* e, Vector3 point, float radius, float speed);
		void Update(FrameParams* frame);
		
	private:
		Vector3 _p;
		float _r, _s;
	
};

class SphereTrigger : public Component
{
	public:
		SphereTrigger() = delete;
		SphereTrigger(Entity* e, Entity* target, float radius, void (*foo) (int), int arg);
		void Update(FrameParams* frame);
	
	
	private:
		Entity* _t;
		float r;
		void (*f) (int);
		int _arg;
	
};

class BoolCheck : public Component
{
	public:
		BoolCheck() = delete;
		BoolCheck(Entity* e, bool& b, bool ref,void (*foo) (int), int arg);
		void Update(FrameParams* frame);
		
	private:
		bool r;
		bool t;
		void (*f) (int);
		int _arg;
	
};

//increment the number of kills or do some callback function
class KillTrack : public Component
{
	public:
		KillTrack() = delete;
		KillTrack(Entity* e, void (*foo) (int), int arg, MeshComponent* m);
		void Update(FrameParams* frame);
	
	private:
		void (*f) (int);
		int _arg;
		MeshComponent* _m;
};
