/*
 * definitions for SceneManager.cpp things
 * 
 */
#include "scenes.h"

map<GameScreen, Scene*> SceneManager::_scenes;
Scene* SceneManager::_current;
bool SceneManager::loadNextScene;
GameScreen SceneManager::nextScene;
bool SceneManager::quitGame = false;

void SetSceneState(int state)
{
	SceneManager::GetCurrentScene()->SetState(state);
}


vector<Entity*> SpawnBugs(int n, int x, int y, int z)
{
	vector<Entity*> e;
	Vector3 p = {0, 0, 0};
	
	//load models
	for(int i=0; i<n/3; i++)
	{
		p.x = GetRandomValue(x-5, x+5);
		p.y = GetRandomValue(y-5, y+5);
		p.z = GetRandomValue(z-5, z+5);
		Entity* bug = new Entity(p);
	
		MeshComponent* m = new MeshComponent(bug, "Bug1", 1.5, YELLOW);
		Oscillate* o = new Oscillate(bug, p, Vector3{p.x, p.y+5, p.z}, (double)50.0);
		HoverGlow * h = new HoverGlow(bug, m);
		
		e.push_back(bug);
		cout<<"added another bug at "<<p.x<<p.y<<p.z<<std::endl;
	}
	
	for(int i=0; i<n/3; i++)
	{
		p.x = GetRandomValue(x-5, x+5);
		p.y = GetRandomValue(y-5, y+5);
		p.z = GetRandomValue(z-5, z+5);
		Entity* bug = new Entity(p);
		MeshComponent* m = new MeshComponent(bug, "Bug2", 1.5, RED);
		Rotate* r = new Rotate(bug, Vector3{0, 1, 0}, 0.5);
		HoverGlow * h = new HoverGlow(bug, m);
		
		e.push_back(bug);
		cout<<"added another bug at "<<p.x<<p.y<<p.z<<std::endl;
	}
	
	for(int i=0; i<n/3; i++)
	{
		p.x = GetRandomValue(x-5, x+5);
		p.y = GetRandomValue(y-5, y+5);
		p.z = GetRandomValue(z-5, z+5);
		Entity* bug = new Entity(p);
		MeshComponent* m = new MeshComponent(bug, "Bug3", 1.5);
		HoverGlow * h = new HoverGlow(bug, m);
		e.push_back(bug);
		cout<<"added another bug at "<<p.x<<p.y<<p.z<<std::endl;
	}
	
	
	return e;
}

void deleteBugs(vector<Entity*> & e)
{
	for( int i = 0; i < e.size(); i++)
	{
		delete e[i];	
	}
}

void UpdateCamera(FrameParams* frame, float inclination, float azimuth)
{
	Vector3 newTarget = {0, 0, 0};
	
	float y0 = frame->cam->position.y;
	float x0 = frame->cam->position.x;
	float z0 = frame->cam->position.z;
	
	float xT = sinf(inclination) * cosf(azimuth) + x0;
	float yT = cosf(inclination) + y0;
	float zT = sinf(inclination) * sinf(azimuth) + z0;
	
	newTarget.x = xT;
	newTarget.y = yT;
	newTarget.z = zT;
	
	frame->cam->target = newTarget;
}
