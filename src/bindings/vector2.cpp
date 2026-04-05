#include "bindings.hpp"
#include "vector2.hpp"
#include <cassert>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

static Vector2 *check(lua_State *state, const int index) {
    return static_cast<Vector2 *>(luaL_checkudata(state, index, "Vector2"));
}

static Vector2 *push(lua_State *state) {
    auto *vector = static_cast<Vector2 *>(lua_newuserdata(state, sizeof(Vector2)));
    luaL_setmetatable(state, "Vector2");
    return vector;
}

// Vector2.new(x, y)
// Vector2.new() -- defaults to (0, 0)
static int new_(lua_State *state) {
    const auto x = static_cast<float>(luaL_optnumber(state, 1, 0.0));
    const auto y = static_cast<float>(luaL_optnumber(state, 2, 0.0));
    *push(state) = Vector2(x, y);
    return 1;
}

// __index: vector.x, vector.y, vector:method()
static int index(lua_State *state) {
    const Vector2 *vector = check(state, 1);
    const char *key = luaL_checkstring(state, 2);

    if (strcmp(key, "x") == 0) { lua_pushnumber(state, vector->x); }
    if (strcmp(key, "y") == 0) { lua_pushnumber(state, vector->y); }

    // fall back to metatable for methods
    luaL_getmetatable(state, "Vector2");
    lua_pushvalue(state, 2);
    lua_rawget(state, -2);
    return 1;
}

// __newindex: vector.x = 1.0 etc
static int newindex(lua_State *state) {
    Vector2 *vector  = check(state, 1);
    const char *key  = luaL_checkstring(state, 2);
    const auto value = static_cast<float>(luaL_checknumber(state, 3));

    if      (strcmp(key, "x") == 0) vector->x = value;
    else if (strcmp(key, "y") == 0) vector->y = value;
    else luaL_error(state, "Vector2: unknown field '%s'", key);
    return 0;
}

static int add(lua_State *state) {
    const Vector2 *alpha = check(state, 1);
    const Vector2 *beta  = check(state, 2);
    *push(state) = *alpha + *beta;
    return 1;
}

static int subtract(lua_State *state) {
    const Vector2 *alpha = check(state, 1);
    const Vector2 *beta  = check(state, 2);
    *push(state) = *alpha - *beta;
    return 1;
}

static int multiply(lua_State *state) {
    if (lua_isnumber(state, 1)) {
        const auto scalar     = static_cast<float>(lua_tonumber(state, 1));
        const Vector2 *vector = check(state, 2);
        *push(state) = *vector * scalar;
    } else if (lua_isnumber(state, 2)) {
        const Vector2 *vector = check(state, 1);
        const auto scalar     = static_cast<float>(lua_tonumber(state, 2));
        *push(state) = *vector * scalar;
    } else {
        const Vector2 *alpha = check(state, 1);
        const Vector2 *beta  = check(state, 2);
        *push(state) = *alpha * *beta;
    }
    return 1;
}

static int divide(lua_State *state) {
    const Vector2 *vector = check(state, 1);
    const auto scalar     = static_cast<float>(luaL_checknumber(state, 2));
    *push(state)          = *vector / scalar;
    return 1;
}

static int negate(lua_State *state) {
    const Vector2 *vector = check(state, 1);
    *push(state)          = -*vector;
    return 1;
}

static int eq(lua_State *state) {
    const Vector2 *alpha = check(state, 1);
    const Vector2 *beta  = check(state, 2);
    lua_pushboolean(state, *alpha == *beta);
    return 1;
}

static int tostring(lua_State *state) {
    const Vector2 *vector = check(state, 1);
    lua_pushfstring(state, "Vector2(%f, %f)", vector->x, vector->y);
    return 1;
}

static int length(lua_State *state) {
    const Vector2 *vector = check(state, 1);
    lua_pushnumber(state, static_cast<float>(vector->length()));
    return 1;
}

static int dot(lua_State *state) {
    const Vector2 *alpha = check(state, 1);
    const Vector2 *beta  = check(state, 2);
    lua_pushnumber(state, static_cast<float>(alpha->dot(*beta)));
    return 1;
}

static int distance(lua_State *state) {
    const Vector2 *alpha = check(state, 1);
    const Vector2 *beta  = check(state, 2);
    lua_pushnumber(state, static_cast<float>(alpha->distance(*beta)));
    return 1;
}

static int cross(lua_State *state) {
    const Vector2 *alpha = check(state, 1);
    const Vector2 *beta  = check(state, 2);
    lua_pushnumber(state, static_cast<float>(alpha->cross(*beta)));
    return 1;
}

static int dominant(lua_State *state) {
    const Vector2 *vector = check(state, 1);
    lua_pushnumber(state, static_cast<float>(vector->dominant()));
    return 1;
}

static int recessive(lua_State *state) {
    const Vector2 *vector = check(state, 1);
    lua_pushnumber(state, static_cast<float>(vector->recessive()));
    return 1;
}

static int angle(lua_State *state) {
    const Vector2 *alpha = check(state, 1);
    const Vector2 *beta  = check(state, 2);
    lua_pushnumber(state, static_cast<float>(alpha->angle(*beta)));
    return 1;
}

static int normalized(lua_State *state) {
    const Vector2 *vector = check(state, 1);
    *push(state)          = vector->normalized();
    return 1;
}

static int interpolate(lua_State *state) {
    const Vector2 *alpha = check(state, 1);
    const Vector2 *beta  = check(state, 2);
    const auto factor   = static_cast<float>(luaL_checknumber(state, 3));
    *push(state) = alpha->interpolate(*beta, factor);
    return 1;
}

static int absolute(lua_State *state) {
    const Vector2 *vector = check(state, 1);
    *push(state)          = vector->absolute();
    return 1;
}

static int perpendicular(lua_State *state) {
    const Vector2 *vector = check(state, 1);
    *push(state)          = vector->perpendicular();
    return 1;
}

static int reflect(lua_State *state) {
    const Vector2 *vector = check(state, 1);
    const Vector2 *normal = check(state, 2);
    *push(state)          = vector->reflect(*normal);
    return 1;
}

static int project(lua_State *state) {
    const Vector2 *vector = check(state, 1);
    const Vector2 *onto   = check(state, 2);
    *push(state)          = vector->project(*onto);
    return 1;
}

// vector:clamped(minx, miny, maxx, maxy)
static int clamped(lua_State *state) {
    const Vector2 *vector = check(state, 1);
    const auto minx = static_cast<float>(luaL_checknumber(state, 2));
    const auto miny = static_cast<float>(luaL_checknumber(state, 3));
    const auto maxx = static_cast<float>(luaL_checknumber(state, 4));
    const auto maxy = static_cast<float>(luaL_checknumber(state, 5));
    *push(state) = vector->clamped({minx, miny}, {maxx, maxy});
    return 1;
}

static int approximately(lua_State *state) {
    const Vector2 *alpha = check(state, 1);
    const Vector2 *beta  = check(state, 2);
    const auto epsilon  = static_cast<float>(luaL_optnumber(state, 3, 1e-5));
    lua_pushboolean(state, alpha->approximately(*beta, epsilon));
    return 1;
}

// Vector2.minimum(a, b), Vector2.maximum(a, b)
static int minimum(lua_State *state) {
    const Vector2 *alpha = check(state, 1);
    const Vector2 *beta  = check(state, 2);
    *push(state) = Vector2::minimum(*alpha, *beta);
    return 1;
}

static int maximum(lua_State *state) {
    const Vector2 *alpha = check(state, 1);
    const Vector2 *beta  = check(state, 2);
    *push(state) = Vector2::maximum(*alpha, *beta);
    return 1;
}

static const luaL_Reg methods[] = {
    { "length",        length        },
    { "dot",           dot           },
    { "distance",      distance      },
    { "cross",         cross         },
    { "dominant",      dominant      },
    { "recessive",     recessive     },
    { "angle",         angle         },
    { "normalized",    normalized    },
    { "interpolate",   interpolate   },
    { "absolute",      absolute      },
    { "perpendicular", perpendicular },
    { "reflect",       reflect       },
    { "project",       project       },
    { "clamped",       clamped       },
    { "approximately", approximately },
    { nullptr, nullptr }
};

static constexpr luaL_Reg functions[] = {
    { "new",     new_    },
    { "minimum", minimum },
    { "maximum", maximum },
    { nullptr, nullptr }
};

void register_vector2(lua_State *state) {
    luaL_newmetatable(state, "Vector2");

    lua_pushcfunction(state, index);    lua_setfield(state, -2, "__index");
    lua_pushcfunction(state, newindex); lua_setfield(state, -2, "__newindex");
    lua_pushcfunction(state, add);      lua_setfield(state, -2, "__add");
    lua_pushcfunction(state, subtract); lua_setfield(state, -2, "__sub");
    lua_pushcfunction(state, multiply); lua_setfield(state, -2, "__mul");
    lua_pushcfunction(state, divide);   lua_setfield(state, -2, "__div");
    lua_pushcfunction(state, negate);   lua_setfield(state, -2, "__unm");
    lua_pushcfunction(state, eq);       lua_setfield(state, -2, "__eq");
    lua_pushcfunction(state, tostring); lua_setfield(state, -2, "__tostring");

    luaL_setfuncs(state, methods, 0);
    lua_pop(state, 1);

    luaL_newlib(state, functions);

    // static constants
    auto push_const = [&](const char *name, const Vector2 &value) {
        *push(state) = value;
        lua_setfield(state, -2, name);
    };
    push_const("ZERO",  Vector2::ZERO);
    push_const("ONE",   Vector2::ONE);
    push_const("UP",    Vector2::UP);
    push_const("DOWN",  Vector2::DOWN);
    push_const("LEFT",  Vector2::LEFT);
    push_const("RIGHT", Vector2::RIGHT);

    lua_setglobal(state, "Vector2");
}