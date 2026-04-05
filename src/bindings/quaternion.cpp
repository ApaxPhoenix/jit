#include "bindings.hpp"
#include "quaternion.hpp"
#include <cassert>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

static Quaternion *check(lua_State *state, const int index) {
    return static_cast<Quaternion *>(luaL_checkudata(state, index, "Quaternion"));
}

static Quaternion *push(lua_State *state) {
    auto *quaternion = static_cast<Quaternion *>(lua_newuserdata(state, sizeof(Quaternion)));
    luaL_setmetatable(state, "Quaternion");
    return quaternion;
}

// Quaternion.new(x, y, z, w)
// Quaternion.new() -- defaults to identity (0, 0, 0, 1)
static int new_(lua_State *state) {
    const auto x = static_cast<float>(luaL_optnumber(state, 1, 0.0));
    const auto y = static_cast<float>(luaL_optnumber(state, 2, 0.0));
    const auto z = static_cast<float>(luaL_optnumber(state, 3, 0.0));
    const auto w = static_cast<float>(luaL_optnumber(state, 4, 1.0));
    *push(state) = Quaternion(x, y, z, w);
    return 1;
}

// Quaternion.euler(pitch, yaw, roll) -- degrees
static int from_euler(lua_State *state) {
    const auto x = static_cast<float>(luaL_checknumber(state, 1));
    const auto y = static_cast<float>(luaL_checknumber(state, 2));
    const auto z = static_cast<float>(luaL_checknumber(state, 3));
    *push(state) = Quaternion::euler({x, y, z});
    return 1;
}

// Quaternion.between(fx, fy, fz, tx, ty, tz) -- rotation from one vector to another
static int between(lua_State *state) {
    const auto fx = static_cast<float>(luaL_checknumber(state, 1));
    const auto fy = static_cast<float>(luaL_checknumber(state, 2));
    const auto fz = static_cast<float>(luaL_checknumber(state, 3));
    const auto tx = static_cast<float>(luaL_checknumber(state, 4));
    const auto ty = static_cast<float>(luaL_checknumber(state, 5));
    const auto tz = static_cast<float>(luaL_checknumber(state, 6));
    *push(state) = Quaternion::between({fx, fy, fz}, {tx, ty, tz});
    return 1;
}

// Quaternion.look(fx, fy, fz, ux, uy, uz) -- forward and up vectors
static int look(lua_State *state) {
    const auto fx = static_cast<float>(luaL_checknumber(state, 1));
    const auto fy = static_cast<float>(luaL_checknumber(state, 2));
    const auto fz = static_cast<float>(luaL_checknumber(state, 3));
    const auto ux = static_cast<float>(luaL_optnumber(state, 4, 0.0));
    const auto uy = static_cast<float>(luaL_optnumber(state, 5, 1.0));
    const auto uz = static_cast<float>(luaL_optnumber(state, 6, 0.0));
    *push(state) = Quaternion::look({fx, fy, fz}, {ux, uy, uz});
    return 1;
}

// Quaternion.around(angle, ax, ay, az) -- angle in degrees, axis vector
static int around(lua_State *state) {
    const auto angle = static_cast<float>(luaL_checknumber(state, 1));
    const auto ax    = static_cast<float>(luaL_checknumber(state, 2));
    const auto ay    = static_cast<float>(luaL_checknumber(state, 3));
    const auto az    = static_cast<float>(luaL_checknumber(state, 4));
    *push(state) = Quaternion::around(angle, {ax, ay, az});
    return 1;
}

// __index: quaternion.x, quaternion.y, quaternion.z, quaternion.w, quaternion:method()
static int index(lua_State *state) {
    const Quaternion *quaternion = check(state, 1);
    const char *key = luaL_checkstring(state, 2);

    if (strcmp(key, "x") == 0) { lua_pushnumber(state, quaternion->x); }
    if (strcmp(key, "y") == 0) { lua_pushnumber(state, quaternion->y); }
    if (strcmp(key, "z") == 0) { lua_pushnumber(state, quaternion->z); }
    if (strcmp(key, "w") == 0) { lua_pushnumber(state, quaternion->w); }

    // fall back to metatable for methods
    luaL_getmetatable(state, "Quaternion");
    lua_pushvalue(state, 2);
    lua_rawget(state, -2);
    return 1;
}

// __newindex: quaternion.x = 1.0 etc
static int newindex(lua_State *state) {
    Quaternion *quaternion = check(state, 1);
    const char *key        = luaL_checkstring(state, 2);
    const auto value       = static_cast<float>(luaL_checknumber(state, 3));

    if (strcmp(key, "x") == 0) quaternion->x = value;
    else if (strcmp(key, "y") == 0) quaternion->y = value;
    else if (strcmp(key, "z") == 0) quaternion->z = value;
    else if (strcmp(key, "w") == 0) quaternion->w = value;
    else luaL_error(state, "Quaternion: unknown field '%s'", key);
    return 0;
}

static int add(lua_State *state) {
    const Quaternion *alpha = check(state, 1);
    const Quaternion *beta  = check(state, 2);
    *push(state) = *alpha + *beta;
    return 1;
}

static int subtract(lua_State *state) {
    const Quaternion *alpha = check(state, 1);
    const Quaternion *beta  = check(state, 2);
    *push(state) = *alpha - *beta;
    return 1;
}

static int multiply(lua_State *state) {
    const Quaternion *alpha = check(state, 1);
    if (lua_isnumber(state, 2)) {
        const auto scalar = static_cast<float>(lua_tonumber(state, 2));
        *push(state) = *alpha * scalar;
    } else {
        const Quaternion *beta = check(state, 2);
        *push(state) = *alpha * *beta;
    }
    return 1;
}

static int eq(lua_State *state) {
    const Quaternion *alpha = check(state, 1);
    const Quaternion *beta  = check(state, 2);
    lua_pushboolean(state, *alpha == *beta);
    return 1;
}

static int tostring(lua_State *state) {
    const Quaternion *quaternion = check(state, 1);
    lua_pushfstring(state, "Quaternion(%f, %f, %f, %f)", quaternion->x, quaternion->y, quaternion->z, quaternion->w);
    return 1;
}

static int length(lua_State *state) {
    const Quaternion *quaternion = check(state, 1);
    lua_pushnumber(state, static_cast<float>(quaternion->length()));
    return 1;
}

static int dot(lua_State *state) {
    const Quaternion *alpha = check(state, 1);
    const Quaternion *beta  = check(state, 2);
    lua_pushnumber(state, static_cast<float>(alpha->dot(*beta)));
    return 1;
}

static int normalized(lua_State *state) {
    const Quaternion *quaternion = check(state, 1);
    *push(state) = quaternion->normalized();
    return 1;
}

static int conjugate(lua_State *state) {
    const Quaternion *quaternion = check(state, 1);
    *push(state) = quaternion->conjugate();
    return 1;
}

static int inverse(lua_State *state) {
    const Quaternion *quaternion = check(state, 1);
    *push(state) = quaternion->inverse();
    return 1;
}

static int interpolate(lua_State *state) {
    const Quaternion *alpha = check(state, 1);
    const Quaternion *beta  = check(state, 2);
    const auto factor      = static_cast<float>(luaL_checknumber(state, 3));
    *push(state) = alpha->interpolate(*beta, factor);
    return 1;
}

static int approximately(lua_State *state) {
    const Quaternion *alpha = check(state, 1);
    const Quaternion *beta  = check(state, 2);
    const auto epsilon     = static_cast<float>(luaL_optnumber(state, 3, 1e-5));
    lua_pushboolean(state, alpha->approximately(*beta, epsilon));
    return 1;
}

// quaternion:euler() -- returns pitch, yaw, roll in degrees
static int to_euler(lua_State *state) {
    const Quaternion *quaternion = check(state, 1);
    const Vector3 euler = quaternion->euler();
    lua_pushnumber(state, euler.x);
    lua_pushnumber(state, euler.y);
    lua_pushnumber(state, euler.z);
    return 3;
}

static const luaL_Reg methods[] = {
    { "length",        length        },
    { "dot",           dot           },
    { "normalized",    normalized    },
    { "conjugate",     conjugate     },
    { "inverse",       inverse       },
    { "interpolate",   interpolate   },
    { "approximately", approximately },
    { "euler",         to_euler      },
    { nullptr, nullptr }
};

static const luaL_Reg functions[] = {
    { "new",     new_       },
    { "euler",   from_euler },
    { "between", between    },
    { "look",    look       },
    { "around",  around     },
    { nullptr, nullptr }
};

void register_quaternion(lua_State *state) {
    luaL_newmetatable(state, "Quaternion");

    lua_pushcfunction(state, index);    lua_setfield(state, -2, "__index");
    lua_pushcfunction(state, newindex); lua_setfield(state, -2, "__newindex");
    lua_pushcfunction(state, add);      lua_setfield(state, -2, "__add");
    lua_pushcfunction(state, subtract); lua_setfield(state, -2, "__sub");
    lua_pushcfunction(state, multiply); lua_setfield(state, -2, "__mul");
    lua_pushcfunction(state, eq);       lua_setfield(state, -2, "__eq");
    lua_pushcfunction(state, tostring); lua_setfield(state, -2, "__tostring");

    luaL_setfuncs(state, methods, 0);
    lua_pop(state, 1);

    luaL_newlib(state, functions);

    auto *identity_const = static_cast<Quaternion *>(lua_newuserdata(state, sizeof(Quaternion)));
    *identity_const = Quaternion::IDENTITY;
    luaL_setmetatable(state, "Quaternion");
    lua_setfield(state, -2, "IDENTITY");

    lua_setglobal(state, "Quaternion");
}