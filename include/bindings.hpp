#ifndef BINDINGS_HPP
#define BINDINGS_HPP

extern "C" {
#include <lua.h>
}

void register_color3(lua_State *state);
void register_color4(lua_State *state);
void register_matrix3(lua_State *state);
void register_matrix4(lua_State *state);
void register_quaternion(lua_State *state);
void register_scalar(lua_State *state);
void register_transform(lua_State *state);
void register_vector2(lua_State *state);
void register_vector3(lua_State *state);
void register_vector4(lua_State *state);

#endif