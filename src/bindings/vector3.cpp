#include "bindings/vector3.hpp"
#include "vector3.hpp"
#include <sstream>
#include <cstring>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

static Vector3 *check(lua_State *state, const int index) {
    return static_cast<Vector3 *>(luaL_checkudata(state, index, "Vector3"));
}

static void push(lua_State *state, const Vector3 &vector) {
    auto *data = static_cast<Vector3 *>(lua_newuserdata(state, sizeof(Vector3)));
    *data = vector;
    luaL_getmetatable(state, "Vector3");
    lua_setmetatable(state, -2);
}

static int vector3_new(lua_State *state) {
    push(state, Vector3(
        static_cast<float>(luaL_checknumber(state, 1)),
        static_cast<float>(luaL_checknumber(state, 2)),
        static_cast<float>(luaL_checknumber(state, 3))
    ));
    return 1;
}

static int vector3_index(lua_State *state) {
    const Vector3 *vector = check(state, 1);
    const char *key = luaL_checkstring(state, 2);

    if      (strcmp(key, "x")          == 0) { lua_pushnumber(state, vector->x); }
    else if (strcmp(key, "y")          == 0) { lua_pushnumber(state, vector->y); }
    else if (strcmp(key, "z")          == 0) { lua_pushnumber(state, vector->z); }
    else if (strcmp(key, "length")     == 0) { lua_pushnumber(state, vector->length()); }
    else if (strcmp(key, "normalized") == 0) { push(state, vector->normalized()); }
    else {
        luaL_getmetatable(state, "Vector3");
        lua_getfield(state, -1, key);
        lua_remove(state, -2);
    }

    return 1;
}

static int vector3_add(lua_State *state)  { push(state, *check(state, 1) + *check(state, 2)); return 1; }
static int vector3_sub(lua_State *state)  { push(state, *check(state, 1) - *check(state, 2)); return 1; }
static int vector3_div(lua_State *state)  { push(state, *check(state, 1) / static_cast<float>(luaL_checknumber(state, 2))); return 1; }
static int vector3_unm(lua_State *state)  { push(state, -*check(state, 1)); return 1; }
static int vector3_eq(lua_State *state)   { lua_pushboolean(state, *check(state, 1) == *check(state, 2)); return 1; }

static int vector3_mul(lua_State *state) {
    if (lua_isuserdata(state, 1) && lua_isuserdata(state, 2)) {
        push(state, *check(state, 1) * *check(state, 2));
    } else if (lua_isuserdata(state, 1)) {
        push(state, *check(state, 1) * static_cast<float>(luaL_checknumber(state, 2)));
    } else {
        push(state, *check(state, 2) * static_cast<float>(luaL_checknumber(state, 1)));
    }
    return 1;
}

static int vector3_distance(lua_State *state)    { lua_pushnumber(state, check(state, 1)->distance(*check(state, 2))); return 1; }
static int vector3_dot(lua_State *state)         { lua_pushnumber(state, check(state, 1)->dot(*check(state, 2))); return 1; }
static int vector3_cross(lua_State *state)       { push(state, check(state, 1)->cross(*check(state, 2))); return 1; }
static int vector3_interpolate(lua_State *state) { push(state, check(state, 1)->interpolate(*check(state, 2), static_cast<float>(luaL_checknumber(state, 3)))); return 1; }
static int vector3_absolute(lua_State *state)    { push(state, check(state, 1)->absolute()); return 1; }
static int vector3_clamp(lua_State *state)       { push(state, check(state, 1)->clamp(*check(state, 2), *check(state, 3))); return 1; }
static int vector3_reflect(lua_State *state)     { push(state, check(state, 1)->reflect(*check(state, 2))); return 1; }
static int vector3_project(lua_State *state)     { push(state, check(state, 1)->project(*check(state, 2))); return 1; }

static int vector3_approximately(lua_State *state) {
    const float epsilon = lua_isnumber(state, 3) ? static_cast<float>(lua_tonumber(state, 3)) : 1e-5f;
    lua_pushboolean(state, check(state, 1)->approximately(*check(state, 2), epsilon));
    return 1;
}

static int vector3_angle(lua_State *state) {
    const Vector3 *a = check(state, 1);
    const Vector3 *b = check(state, 2);
    if (lua_isuserdata(state, 3)) {
        const Vector3 *axis = check(state, 3);
        lua_pushnumber(state, a->angle(*b, axis));
    } else {
        lua_pushnumber(state, a->angle(*b));
    }
    return 1;
}

static int vector3_tostring(lua_State *state) {
    const Vector3 *vector = check(state, 1);
    std::ostringstream oss;
    oss << *vector;
    lua_pushstring(state, oss.str().c_str());
    return 1;
}

void register_vector3(lua_State *state) {
    luaL_newmetatable(state, "Vector3");

    lua_pushcfunction(state, vector3_tostring);    lua_setfield(state, -2, "__tostring");
    lua_pushcfunction(state, vector3_add);         lua_setfield(state, -2, "__add");
    lua_pushcfunction(state, vector3_sub);         lua_setfield(state, -2, "__sub");
    lua_pushcfunction(state, vector3_mul);         lua_setfield(state, -2, "__mul");
    lua_pushcfunction(state, vector3_div);         lua_setfield(state, -2, "__div");
    lua_pushcfunction(state, vector3_unm);         lua_setfield(state, -2, "__unm");
    lua_pushcfunction(state, vector3_eq);          lua_setfield(state, -2, "__eq");
    lua_pushcfunction(state, vector3_index);       lua_setfield(state, -2, "__index");
    lua_pushcfunction(state, vector3_distance);    lua_setfield(state, -2, "distance");
    lua_pushcfunction(state, vector3_dot);         lua_setfield(state, -2, "dot");
    lua_pushcfunction(state, vector3_cross);       lua_setfield(state, -2, "cross");
    lua_pushcfunction(state, vector3_interpolate); lua_setfield(state, -2, "interpolate");
    lua_pushcfunction(state, vector3_angle);       lua_setfield(state, -2, "angle");
    lua_pushcfunction(state, vector3_absolute);    lua_setfield(state, -2, "absolute");
    lua_pushcfunction(state, vector3_clamp);       lua_setfield(state, -2, "clamp");
    lua_pushcfunction(state, vector3_reflect);     lua_setfield(state, -2, "reflect");
    lua_pushcfunction(state, vector3_project);     lua_setfield(state, -2, "project");
    lua_pushcfunction(state, vector3_approximately);lua_setfield(state, -2, "approximately");

    lua_pop(state, 1);

    lua_newtable(state);
    lua_pushcfunction(state, vector3_new); lua_setfield(state, -2, "new");

    push(state, Vector3::ZERO);    lua_setfield(state, -2, "zero");
    push(state, Vector3::ONE);     lua_setfield(state, -2, "one");
    push(state, Vector3::UP);      lua_setfield(state, -2, "up");
    push(state, Vector3::DOWN);    lua_setfield(state, -2, "down");
    push(state, Vector3::LEFT);    lua_setfield(state, -2, "left");
    push(state, Vector3::RIGHT);   lua_setfield(state, -2, "right");
    push(state, Vector3::FORWARD); lua_setfield(state, -2, "forward");
    push(state, Vector3::BACK);    lua_setfield(state, -2, "back");

    lua_setglobal(state, "Vector3");
}