#include "bindings/quaternion.hpp"
#include "quaternion.hpp"
#include <sstream>
#include <cstring>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

static Quaternion *check(lua_State *state, const int index) {
    return static_cast<Quaternion *>(luaL_checkudata(state, index, "Quaternion"));
}

static void push(lua_State *state, const Quaternion &quaternion) {
    auto *data = static_cast<Quaternion *>(lua_newuserdata(state, sizeof(Quaternion)));
    *data = quaternion;
    luaL_getmetatable(state, "Quaternion");
    lua_setmetatable(state, -2);
}

static Vector3 *checkVector3(lua_State *state, const int index) {
    return static_cast<Vector3 *>(luaL_checkudata(state, index, "Vector3"));
}

static void pushVector3(lua_State *state, const Vector3 &vector) {
    auto *data = static_cast<Vector3 *>(lua_newuserdata(state, sizeof(Vector3)));
    *data = vector;
    luaL_getmetatable(state, "Vector3");
    lua_setmetatable(state, -2);
}

static int quaternion_new(lua_State *state) {
    push(state, Quaternion(
        static_cast<float>(luaL_checknumber(state, 1)),
        static_cast<float>(luaL_checknumber(state, 2)),
        static_cast<float>(luaL_checknumber(state, 3)),
        static_cast<float>(luaL_checknumber(state, 4))
    ));
    return 1;
}

static int quaternion_index(lua_State *state) {
    const Quaternion *quaternion = check(state, 1);
    const char *key = luaL_checkstring(state, 2);

    if      (strcmp(key, "x")          == 0) { lua_pushnumber(state, quaternion->x); }
    else if (strcmp(key, "y")          == 0) { lua_pushnumber(state, quaternion->y); }
    else if (strcmp(key, "z")          == 0) { lua_pushnumber(state, quaternion->z); }
    else if (strcmp(key, "w")          == 0) { lua_pushnumber(state, quaternion->w); }
    else if (strcmp(key, "length")     == 0) { lua_pushnumber(state, quaternion->length()); }
    else if (strcmp(key, "normalized") == 0) { push(state, quaternion->normalized()); }
    else if (strcmp(key, "conjugate")  == 0) { push(state, quaternion->conjugate()); }
    else if (strcmp(key, "inverse")    == 0) { push(state, quaternion->inverse()); }
    else if (strcmp(key, "euler")      == 0) { pushVector3(state, quaternion->euler()); }
    else {
        luaL_getmetatable(state, "Quaternion");
        lua_getfield(state, -1, key);
        lua_remove(state, -2);
    }

    return 1;
}

static int quaternion_add(lua_State *state) { push(state, *check(state, 1) + *check(state, 2)); return 1; }
static int quaternion_sub(lua_State *state) { push(state, *check(state, 1) - *check(state, 2)); return 1; }
static int quaternion_eq(lua_State *state)  { lua_pushboolean(state, *check(state, 1) == *check(state, 2)); return 1; }

static int quaternion_mul(lua_State *state) {
    if (lua_isuserdata(state, 1) && lua_isuserdata(state, 2)) {
        if (luaL_testudata(state, 2, "Vector3")) {
            pushVector3(state, *check(state, 1) * *checkVector3(state, 2));
        } else {
            push(state, *check(state, 1) * *check(state, 2));
        }
    } else if (lua_isuserdata(state, 1)) {
        push(state, *check(state, 1) * static_cast<float>(luaL_checknumber(state, 2)));
    } else {
        push(state, *check(state, 2) * static_cast<float>(luaL_checknumber(state, 1)));
    }
    return 1;
}

static int quaternion_dot(lua_State *state)         { lua_pushnumber(state, check(state, 1)->dot(*check(state, 2))); return 1; }
static int quaternion_interpolate(lua_State *state) { push(state, check(state, 1)->interpolate(*check(state, 2), static_cast<float>(luaL_checknumber(state, 3)))); return 1; }

static int quaternion_approximately(lua_State *state) {
    const float epsilon = lua_isnumber(state, 3) ? static_cast<float>(lua_tonumber(state, 3)) : 1e-5f;
    lua_pushboolean(state, check(state, 1)->approximately(*check(state, 2), epsilon));
    return 1;
}

static int quaternion_euler(lua_State *state) {
    push(state, Quaternion::euler(*checkVector3(state, 1)));
    return 1;
}

static int quaternion_between(lua_State *state) {
    push(state, Quaternion::between(*checkVector3(state, 1), *checkVector3(state, 2)));
    return 1;
}

static int quaternion_look(lua_State *state) {
    if (lua_isuserdata(state, 2)) {
        push(state, Quaternion::look(*checkVector3(state, 1), *checkVector3(state, 2)));
    } else {
        push(state, Quaternion::look(*checkVector3(state, 1)));
    }
    return 1;
}

static int quaternion_around(lua_State *state) {
    push(state, Quaternion::around(
        static_cast<float>(luaL_checknumber(state, 1)),
        *checkVector3(state, 2)
    ));
    return 1;
}

static int quaternion_tostring(lua_State *state) {
    const Quaternion *quaternion = check(state, 1);
    std::ostringstream oss;
    oss << *quaternion;
    lua_pushstring(state, oss.str().c_str());
    return 1;
}

void register_quaternion(lua_State *state) {
    luaL_newmetatable(state, "Quaternion");

    lua_pushcfunction(state, quaternion_tostring);    lua_setfield(state, -2, "__tostring");
    lua_pushcfunction(state, quaternion_add);         lua_setfield(state, -2, "__add");
    lua_pushcfunction(state, quaternion_sub);         lua_setfield(state, -2, "__sub");
    lua_pushcfunction(state, quaternion_mul);         lua_setfield(state, -2, "__mul");
    lua_pushcfunction(state, quaternion_eq);          lua_setfield(state, -2, "__eq");
    lua_pushcfunction(state, quaternion_index);       lua_setfield(state, -2, "__index");
    lua_pushcfunction(state, quaternion_dot);         lua_setfield(state, -2, "dot");
    lua_pushcfunction(state, quaternion_interpolate); lua_setfield(state, -2, "interpolate");
    lua_pushcfunction(state, quaternion_approximately);lua_setfield(state, -2, "approximately");

    lua_pop(state, 1);

    lua_newtable(state);
    lua_pushcfunction(state, quaternion_new);     lua_setfield(state, -2, "new");
    lua_pushcfunction(state, quaternion_euler);   lua_setfield(state, -2, "euler");
    lua_pushcfunction(state, quaternion_between); lua_setfield(state, -2, "between");
    lua_pushcfunction(state, quaternion_look);    lua_setfield(state, -2, "look");
    lua_pushcfunction(state, quaternion_around);  lua_setfield(state, -2, "around");

    push(state, Quaternion::IDENTITY); lua_setfield(state, -2, "identity");

    lua_setglobal(state, "Quaternion");
}