#pragma once

/*
	Scene needs...
	Game objects, run function, draw function, update function
	setup function, close function

*/

#define MAX_COLUMNS 20


#include <map>
#include <iostream>
#include <vector>

#include "raylib.h"
#include "raymath.h"
#include "../entity.h"
#include "../dialogue.h"
//#include "../Engine.hpp"

using std::map;
using std::cout;
using std::vector;

//scren resolution
const int screenWidth = 1824;
const int screenHeight = 984;

typedef enum GameScreen { LOGO, TITLE, CHAR, FERRO, DEMO, TIMEATTACK, OPTIONS, GAMEPLAY, CREDITS, CREDITS2, DIALOGUE } GameScreen;
/*
	Map <GameScreen, Scene*>
	Screen* current;
*/

vector<Entity*> SpawnBugs(int n, int x, int y, int z);
void deleteBugs(vector<Entity*> & e);
void SetSceneState(int state);
void UpdateCamera(FrameParams* frame, float inclination, float azimuth);

class Scene
{
	public:
		Scene() {}
		
		virtual void Load(FrameParams* frame) {}
		virtual void Unload() {}
		virtual void Reset() {}
		virtual void SetState(int s) {};
		
		virtual void Update(FrameParams* frame) {}
		virtual void Draw3D(FrameParams* frame) {}
		virtual void Draw2D(FrameParams* frame) {}
		
	protected:
		
};

class SceneManager
{
	public:
		SceneManager() 
		{
			_current = NULL;
			loadNextScene = false;
		}
		
		static Scene* GetScene(GameScreen g)
		{
			return _scenes[g]; 
		}
		
		static Scene* GetCurrentScene()
		{
			return _current;
		}
		
		static void AddScene(GameScreen g, Scene* s)
		{
			_scenes[g] = s;
		}
		static void SetScene(GameScreen g)
		{
			if(_current!=NULL)
				_current->Unload();
			_current = _scenes[g];
			if(_current == NULL)
				cout<<"Huh."<<std::endl;
			cout<<"Done setting scene"<<std::endl;
		}
		static void LoadScene(FrameParams* frame)
		{
			//cout<<"Loading Scene"<<std::endl;
			_current->Load(frame);
		}
		
		void Update(FrameParams* frame)
		{
			_current->Update(frame);
			
			BeginDrawing();
				_current->Draw3D(frame);
				_current->Draw2D(frame);
			EndDrawing();
		}
		
		static void UnloadScene(GameScreen g)
		{
			_scenes[g]->Unload();
		}
		
		void UnloadCurrent()
		{
			_current->Unload();
		}
		
		static bool loadNextScene;
		static bool quitGame;
		static GameScreen nextScene;
	private:
	/*
		Map <Gamescreen, Scene*>
		Screen* current;
	*/
		static map<GameScreen, Scene*> _scenes;
		static Scene* _current;
		
};

//display the raylib logo
class Logos: public Scene
{
	public:
		Logos() : Scene() {}	
		void Load(FrameParams* frame);
		void Unload();	
		void Reset();
		void SetState(int s);
		void Update(FrameParams* frame);
		void Draw2D(FrameParams* frame);
		
		
	private:
		int logoPositionX, logoPositionY;

		int framesCounter, lettersCount, topSideRecWidth, leftSideRecHeight, bottomSideRecWidth;
		int rightSideRecHeight;

		int state;                  // Tracking animation states (State Machine)
		float alpha;             // Useful for fading
		
		Entity* fate;
		SceneAdvance* c;
};

class FerrohoundLogo: public Scene
{
	public:
		FerrohoundLogo() : Scene () {}
		
		void Load(FrameParams* frame);
		void Unload();
		void SetState(int s);
		void Update(FrameParams* frame);
		void Draw2D(FrameParams* frame);
		
	private:
		Texture2D logo;
		int logoPositionX, logoPositionY, lettersCount;
		Vector2 pos;

		int framesCounter, state;
		float alpha;
		
		Entity* fate;
		SceneAdvance* c;
};

class TitleScreen : public Scene
{
	public:
		TitleScreen() : Scene() {}
		
		void Load(FrameParams* frame);
		void Unload();
		void SetState(int s);
		void Update(FrameParams* frame);
		void Draw2D(FrameParams* frame);
		void Draw3D(FrameParams* frame);
	
	private:
		
		Reticule* R1;
		Reticule* R2;
		
		Entity* StartButton;
		Button* B;
		
		Texture2D logo;
		Texture2D foreground;
		Texture2D background;
		Texture2D L, R, TXT;
		int counter;
		//int logoPositionX, logoPositionY, lettersCount;
		Vector3 logoPos;

		int framesCounter, state;
		float alpha;
		int bump;
		int bump2;
		
		
		Music Theme;
};

class CharSel : public Scene
{
	public:
		CharSel() : Scene() {}
		
		void Load(FrameParams* frame);
		void Unload();
		void SetState(int s);
		void Update(FrameParams* frame);
		void Draw2D(FrameParams* frame);
		//void Draw3D(FrameParams* frame);
	
	private:
		
		Reticule* R1;
		Reticule* R2;
		
		Entity* StartButton;
		Button* B;
		
		Vector2 StarPos;
		Vector2 KnightPos;
		Vector2 SamuraiPos;
		//int logoPositionX, logoPositionY, lettersCount;
		Vector3 logoPos;

		int framesCounter, state, counter;
		float alpha;
		
		Texture2D lock;
		
		Music Theme;
};

//intro logos
/*class Intro: public Scene
{
	public:
	
	private:
	
};*/

//main Demo scene
class Demo: public Scene
{
	public:
		Demo() : Scene() {}
		
		void Load(FrameParams* frame);
		void Unload();
		void SetState(int s);
		void Update(FrameParams* frame);
		void Draw3D(FrameParams* frame);
		void Draw2D(FrameParams* frame);
		
	private:
		Reticule* R1;
		Reticule* R2;
		//raylib stuffs
		// Generates some random columns
		float heights[MAX_COLUMNS];
		Vector3 positions[MAX_COLUMNS];
		Color colors[MAX_COLUMNS];
		Model terrain;
		Texture2D terrainTex;
		Vector3 terrainPosition; 
		Vector3 newTarget;
		
		vector<Entity*> _models;
		
		/*
		 * Skybox stuff
		 * should probably make a struct for this later
		 */
		 
		Mesh cube;
		Model skybox;
		Texture2D hdr;
		
		int framesCounter, state;
		int omega = 0;
		int gamma = 0;
		float x0, y0, xT, xT2, yT, yT2, z0, zT;
		float inclination, azimuth;
		
		//use this to determine which set of entities to draw and update based
		//on the current state
		vector<vector<Entity*>> Entities;
		
		Entity* Mechaloid;
		
};

//identical to demo in setup
class TimeAttack: public Scene
{
	public:
		TimeAttack() : Scene() {}
		
		void Load(FrameParams* frame);
		void Unload();
		void SetState(int s);
		void Update(FrameParams* frame);
		void Draw3D(FrameParams* frame);
		void Draw2D(FrameParams* frame);
		static void IncrementKills(int n);
		
		static int kills;
		static int prevKills;
		
	private:
		Reticule* R1;
		Reticule* R2;
		//raylib stuffs
		// Generates some random columns
		float heights[MAX_COLUMNS];
		Vector3 positions[MAX_COLUMNS];
		Color colors[MAX_COLUMNS];
		Model terrain;
		Texture2D terrainTex;
		Vector3 terrainPosition; 
		
		//use an array instead of vector? doesn't really make a difference.
		//other than more accurate indexing
		vector<vector<Entity*>> Enemies;
		
		/*
		 * Skybox stuff
		 * should probably make a struct for this later
		 */
		 
		Mesh cube;
		Model skybox;
		Texture2D hdr;
		
		int framesCounter, state, timeCounter, numEnemies;
		
};

class DialogueTest: public Scene
{
	public: 
		DialogueTest() : Scene() {}
		
		void Load(FrameParams* frame);
		void Unload();
		void SetState(int s);
		void Update(FrameParams* frame);
		void Draw3D(FrameParams* frame);
		void Draw2D(FrameParams* frame);
		
		
	private:
		Reticule* R1;
		Reticule* R2;
		//raylib stuffs
		// Generates some random columns
		float heights[MAX_COLUMNS];
		Vector3 positions[MAX_COLUMNS];
		Color colors[MAX_COLUMNS];
		Model terrain;
		Texture2D terrainTex;
		Vector3 terrainPosition; 
		Vector3 newTarget;
		
		vector<Entity*> _models;
		 
		Mesh cube;
		Model skybox;
		Texture2D hdr;
		
		int framesCounter, state;
		int omega = 0;
		int gamma = 0;
		float x0, y0, xT, xT2, yT, yT2, z0, zT;
		float inclination, azimuth;
		
		DialoguePanel* P;
		
		//=========================================
		Shader hex;
		RenderTexture2D target;
		float hexCenter[2];
		int center;
		
		//========================================
		
		Texture2D UI[2];
		Vector2 UIpos[6];	
		int UIRotCounter;
		
		Rectangle sources[3];
		Rectangle dsts[3];
	
};

class Credits: public Scene
{
	public:
		Credits() : Scene() {}
		
		void Load(FrameParams* frame);
		void Unload();
		void SetState(int s);
		void Update(FrameParams* frame);
		void Draw3D(FrameParams* frame);
		void Draw2D(FrameParams* frame);
		
	private:
		int framesCounter, state, timeCounter;
};


class Credits2: public Scene
{
	public:
		Credits2() : Scene() {}
		
		void Load(FrameParams* frame);
		void Unload();
		void SetState(int s);
		void Update(FrameParams* frame);
		void Draw3D(FrameParams* frame);
		void Draw2D(FrameParams* frame);
		
	private:
		int framesCounter, state, timeCounter;
};

