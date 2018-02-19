#pragma once

#include <iostream>
#include "luaComponent.h"
#include "lua.hpp"

//======================Lua registry functions======================================
//boolean of some kind, takes as an argument frame params
static int cprint(lua_State* L)
{
	//retrieve the params pointer
	std::string tp = (std::string) lua_tostring(L, 1);
	std::cout<<tp<<std::endl;
	
	lua_pushstring(L, tp.c_str());
	return 1;
}


static int frame_params_get_input_left(lua_State* L)
{
	//retrieve the params pointer
	/*ga_frame_params* params = (ga_frame_params*) lua_touserdata(L, 1);
	
	//check if specified key is pressed
	if(params->_button_mask == k_button_o)
	{
		lua_pushboolean(L, 1);
		//0 is true
	}
	else
	{
		lua_pushboolean(L, 0);
	}
	
	return 1;*/
}


//boolean of some kind, whether or not a certain key is pressed
static int frame_params_get_input_right(lua_State* L)
{
	/*//retrieve the params pointer
	ga_frame_params* params = (ga_frame_params*) lua_touserdata(L, 1);

	//check if specified key is pressed
	if(params->_button_mask == k_button_p)
	{
		lua_pushboolean(L, 1);
		//0 is true
	}
	else
	{
		lua_pushboolean(L, 0);
	}
	return 1;*/
}


//returns the state
static int LuaGetInput(lua_State* L)
{
	//retrieve the params pointer
	FrameParams* params = (FrameParams*) lua_touserdata(L, 1);
	float x, y, z;
	x = y = z = 0;
	//z = 0;
	//check if specified key is pressed
	/*if(params->_button_mask == k_button_i)
	{
		y = 1;
	}
	else if(params->_button_mask == k_button_k)
	{
		y = -1;
	}
	else
	{
		y = 0;
	}
	
	if(params->_button_mask == k_button_j)
	{
		x = -1;
	}
	else if(params->_button_mask == k_button_l)
	{
		x = 1;
	}
	else
	{
		x = 0;
	}*/
	
	//push values up
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, z);
	
	return 3;
}

//return the entity of the component, takes as an argument a component, presumibly
//returns the component's entity
//probably push the address casted as an integer
static int LuaComponentGetEntity(lua_State* L)
{
	//retrieve component pointer
	LuaComponent* component = (LuaComponent*) lua_touserdata(L, 1);
	
	//get the entity
	Entity* entity = component->getEntity();
	
	//push it
	lua_pushlightuserdata(L, entity);
	return 1;
	
}

//apply the translation of the component, takes as arguments an entity and
//x, y, z transforms
static int EntityTranslate(lua_State* L)
{	
	//(L, 0) is the number of arguments
	Entity* entity = (Entity*) lua_touserdata(L, 1);
	
	//get the x, y, z values
	float x = (float)lua_tonumber(L, 2);
	float y = (float)lua_tonumber(L, 3);
	float z = (float)lua_tonumber(L, 4);
	
	//translate the entity
	Vector3 translation {x, y, z};
	//translation.x = x;
	//translation.y = y;
	//translation.z = z;
	
	entity->Translate(translation);
	return 0;
}


LuaComponent::LuaComponent(Entity* ent, const char* path) : Component(ent)
{
	//initialize Lua
	_Lua = luaL_newstate();
	//loat Lua base libraries
	luaL_openlibs(_Lua);
	
	extern char g_root_path[256];
	std::string fullpath = "";
	fullpath += path;
	
	//load the script
	int status = luaL_dofile(_Lua, fullpath.c_str());
	
	//error check
	if (status)
	{
		std::cerr << "Failed to load script " << path << ": " << lua_tostring(_Lua, -1);
		lua_close(_Lua);
	}
	
	//register functions
	//lua_register(_Lua, "frame_params_get_input_left", frame_params_get_input_left);
	//lua_register(_Lua, "frame_params_get_input_right", frame_params_get_input_right);
	lua_register(_Lua, "LuaGetInput", LuaGetInput);
	lua_register(_Lua, "LuaComponentGetEntity", LuaComponentGetEntity);
	lua_register(_Lua, "EntityTranslate", EntityTranslate);
	lua_register(_Lua, "cprint", cprint);
	
	
}

LuaComponent::~LuaComponent()
{
	lua_close(_Lua);
}

void LuaComponent::Update(FrameParams* frame)
{
	/*
		/* the function name 
		lua_getglobal(L, "add");

		/* the first argument 
		lua_pushnumber(L, x);

		/* the second argument 
		lua_pushnumber(L, y);

		/* call the function with 2 arguments, return 1 result 
		lua_call(L, 2, 1);
		
		lua_pcall(Lua, number of arguments, number of return values, error function)
	*/
	
	//the function name
	lua_getglobal(_Lua, "update2");
	
	//first argument, push a pointer to component
	lua_pushlightuserdata(_Lua, this);

	//second argument, push pointer to params
	lua_pushlightuserdata(_Lua, frame);
	
	//call the function with 2 arguments and no results
	lua_call(_Lua, 2, 0);
}
