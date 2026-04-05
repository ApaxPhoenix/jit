#include "bindings.hpp"
#include "transform.hpp"
#include <cassert>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

static Transform *check(lua_State *state, const int index) {
    return static_cast<Transform *>(luaL_checkudata(state, index, "Transform"));
}

static Transform *push(lua_State *state) {
    auto *transform = static_cast<Transform *>(lua_newuserdata(state, sizeof(Transform)));
    luaL_setmetatable(state, "Transform");
    return transform;
}

// Transform.new(tx, ty, tz, qx, qy, qz, qw, sx, sy, sz)
// Transform.new() -- defaults to identity
static int new_(lua_State *state) {
    const auto tx = static_cast<float>(luaL_optnumber(state, 1,  0.0));
    const auto ty = static_cast<float>(luaL_optnumber(state, 2,  0.0));
    const auto tz = static_cast<float>(luaL_optnumber(state, 3,  0.0));
    const auto qx = static_cast<float>(luaL_optnumber(state, 4,  0.0));
    const auto qy = static_cast<float>(luaL_optnumber(state, 5,  0.0));
    const auto qz = static_cast<float>(luaL_optnumber(state, 6,  0.0));
    const auto qw = static_cast<float>(luaL_optnumber(state, 7,  1.0));
    const auto sx = static_cast<float>(luaL_optnumber(state, 8,  1.0));
    const auto sy = static_cast<float>(luaL_optnumber(state, 9,  1.0));
    const auto sz = static_cast<float>(luaL_optnumber(state, 10, 1.0));
    *push(state) = Transform({tx, ty, tz}, {qx, qy, qz, qw}, {sx, sy, sz});
    return 1;
}

// Transform.from(matrix) -- decompose a Matrix4 into a Transform
static int from(lua_State *state) {
    const auto *matrix = static_cast<Matrix4 *>(luaL_checkudata(state, 1, "Matrix4"));
    *push(state) = Transform::from(*matrix);
    return 1;
}

// __index: transform.tx/ty/tz, transform.qx/qy/qz/qw, transform.sx/sy/sz, transform:method()
static int index(lua_State *state) {
    const Transform *transform = check(state, 1);
    const char *key = luaL_checkstring(state, 2);

    // translation
    if (strcmp(key, "tx") == 0) { lua_pushnumber(state, transform->translation.x); return 1; }
    if (strcmp(key, "ty") == 0) { lua_pushnumber(state, transform->translation.y); return 1; }
    if (strcmp(key, "tz") == 0) { lua_pushnumber(state, transform->translation.z); return 1; }
    // rotation
    if (strcmp(key, "qx") == 0) { lua_pushnumber(state, transform->rotation.x); return 1; }
    if (strcmp(key, "qy") == 0) { lua_pushnumber(state, transform->rotation.y); return 1; }
    if (strcmp(key, "qz") == 0) { lua_pushnumber(state, transform->rotation.z); return 1; }
    if (strcmp(key, "qw") == 0) { lua_pushnumber(state, transform->rotation.w); return 1; }
    // scale
    if (strcmp(key, "sx") == 0) { lua_pushnumber(state, transform->scale.x); return 1; }
    if (strcmp(key, "sy") == 0) { lua_pushnumber(state, transform->scale.y); return 1; }
    if (strcmp(key, "sz") == 0) { lua_pushnumber(state, transform->scale.z); return 1; }

    // fall back to metatable for methods
    luaL_getmetatable(state, "Transform");
    lua_pushvalue(state, 2);
    lua_rawget(state, -2);
    return 1;
}

// __newindex: transform.tx = 1.0 etc
static int newindex(lua_State *state) {
    Transform *transform = check(state, 1);
    const char *key      = luaL_checkstring(state, 2);
    const auto value     = static_cast<float>(luaL_checknumber(state, 3));

    if      (strcmp(key, "tx") == 0) transform->translation.x = value;
    else if (strcmp(key, "ty") == 0) transform->translation.y = value;
    else if (strcmp(key, "tz") == 0) transform->translation.z = value;
    else if (strcmp(key, "qx") == 0) transform->rotation.x = value;
    else if (strcmp(key, "qy") == 0) transform->rotation.y = value;
    else if (strcmp(key, "qz") == 0) transform->rotation.z = value;
    else if (strcmp(key, "qw") == 0) transform->rotation.w = value;
    else if (strcmp(key, "sx") == 0) transform->scale.x = value;
    else if (strcmp(key, "sy") == 0) transform->scale.y = value;
    else if (strcmp(key, "sz") == 0) transform->scale.z = value;
    else luaL_error(state, "Transform: unknown field '%s'", key);
    return 0;
}

static int multiply(lua_State *state) {
    const Transform *alpha = check(state, 1);
    const Transform *beta  = check(state, 2);
    *push(state) = *alpha * *beta;
    return 1;
}

static int eq(lua_State *state) {
    const Transform *alpha = check(state, 1);
    const Transform *beta  = check(state, 2);
    lua_pushboolean(state, *alpha == *beta);
    return 1;
}

static int tostring(lua_State *state) {
    const Transform *transform = check(state, 1);
    lua_pushfstring(state,
        "Transform(translation=(%f, %f, %f), rotation=(%f, %f, %f, %f), scale=(%f, %f, %f))",
        transform->translation.x, transform->translation.y, transform->translation.z,
        transform->rotation.x,    transform->rotation.y,    transform->rotation.z,    transform->rotation.w,
        transform->scale.x,       transform->scale.y,       transform->scale.z);
    return 1;
}

static int matrix(lua_State *state) {
    const Transform *transform = check(state, 1);
    auto *m = static_cast<Matrix4 *>(lua_newuserdata(state, sizeof(Matrix4)));
    *m = transform->matrix();
    luaL_setmetatable(state, "Matrix4");
    return 1;
}

// transform:forward(), transform:right(), transform:up() -- direction vectors
static int forward(lua_State *state) {
    const Transform *transform = check(state, 1);
    const Vector3 v = transform->forward();
    lua_pushnumber(state, v.x); lua_pushnumber(state, v.y); lua_pushnumber(state, v.z);
    return 3;
}

static int right(lua_State *state) {
    const Transform *transform = check(state, 1);
    const Vector3 v = transform->right();
    lua_pushnumber(state, v.x); lua_pushnumber(state, v.y); lua_pushnumber(state, v.z);
    return 3;
}

static int up(lua_State *state) {
    const Transform *transform = check(state, 1);
    const Vector3 v = transform->up();
    lua_pushnumber(state, v.x); lua_pushnumber(state, v.y); lua_pushnumber(state, v.z);
    return 3;
}

static int inversed(lua_State *state) {
    const Transform *transform = check(state, 1);
    *push(state) = transform->inversed();
    return 1;
}

static int interpolate(lua_State *state) {
    const Transform *alpha = check(state, 1);
    const Transform *beta  = check(state, 2);
    const auto factor      = static_cast<float>(luaL_checknumber(state, 3));
    *push(state) = alpha->interpolate(*beta, factor);
    return 1;
}

// transform:point(x, y, z) -- transform a world point
static int point(lua_State *state) {
    const Transform *transform = check(state, 1);
    const auto x = static_cast<float>(luaL_checknumber(state, 2));
    const auto y = static_cast<float>(luaL_checknumber(state, 3));
    const auto z = static_cast<float>(luaL_checknumber(state, 4));
    const Vector3 result = transform->point({x, y, z});
    lua_pushnumber(state, result.x);
    lua_pushnumber(state, result.y);
    lua_pushnumber(state, result.z);
    return 3;
}

// transform:direction(x, y, z) -- transform a direction vector (no translation)
static int direction(lua_State *state) {
    const Transform *transform = check(state, 1);
    const auto x = static_cast<float>(luaL_checknumber(state, 2));
    const auto y = static_cast<float>(luaL_checknumber(state, 3));
    const auto z = static_cast<float>(luaL_checknumber(state, 4));
    const Vector3 result = transform->direction({x, y, z});
    lua_pushnumber(state, result.x);
    lua_pushnumber(state, result.y);
    lua_pushnumber(state, result.z);
    return 3;
}

static int approximately(lua_State *state) {
    const Transform *alpha = check(state, 1);
    const Transform *beta  = check(state, 2);
    const auto epsilon     = static_cast<float>(luaL_optnumber(state, 3, 1e-5));
    lua_pushboolean(state, alpha->approximately(*beta, epsilon));
    return 1;
}

static const luaL_Reg methods[] = {
    { "matrix",        matrix        },
    { "forward",       forward       },
    { "right",         right         },
    { "up",            up            },
    { "inversed",      inversed      },
    { "interpolate",   interpolate   },
    { "point",         point         },
    { "direction",     direction     },
    { "approximately", approximately },
    { nullptr, nullptr }
};

static constexpr luaL_Reg functions[] = {
    { "new",  new_ },
    { "from", from },
    { nullptr, nullptr }
};

void register_transform(lua_State *state) {
    luaL_newmetatable(state, "Transform");

    luaL_setfuncs(state, methods, 0);

    lua_pushcfunction(state, index);    lua_setfield(state, -2, "__index");
    lua_pushcfunction(state, newindex); lua_setfield(state, -2, "__newindex");
    lua_pushcfunction(state, multiply); lua_setfield(state, -2, "__mul");
    lua_pushcfunction(state, eq);       lua_setfield(state, -2, "__eq");
    lua_pushcfunction(state, tostring); lua_setfield(state, -2, "__tostring");

    lua_pop(state, 1);

    luaL_newlib(state, functions);

    auto *identity_const = static_cast<Transform *>(lua_newuserdata(state, sizeof(Transform)));
    *identity_const = Transform::IDENTITY;
    luaL_setmetatable(state, "Transform");
    lua_setfield(state, -2, "IDENTITY");

    lua_setglobal(state, "Transform");
}