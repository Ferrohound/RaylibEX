#pragma once
/*
	Raylib dialogue class
		Name
		Icon
		Dialogue box
		Display position
		Dialogue tree; linked list?
		text scroll speed
		
		look at text_writing_anim.c
*/
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <cstddef>
#include <fstream>
#include <string.h>

#include "raylib.h"
#include "entity.h"
#include "component.h"


//define those slick screen positions for placing dialogue boxes
#define UPPER_RIGHT	Vector2 { 0, 0 }
#define UPPER_LEFT	Vector2 { 0, 0 }
#define LOWER_LEFT	Vector2 { 0, 0 }
#define LOWER_RIGHT	Vector2 { 0, 0 }
#define CENTER Vector2 { 0, 0 }

using std::string;
using std::cout;
using std::map;

/*
 * Character icons and whatnot
 */

//simple node class, currently no branching so no need for edges
struct Node
{
	Node* next;
	string text, speaker, position;
	float x, y;
	bool A;
};

//dialogue panel
//attach the onclick/onfire component to it 
class DialoguePanel : public Entity
{
	public:
		DialoguePanel(Vector3 p, float speed = 12, int id = 0);
		~DialoguePanel()
		{
			UnloadTexture(_panel);
		}
		//draw panel at (x,y) position with msg text and jpg image
		void Draw(FrameParams* frame);
		void Update(FrameParams* f);
		
		void Set(Node* node);
		void Bark(Node* node, int duration);
		int GetID() { return ID; }
		void ToggleJump() { jump = !jump; }
		
	private:
		//keep icon up in here?
		Texture2D _panel;
		int framesCounter;
		float _offsetx, _offsety, _iconWidth, _textSpeed;
		char _text[128];
		int _counter, _duration;
		int ID;
		Vector2 position;
		string speaker;
		bool jump;
};


//dialogue class which holds basically everything
//should probably be a static class like SceneManager
//one list of panels for popups, one for main dialogue
class DialogueManager
{
	public:
		static map<string, Texture2D> CharacterIcons;
		
		DialogueManager();
		~DialogueManager()
		{
			//unload textures then do this
			//free(_d);
		}
		//called by the onclick component
		//popup
		static void Bark(Node* node, int duration);
		static void Advance();
		static void buildTree(string path);
		static void LoadDialogue(string name, string path);
		static void Set(string name);
		static void SetPanel(DialoguePanel* P);
		static void testOutput();
		static bool IsPlaying() { return play; }
		static void Play() { play = true; }
		static void Bark(bool pos);
		
		static DialoguePanel* _barks[5];
		//void Draw();
		//void LoadIcons(map<string, Texture2D> ico);
		
	
	private:
		//get the icon based on the speaker
		//map<string, Texture2D> icons;
		//Texture2D _icon;
		//Texture2D _dialogueBox;
		//Vector2 _position;
		static DialoguePanel* _main;
		//only allow 5 barks onscreen at once
		//be careful, these have killswitches in them and could be junk, handle them safely
		static DialoguePanel* _posBarks[6];
		static DialoguePanel* _negBarks[6];
		static Node* _current;
		static bool play;
		static map<string, Node*> Dialogues;
};
