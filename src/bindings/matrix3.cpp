#include "matrix3.hpp"
#include <cassert>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

static Matrix3 *check(lua_State *state, const int index) {
    return static_cast<Matrix3 *>(luaL_checkudata(state, index, "Matrix3"));
}

static Matrix3 *push(lua_State *state) {
    auto *matrix = static_cast<Matrix3 *>(lua_newuserdata(state, sizeof(Matrix3)));
    luaL_setmetatable(state, "Matrix3");
    return matrix;
}

// Matrix3.new(scalar) -- e.g. Matrix3.new(1) for identity
// Matrix3.new()         -- defaults to zero matrix
static int new_(lua_State *state) {
    const auto diagonal = static_cast<float>(luaL_optnumber(state, 1, 0.0));
    *push(state) = Matrix3(diagonal);
    return 1;
}

// Matrix3.identity()
static int identity(lua_State *state) {
    *push(state) = Matrix3::identity();
    return 1;
}

// Matrix3.translate(x, y)
static int translate(lua_State *state) {
    const auto x = static_cast<float>(luaL_checknumber(state, 1));
    const auto y = static_cast<float>(luaL_checknumber(state, 2));
    *push(state) = Matrix3::translate({x, y});
    return 1;
}

// Matrix3.rotate(angle) -- degrees
static int rotate(lua_State *state) {
    const auto angle = static_cast<float>(luaL_checknumber(state, 1));
    *push(state) = Matrix3::rotate(angle);
    return 1;
}

// Matrix3.scale(x, y)
static int scale(lua_State *state) {
    const auto x = static_cast<float>(luaL_checknumber(state, 1));
    const auto y = static_cast<float>(luaL_checknumber(state, 2));
    *push(state) = Matrix3::scale({x, y});
    return 1;
}

// Matrix3.compose(tx, ty, rotation, sx, sy)
static int compose(lua_State *state) {
    const auto tx  = static_cast<float>(luaL_checknumber(state, 1));
    const auto ty  = static_cast<float>(luaL_checknumber(state, 2));
    const auto rot = static_cast<float>(luaL_checknumber(state, 3));
    const auto sx  = static_cast<float>(luaL_checknumber(state, 4));
    const auto sy  = static_cast<float>(luaL_checknumber(state, 5));
    *push(state) = Matrix3::compose({tx, ty}, rot, {sx, sy});
    return 1;
}

// Matrix3.orthographic(left, right, bottom, top)
static int orthographic(lua_State *state) {
    const auto left   = static_cast<float>(luaL_checknumber(state, 1));
    const auto right  = static_cast<float>(luaL_checknumber(state, 2));
    const auto bottom = static_cast<float>(luaL_checknumber(state, 3));
    const auto top    = static_cast<float>(luaL_checknumber(state, 4));
    *push(state) = Matrix3::orthographic(left, right, bottom, top);
    return 1;
}

// __index: matrix:method()
static int index(lua_State *state) {
    luaL_getmetatable(state, "Matrix3");
    lua_pushvalue(state, 2);
    lua_rawget(state, -2);
    return 1;
}

// matrix:get(row, col) -- 1-based
static int get(lua_State *state) {
    const Matrix3 *matrix = check(state, 1);
    const int row    = static_cast<int>(luaL_checkinteger(state, 2)) - 1;
    const int column = static_cast<int>(luaL_checkinteger(state, 3)) - 1;
    lua_pushnumber(state, (*matrix)(row, column));
    return 1;
}

// matrix:set(row, col, value) -- 1-based
static int set(lua_State *state) {
    Matrix3 *matrix  = check(state, 1);
    const int row    = static_cast<int>(luaL_checkinteger(state, 2)) - 1;
    const int column = static_cast<int>(luaL_checkinteger(state, 3)) - 1;
    const auto value = static_cast<float>(luaL_checknumber(state, 4));
    (*matrix)(row, column) = value;
    return 0;
}

static int add(lua_State *state) {
    const Matrix3 *alpha = check(state, 1);
    const Matrix3 *beta  = check(state, 2);
    *push(state) = *alpha + *beta;
    return 1;
}

static int subtract(lua_State *state) {
    const Matrix3 *alpha = check(state, 1);
    const Matrix3 *beta  = check(state, 2);
    *push(state) = *alpha - *beta;
    return 1;
}

static int multiply(lua_State *state) {
    const Matrix3 *alpha = check(state, 1);
    if (lua_isnumber(state, 2)) {
        const auto scalar = static_cast<float>(lua_tonumber(state, 2));
        *push(state) = *alpha * scalar;
    } else {
        const Matrix3 *beta = check(state, 2);
        *push(state) = *alpha * *beta;
    }
    return 1;
}

static int eq(lua_State *state) {
    const Matrix3 *alpha = check(state, 1);
    const Matrix3 *beta  = check(state, 2);
    lua_pushboolean(state, *alpha == *beta);
    return 1;
}

static int tostring(lua_State *state) {
    const Matrix3 *matrix = check(state, 1);
    luaL_Buffer buffer;
    luaL_buffinit(state, &buffer);
    for (int row = 0; row < 3; row++) {
        luaL_addstring(&buffer, "[ ");
        for (int col = 0; col < 3; col++) {
            lua_pushfstring(state, "%f%s", (*matrix)(row, col), col < 2 ? ", " : " ");
            luaL_addvalue(&buffer);
        }
        luaL_addstring(&buffer, "]\n");
    }
    luaL_pushresult(&buffer);
    return 1;
}

static int transposed(lua_State *state) {
    const Matrix3 *matrix = check(state, 1);
    *push(state) = matrix->transposed();
    return 1;
}

static int inverted(lua_State *state) {
    const Matrix3 *matrix = check(state, 1);
    *push(state) = matrix->inverted();
    return 1;
}

static int determinant(lua_State *state) {
    const Matrix3 *matrix = check(state, 1);
    lua_pushnumber(state, static_cast<float>(matrix->determinant()));
    return 1;
}

static int approximately(lua_State *state) {
    const Matrix3 *alpha = check(state, 1);
    const Matrix3 *beta  = check(state, 2);
    const auto epsilon  = static_cast<float>(luaL_optnumber(state, 3, 1e-5));
    lua_pushboolean(state, alpha->approximately(*beta, epsilon));
    return 1;
}

// matrix:translation() -- returns tx, ty
static int translation(lua_State *state) {
    const Matrix3 *matrix = check(state, 1);
    const Vector2 t = matrix->translation();
    lua_pushnumber(state, t.x);
    lua_pushnumber(state, t.y);
    return 2;
}

// matrix:rotation() -- returns degrees
static int rotation(lua_State *state) {
    const Matrix3 *matrix = check(state, 1);
    lua_pushnumber(state, static_cast<float>(matrix->rotation()));
    return 1;
}

// matrix:scale() -- returns sx, sy
static int get_scale(lua_State *state) {
    const Matrix3 *matrix = check(state, 1);
    const Vector2 s = matrix->scale();
    lua_pushnumber(state, s.x);
    lua_pushnumber(state, s.y);
    return 2;
}

// matrix:decompose() -- returns tx, ty, rotation, sx, sy (or nil on failure)
static int decompose(lua_State *state) {
    const Matrix3 *matrix = check(state, 1);
    Vector2 translation, scale;
    float rotation;
    if (!matrix->decompose(translation, rotation, scale)) {
        lua_pushnil(state);
        return 1;
    }
    lua_pushnumber(state, translation.x);
    lua_pushnumber(state, translation.y);
    lua_pushnumber(state, rotation);
    lua_pushnumber(state, scale.x);
    lua_pushnumber(state, scale.y);
    return 5;
}

static const luaL_Reg methods[] = {
    { "get",           get           },
    { "set",           set           },
    { "transposed",    transposed    },
    { "inverted",      inverted      },
    { "determinant",   determinant   },
    { "approximately", approximately },
    { "translation",   translation   },
    { "rotation",      rotation      },
    { "scale",         get_scale     },
    { "decompose",     decompose     },
    { nullptr, nullptr }
};

static const luaL_Reg functions[] = {
    { "new",          new_         },
    { "identity",     identity     },
    { "translate",    translate    },
    { "rotate",       rotate       },
    { "scale",        scale        },
    { "compose",      compose      },
    { "orthographic", orthographic },
    { nullptr, nullptr }
};

void register_matrix3(lua_State *state) {
    luaL_newmetatable(state, "Matrix3");

    lua_pushcfunction(state, index);    lua_setfield(state, -2, "__index");
    lua_pushcfunction(state, add);      lua_setfield(state, -2, "__add");
    lua_pushcfunction(state, subtract); lua_setfield(state, -2, "__sub");
    lua_pushcfunction(state, multiply); lua_setfield(state, -2, "__mul");
    lua_pushcfunction(state, eq);       lua_setfield(state, -2, "__eq");
    lua_pushcfunction(state, tostring); lua_setfield(state, -2, "__tostring");

    luaL_setfuncs(state, methods, 0);
    lua_pop(state, 1);

    luaL_newlib(state, functions);

    auto *identity_const = static_cast<Matrix3 *>(lua_newuserdata(state, sizeof(Matrix3)));
    *identity_const = Matrix3::IDENTITY;
    luaL_setmetatable(state, "Matrix3");
    lua_setfield(state, -2, "IDENTITY");

    lua_setglobal(state, "Matrix3");
}