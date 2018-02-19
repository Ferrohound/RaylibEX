#include "component.h"
#include "entity.h"

//mesh manager's mesh map
map<string, Model> MeshManager::Models;

//====================================================================

Component::Component(Entity* e) : _entity(e)
{
	_entity->addComponent(this);
}

Component::~Component()
{
	
}



//=====================================================================

MeshComponent::MeshComponent(Entity* e, Model* m, float scale,Color col) : Component(e)
{
	_model = m;
	_s = scale;
	_c = col;
}

MeshComponent::MeshComponent(Entity* e, string m, float scale,Color col) : Component(e)
{
	_m= m;
	_model = NULL;
	_s = scale;
	_c = col;
	draw = true;
}


MeshComponent::~MeshComponent()
{
	
}

void MeshComponent::Update(FrameParams* frame)
{
	//_model->transform = _entity->GetTransform();
}


//draw mesh with model's rotation, etx...
void MeshComponent::Draw(FrameParams* frame)
{
	if(draw)
	{
		if(_model!=NULL)
		{
			_model->transform = _entity->GetTransform();
			DrawModel(*_model, _entity->GetPosition(), _s, _c);
		}
		else
		{
			MeshManager::Models[_m].transform = _entity->GetTransform();
			DrawModel(MeshManager::Models[_m], _entity->GetPosition(), _s, _c);
		}
	}
}

BoundingBox MeshComponent::GetBoundingBox()
{
	BoundingBox result;
	
	if(_model!=NULL)
		result = CalculateBoundingBox(_model->mesh);
	else
		result = CalculateBoundingBox(MeshManager::Models[_m].mesh);
	
	Vector3Transform(&result.min, _entity->GetTransform());
	Vector3Transform(&result.max, _entity->GetTransform());
	
	result.min = Vector3Add(result.min, _entity->GetPosition());
	result.max = Vector3Add(result.max, _entity->GetPosition());
	
	return result;
}


//=======================================================================

Button::Button(Entity*e, Texture2D button, Color tint, Color HoverTint, float scale, void (*fun) (int), int arg) : Component(e)
{
	_b = button;
	_c = tint;
	_c2 = HoverTint;
	f = fun;
	_arg = arg;
	_pos.x = e->GetPosition().x;
	_pos.y = e->GetPosition().y;
	_scale = scale;
	
	hitbox.x = _pos.x;
	hitbox.y = _pos.y;
	hitbox.width = _b.width;
	hitbox.height = _b.height;
}

Button::~Button()
{
	UnloadTexture(_b);
}

void Button::Draw(FrameParams* frame)
{
	if (CheckCollisionPointRec(frame->R1Pos, hitbox) || 
		CheckCollisionPointRec(frame->R2Pos, hitbox) )
	{
		//drawtexturewithALTtint
		DrawTextureEx(_b, _pos, 0, _scale, _c2);
	}
	
	else
	{
		//draw with regular tint
		DrawTextureEx(_b, _pos, 0, _scale, _c);
	}
}

void Button::Update(FrameParams* frame)
{
	//execute callback when button pressed down
	if ((CheckCollisionPointRec(frame->R1Pos, hitbox) && frame->input->fire1) || 
		(CheckCollisionPointRec(frame->R2Pos, hitbox) && frame->input->fire2))
	{
		Exec();
		//more likely call _func
	}
	
}

//when pressed, execute a callback function, maybe this should be a public function?
bool Button::Clicked(Vector2 mousePos)
{
	
}

//callback function
void Button::Exec()
{
	f(_arg);
}

//============================================================================

ClickAdvance::ClickAdvance(Entity* e, void (*foo) ()) : Component(e) { f = foo;}

//need callbacks again bleh
void ClickAdvance::Update(FrameParams* frame)
{
	if((frame->input->fire1 && !frame->input->prevFire1) || 
		(frame->input->fire2 && !frame->input->prevFire2))
	{
		f();
	}
	
}


//==============================================================================

SceneAdvance::SceneAdvance(Entity* e, void (*foo) (int), int arg) : Component(e) { f = foo; _arg = arg; }

void SceneAdvance::Update(FrameParams* frame)
{	
	if((frame->input->fire1) || 
		(frame->input->fire2))
	{
		Exec();
	}
}

void SceneAdvance::Exec()
{
	f(_arg);
}

//=============================================================================

DialogueAdvance::DialogueAdvance(Entity* e, void (*foo) (DialogueAdvance*)) : Component(e)
{
	f = foo;
}

void DialogueAdvance::Update(FrameParams* frame)
{
	if((frame->input->fire1) || 
		(frame->input->fire2))
	{
		//cout<<"Out the other"<<std::endl;
		Exec();
	}
}

void DialogueAdvance::Exec()
{
	f(this);
}

//==============================================================================
ControllerMovement::ControllerMovement(Entity* e) : Component(e) {}

void ControllerMovement::Update(FrameParams* frame)
{
	
}

//==============================================================================

Rotate::Rotate(Entity* e, Vector3 axis, float speed) : Component(e)
{
	_axis = axis;
	dr = speed;
	_rot = dr;
}

//this could get pretty freaky....
//divide by the framerate
void Rotate::Update(FrameParams* frame)
{
	//_rot+=dr;
	_entity->Rotate(_axis, _rot);
	
}

//==============================================================================

Oscillate::Oscillate(Entity* e, Vector3 A, Vector3 B, double frequency) : Component(e),
	_A(A), _B(B), _f(frequency)
{
	//frequency should actually be period in seconds
	_current = 0;
	_back = false;
}


void Oscillate::Update(FrameParams* frame)
{
	//_current += frame->deltaTime;
	_current++;
	Vector3 newPos;
	
	if(!_back)
	{
		newPos = Vector3Lerp(_A, _B, (_current)/(_f));
	}
	else
	{
		newPos = Vector3Lerp(_B, _A, (_current)/(_f));
	}
	
	if(_current >= _f)
	{
		_current = 0;
		_back = !_back;
	}
	
	_entity->setPosition(newPos);
}

//==================================================================================

Shot::Shot(Entity* e, MeshComponent* m, void (*foo) (Entity*)) : Component(e)
{
	_m = m;
	f = foo;
}

void Shot::Update(FrameParams* frame)
{
	BoundingBox b = _m->GetBoundingBox();
	Ray ray;
	
	if(frame->input->fire1)
	{
		ray = GetMouseRay(frame->R1Pos, *frame->cam);
		
		//execute callback
		if(CheckCollisionRayBox(ray, b))
		{
			f(_entity);
		}
	}
	
	if(frame->input->fire2)
	{
		ray = GetMouseRay(frame->R2Pos, *frame->cam);
		
		//execute callback
		if(CheckCollisionRayBox(ray, b))
		{
			f(_entity);
		}
	}
}

KillSwitch::KillSwitch(Entity* e, float d) : Component(e)
{
	duration = d;
	counter = 0;
}

void KillSwitch::Update(FrameParams* frame)
{
	counter+=frame->deltaTime;
	if(counter>=duration)
	{
		delete _entity;
	}
}

//=========================================================================================

BARK::BARK(Entity* e, float d, int id, void (*foo) (int)) : Component(e)
{
	duration = d;
	counter = 0;
	ID = id;
	f = foo;
}

void BARK::Update(FrameParams* frame)
{
	counter+=frame->deltaTime;
	if(counter>=duration)
	{
		//DialogueManager::_barks[ID] = NULL;
		f(ID);
	}
}

void BARK::Reset(int id)
{
	ID = id;
	counter = 0;
}

//=======================================================================================

HoverGlow::HoverGlow(Entity* e, MeshComponent* m) : Component (e) { highlight = false; _m = m;}

void HoverGlow::Update(FrameParams* frame)
{
	b = _m->GetBoundingBox();
	Ray ray;
	
	ray = GetMouseRay(frame->R1Pos, *frame->cam);

	if(CheckCollisionRayBox(ray, b))
	{
		cout<<"Yay!"<<std::endl;
		highlight = true;
		return;
	}
	
	ray = GetMouseRay(frame->R2Pos, *frame->cam);
		
	//execute callback
	if(CheckCollisionRayBox(ray, b))
	{
		cout<<"Yay!"<<std::endl;
		highlight = true;
		return;
	}
	
	highlight = false;
}

void HoverGlow::Draw(FrameParams* frame)
{
	//cout<<"Drawing"<<std::endl;
	if(highlight)
	{
		DrawBoundingBox(b, WHITE);
		cout<<"Drew box"<<std::endl;
	}
}

//=================================================================================

Orbit::Orbit(Entity* e, Vector3 point, float radius, float speed) : Component(e)
{
	_p = point;
	_r = radius;
	_s = speed;
	
}

void Orbit::Update(FrameParams* frame)
{
	float xT, yT, zT; float speed = 1;
	xT = sinf(PI/2) * cosf(speed) + _p.x;
	yT = cosf(PI/2) + _p.y;
	zT = sinf(PI/2) * sinf(speed) + _p.z;
	
	_entity->setPosition(Vector3{xT, yT, zT});
	
}

//===================================================================================

SphereTrigger::SphereTrigger(Entity* e, Entity* target, float radius, void (*foo) (int), int arg)
	:	Component(e)
{
	_t = target;
	r = radius;
	f = foo;
	_arg = arg;
}


void SphereTrigger::Update(FrameParams* frame)
{
	if(Vector3Distance(_t->GetPosition(), _entity->GetPosition()) < r)
	{
		f(_arg);
	}
	
}

//=======================================================================================

BoolCheck::BoolCheck(Entity* e, bool& b, bool ref,void (*foo) (int), int arg)
	: Component(e)
{
	r = ref;
	t = b;
	f = foo;
	_arg = arg;	
}

void BoolCheck::Update(FrameParams* frame)
{
	if( t == r)
	{
		f(_arg);
	}
	
}

//=======================================================================================

KillTrack::KillTrack(Entity* e, void (*foo) (int), int arg, MeshComponent* m) : Component(e)
{
	f = foo;
	_arg = arg;
	_m = m;
}

void KillTrack::Update(FrameParams* frame)
{
	BoundingBox b = _m->GetBoundingBox();
	Ray ray;
	
	if(frame->input->fire1)
	{
		ray = GetMouseRay(frame->R1Pos, *frame->cam);
		
		//execute callback
		if(CheckCollisionRayBox(ray, b))
		{
			f(_arg);
			_m->draw = false;
		}
	}
	
	if(frame->input->fire2)
	{
		ray = GetMouseRay(frame->R2Pos, *frame->cam);
		
		//execute callback
		if(CheckCollisionRayBox(ray, b))
		{
			f(_arg);
			_m->draw = false;
		}
	}
	
	if(frame->input->spec1)
	{
		f(_arg);
		_m->draw = false;
	}
}
