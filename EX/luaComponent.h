#pragma once

#include "entity.h"
#include "frame.h"
#include "lua.hpp"

//fix this later

/*
** A component whose logic is implemented in LUA.
*/
class LuaComponent : public Component
{
public:
	LuaComponent(class Entity* ent, const char* path);
	virtual ~LuaComponent();

	virtual void Update(struct FrameParams* frame) override;
	
	lua_State* _Lua;
};
