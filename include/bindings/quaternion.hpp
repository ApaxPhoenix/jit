#ifndef BINDINGS_QUATERNION_HPP
#define BINDINGS_QUATERNION_HPP

extern "C" {
#include <lua.h>
}

void register_quaternion(lua_State *state);

#endif