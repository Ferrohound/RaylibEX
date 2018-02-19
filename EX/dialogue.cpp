/*
	Raylib Dialogue class
*/
#include <iostream>
#include <fstream>

#include "dialogue.h"

map<string, Texture2D> DialogueManager::CharacterIcons;
map<string, Node*> DialogueManager::Dialogues;
bool DialogueManager::play = false;

DialoguePanel* DialogueManager::_main = NULL;
DialoguePanel* DialogueManager::_barks[5];
DialoguePanel* DialogueManager::_posBarks[6];
DialoguePanel* DialogueManager::_negBarks[6];
Node* DialogueManager::_current = NULL;

//helper function for parsing lines
Node* ParseLine(string s)
{
	/*char c;
	
	for( int i=0; i<l.length(); i++)
	{
		c = l[i];
		//std::atoi(str.c_str());->subtr
	}*/
	Node* node = new Node;
	
	string delimiter = "|";
	size_t pos = 0;
	int i = 0;
	string token[5];
	while ((pos = s.find(delimiter)) != string::npos) {
		token[i] = s.substr(0, pos);
		//cout << "TOKEN IS " << token[i] << " i IS " << i <<  std::endl;
		s.erase(0, pos + delimiter.length());
		i++;
	}
	token[i] = s;
	//std::cout << s << std::endl;
	
	if(token[0] == "STOP")
	{
		node->speaker = "";
		node->position = "";
		node->text = "";
		node->A = false;
		return node;
	}
	
	if(token[0] == "SETSTATE")
	{
		node->speaker = "";
		node->position = "";
		node->text = "";
		node->A = false;
		return node;
	}
	
	node->speaker = token[0];
	node->position = token[1];
	node->text = token[2];
	
	//cout<<node->speaker<< " says " << node->text<<std::endl;
	
	//cout<<node->text<<std::endl;
	
	if(i == 3)
		node->A = true;
	
	else
		node->A = false;
	
	return node;
}

void AdvanceDialogue(DialogueAdvance* D);
void dialogueTurnover(DialogueAdvance* D);

//advance the dialogue and set it back to thing
void AdvanceDialogue(DialogueAdvance* D)
{
	DialogueManager::Advance();
	D->setCallback(dialogueTurnover);
}

//change the callback to AdvanceDialogue()
void dialogueTurnover(DialogueAdvance* D)
{
	D->setCallback(AdvanceDialogue);
}

void FreeBark(int id)
{
	DialogueManager::_barks[id] = NULL;
}

//==================================================Panel

//set the dialogue panel
DialoguePanel::DialoguePanel(Vector3 p, float speed, int id) : Entity(p)
{
	_panel = LoadTexture("res/2D/DialoguePanel2.png");
	_counter = 0;
	position = Vector2{p.x, p.y};
	_offsetx = 1;
	_offsety = 1;
	_duration = -1;
	ID = id;
	jump = false;
}

//draw panel at (x,y) position with msg text and jpg image
void DialoguePanel::Draw(FrameParams* frame)
{
	//draw the panel below
	DrawTexture(_panel, position.x, position.y, WHITE);
	
	//scrolling text
	if(jump)
	{
	DrawText(SubText(_text, 0, framesCounter/_textSpeed), position.x + DialogueManager::CharacterIcons[speaker].width + _offsetx,
		position.y + _offsety, 10, WHITE);
	}
	else
	{
		DrawText(_text, position.x + DialogueManager::CharacterIcons[speaker].width + _offsetx,
		position.y + _offsety, 10, WHITE);
	}
		
	DrawTexture(DialogueManager::CharacterIcons[speaker], position.x + _offsetx, position.y + _offsety, WHITE);
	
}

void DialoguePanel::Update(FrameParams* f)
{
	framesCounter++;
	_counter++;
	
	//if this is a bark, compare against duration then delete yourself
	if(_duration == -1)
		return;
	if(_counter>=_duration)
		delete this;
}

//bark should be a different thing?
void DialoguePanel::Bark(Node* node, int duration)
{
	_duration = duration;
}

void DialoguePanel::Set(Node* node)
{
	//Vector2 position, string text, string speaker
	strcpy(_text, node->text.c_str());
	speaker = node->speaker;
}

//===================================================Dialogue
DialogueManager::DialogueManager()
{
	
}

void DialogueManager::SetPanel(DialoguePanel* P)
{
	_main = P;
}

//dialogue panels could have
void DialogueManager::Advance()
{
	cout<<"Advancing"<<std::endl;
	if(_current!=NULL)
	{
		_current = _current->next;	
	}
		
	else
		play = false;
}

void DialogueManager::Bark(Node* node, int duration)
{
	
}

void DialogueManager::buildTree(string path)
{
	//go through the file
	string line;
	std::ifstream FILE (path);
	
	Node* head = NULL;
	if(FILE.is_open())
	{
		//head node
		head = new Node {NULL, "", "", "", 0, 0 };
		Node* prev = head;
		
		while (getline ( FILE, line ))
		{
		  Node* tmp = ParseLine(line);
		  prev->next = tmp;
		}
	}
	_current = head;
}

//load the dialogue sequence and add it to the map of dialogues
void DialogueManager::LoadDialogue(string name, string path)
{
	//go through the file
	string line;
	std::ifstream FILE (path);
	
	Node* head = NULL;
	if(FILE.is_open())
	{
		cout<<"File successfully opened!"<<std::endl;
		//head node
		head = new Node {NULL, "", "", "", 0, 0 };
		Node* current = head;
		current->next = NULL;
		while (getline ( FILE, line ))
		{
			Node* tmp = ParseLine(line);
			current->next = tmp;
			current = current->next;
			current->next = NULL;
		}
	}
	_current = head;
	Dialogues[name] = _current;
	
	cout<<"Done loading dialogue to " << name <<std::endl;
	cout<<"Node text is "<<Dialogues[name]->next->text<<std::endl;
}

void DialogueManager::Set(string name)
{
	_current = Dialogues[name];
	
}

//add it to list of barks with killSwitch component
void DialogueManager::Bark(bool pos)
{
	int i, j;
	while(i<6)
	{
		if(_barks[i] == NULL)
		{
			break;
		}
		i++;
	}
	//no space in bark queue
	if(i==6)
		return;
	
	j = GetRandomValue(0, 5);	
	if(pos)
	{
		_barks[i] = _posBarks[j];
	}
	else
	{
		_barks[i] = _negBarks[j];
	}
	
	_barks[i]->clearComponents();
	BARK* tmp = new BARK(_barks[i], 5, i, FreeBark);
	
}

void DialogueManager::testOutput()
{
	if(_current!=NULL)
		cout<<_current->text<<std::endl;
}
