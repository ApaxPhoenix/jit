#include "bindings.hpp"
#include "matrix4.hpp"
#include <cassert>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

static Matrix4 *check(lua_State *state, const int index) {
    return static_cast<Matrix4 *>(luaL_checkudata(state, index, "Matrix4"));
}

static Matrix4 *push(lua_State *state) {
    auto *matrix = static_cast<Matrix4 *>(lua_newuserdata(state, sizeof(Matrix4)));
    luaL_setmetatable(state, "Matrix4");
    return matrix;
}

// Matrix4.new(scalar) -- e.g. Matrix4.new(1) for identity
// Matrix4.new()         -- defaults to zero matrix
static int new_(lua_State *state) {
    const auto diagonal = static_cast<float>(luaL_optnumber(state, 1, 0.0));
    *push(state) = Matrix4(diagonal);
    return 1;
}

// Matrix4.identity()
static int identity(lua_State *state) {
    *push(state) = Matrix4::identity();
    return 1;
}

// Matrix4.translate(x, y, z)
static int translate(lua_State *state) {
    const auto x = static_cast<float>(luaL_checknumber(state, 1));
    const auto y = static_cast<float>(luaL_checknumber(state, 2));
    const auto z = static_cast<float>(luaL_checknumber(state, 3));
    *push(state) = Matrix4::translate({x, y, z});
    return 1;
}

// Matrix4.rotate(qx, qy, qz, qw) -- normalized quaternion
static int rotate(lua_State *state) {
    const auto qx = static_cast<float>(luaL_checknumber(state, 1));
    const auto qy = static_cast<float>(luaL_checknumber(state, 2));
    const auto qz = static_cast<float>(luaL_checknumber(state, 3));
    const auto qw = static_cast<float>(luaL_checknumber(state, 4));
    *push(state) = Matrix4::rotate({qx, qy, qz, qw});
    return 1;
}

// Matrix4.scale(x, y, z)
static int scale(lua_State *state) {
    const auto x = static_cast<float>(luaL_checknumber(state, 1));
    const auto y = static_cast<float>(luaL_checknumber(state, 2));
    const auto z = static_cast<float>(luaL_checknumber(state, 3));
    *push(state) = Matrix4::scale({x, y, z});
    return 1;
}

// Matrix4.compose(tx, ty, tz, qx, qy, qz, qw, sx, sy, sz)
static int compose(lua_State *state) {
    const auto tx = static_cast<float>(luaL_checknumber(state, 1));
    const auto ty = static_cast<float>(luaL_checknumber(state, 2));
    const auto tz = static_cast<float>(luaL_checknumber(state, 3));
    const auto qx = static_cast<float>(luaL_checknumber(state, 4));
    const auto qy = static_cast<float>(luaL_checknumber(state, 5));
    const auto qz = static_cast<float>(luaL_checknumber(state, 6));
    const auto qw = static_cast<float>(luaL_checknumber(state, 7));
    const auto sx = static_cast<float>(luaL_checknumber(state, 8));
    const auto sy = static_cast<float>(luaL_checknumber(state, 9));
    const auto sz = static_cast<float>(luaL_checknumber(state, 10));
    *push(state) = Matrix4::compose({tx, ty, tz}, {qx, qy, qz, qw}, {sx, sy, sz});
    return 1;
}

// Matrix4.perspective(fov, aspect, zNear, zFar) -- fov in radians
static int perspective(lua_State *state) {
    const auto fov    = static_cast<float>(luaL_checknumber(state, 1));
    const auto aspect = static_cast<float>(luaL_checknumber(state, 2));
    const auto zNear  = static_cast<float>(luaL_checknumber(state, 3));
    const auto zFar   = static_cast<float>(luaL_checknumber(state, 4));
    *push(state) = Matrix4::perspective(fov, aspect, zNear, zFar);
    return 1;
}

// Matrix4.orthographic(left, right, bottom, top, zNear, zFar)
static int orthographic(lua_State *state) {
    const auto left   = static_cast<float>(luaL_checknumber(state, 1));
    const auto right  = static_cast<float>(luaL_checknumber(state, 2));
    const auto bottom = static_cast<float>(luaL_checknumber(state, 3));
    const auto top    = static_cast<float>(luaL_checknumber(state, 4));
    const auto zNear  = static_cast<float>(luaL_checknumber(state, 5));
    const auto zFar   = static_cast<float>(luaL_checknumber(state, 6));
    *push(state) = Matrix4::orthographic(left, right, bottom, top, zNear, zFar);
    return 1;
}

// Matrix4.look(ex, ey, ez, tx, ty, tz, ux, uy, uz)
static int look(lua_State *state) {
    const auto ex = static_cast<float>(luaL_checknumber(state, 1));
    const auto ey = static_cast<float>(luaL_checknumber(state, 2));
    const auto ez = static_cast<float>(luaL_checknumber(state, 3));
    const auto tx = static_cast<float>(luaL_checknumber(state, 4));
    const auto ty = static_cast<float>(luaL_checknumber(state, 5));
    const auto tz = static_cast<float>(luaL_checknumber(state, 6));
    const auto ux = static_cast<float>(luaL_checknumber(state, 7));
    const auto uy = static_cast<float>(luaL_checknumber(state, 8));
    const auto uz = static_cast<float>(luaL_checknumber(state, 9));
    *push(state) = Matrix4::look({ex, ey, ez}, {tx, ty, tz}, {ux, uy, uz});
    return 1;
}

// __index: matrix:method()
static int index(lua_State *state) {
    luaL_getmetatable(state, "Matrix4");
    lua_pushvalue(state, 2);
    lua_rawget(state, -2);
    return 1;
}

// matrix:get(row, col) -- 1-based
static int get(lua_State *state) {
    const Matrix4 *matrix = check(state, 1);
    const int row    = static_cast<int>(luaL_checkinteger(state, 2)) - 1;
    const int column = static_cast<int>(luaL_checkinteger(state, 3)) - 1;
    lua_pushnumber(state, (*matrix)(row, column));
    return 1;
}

// matrix:set(row, col, value) -- 1-based
static int set(lua_State *state) {
    Matrix4 *matrix  = check(state, 1);
    const int row    = static_cast<int>(luaL_checkinteger(state, 2)) - 1;
    const int column = static_cast<int>(luaL_checkinteger(state, 3)) - 1;
    const auto value = static_cast<float>(luaL_checknumber(state, 4));
    (*matrix)(row, column) = value;
    return 0;
}

static int add(lua_State *state) {
    const Matrix4 *alpha = check(state, 1);
    const Matrix4 *beta  = check(state, 2);
    *push(state) = *alpha + *beta;
    return 1;
}

static int subtract(lua_State *state) {
    const Matrix4 *alpha = check(state, 1);
    const Matrix4 *beta  = check(state, 2);
    *push(state) = *alpha - *beta;
    return 1;
}

static int multiply(lua_State *state) {
    const Matrix4 *alpha = check(state, 1);
    if (lua_isnumber(state, 2)) {
        const auto scalar = static_cast<float>(lua_tonumber(state, 2));
        *push(state) = *alpha * scalar;
    } else {
        const Matrix4 *beta = check(state, 2);
        *push(state) = *alpha * *beta;
    }
    return 1;
}

static int eq(lua_State *state) {
    const Matrix4 *alpha = check(state, 1);
    const Matrix4 *beta  = check(state, 2);
    lua_pushboolean(state, *alpha == *beta);
    return 1;
}

static int tostring(lua_State *state) {
    const Matrix4 *matrix = check(state, 1);
    luaL_Buffer buffer;
    luaL_buffinit(state, &buffer);
    for (int row = 0; row < 4; row++) {
        luaL_addstring(&buffer, "[ ");
        for (int col = 0; col < 4; col++) {
            lua_pushfstring(state, "%f%s", (*matrix)(row, col), col < 3 ? ", " : " ");
            luaL_addvalue(&buffer);
        }
        luaL_addstring(&buffer, "]\n");
    }
    luaL_pushresult(&buffer);
    return 1;
}

static int transposed(lua_State *state) {
    const Matrix4 *matrix = check(state, 1);
    *push(state) = matrix->transposed();
    return 1;
}

static int inverted(lua_State *state) {
    const Matrix4 *matrix = check(state, 1);
    *push(state) = matrix->inverted();
    return 1;
}

static int determinant(lua_State *state) {
    const Matrix4 *matrix = check(state, 1);
    lua_pushnumber(state, static_cast<float>(matrix->determinant()));
    return 1;
}

static int approximately(lua_State *state) {
    const Matrix4 *alpha = check(state, 1);
    const Matrix4 *beta  = check(state, 2);
    const auto epsilon  = static_cast<float>(luaL_optnumber(state, 3, 1e-5));
    lua_pushboolean(state, alpha->approximately(*beta, epsilon));
    return 1;
}

// matrix:translation() -- returns tx, ty, tz
static int translation(lua_State *state) {
    const Matrix4 *matrix = check(state, 1);
    const Vector3 t = matrix->translation();
    lua_pushnumber(state, t.x);
    lua_pushnumber(state, t.y);
    lua_pushnumber(state, t.z);
    return 3;
}

// matrix:rotation() -- returns qx, qy, qz, qw
static int rotation(lua_State *state) {
    const Matrix4 *matrix = check(state, 1);
    const Quaternion q = matrix->rotation();
    lua_pushnumber(state, q.x);
    lua_pushnumber(state, q.y);
    lua_pushnumber(state, q.z);
    lua_pushnumber(state, q.w);
    return 4;
}

// matrix:scale() -- returns sx, sy, sz
static int get_scale(lua_State *state) {
    const Matrix4 *matrix = check(state, 1);
    const Vector3 s = matrix->scale();
    lua_pushnumber(state, s.x);
    lua_pushnumber(state, s.y);
    lua_pushnumber(state, s.z);
    return 3;
}

// matrix:decompose() -- returns tx, ty, tz, qx, qy, qz, qw, sx, sy, sz
static int decompose(lua_State *state) {
    const Matrix4 *matrix = check(state, 1);
    Vector3 translation, scale;
    Quaternion rotation;
    [[maybe_unused]] const bool ok = matrix->decompose(translation, rotation, scale);
    lua_pushnumber(state, translation.x);
    lua_pushnumber(state, translation.y);
    lua_pushnumber(state, translation.z);
    lua_pushnumber(state, rotation.x);
    lua_pushnumber(state, rotation.y);
    lua_pushnumber(state, rotation.z);
    lua_pushnumber(state, rotation.w);
    lua_pushnumber(state, scale.x);
    lua_pushnumber(state, scale.y);
    lua_pushnumber(state, scale.z);
    return 10;
}

// matrix:anterior(), posterior(), superior(), inferior(), dexter(), sinister() -- direction vectors
static int anterior(lua_State *state) {
    const Matrix4 *m = check(state, 1);
    const Vector3 v = m->anterior();
    lua_pushnumber(state, v.x); lua_pushnumber(state, v.y); lua_pushnumber(state, v.z);
    return 3;
}

static int posterior(lua_State *state) {
    const Matrix4 *m = check(state, 1);
    const Vector3 v = m->posterior();
    lua_pushnumber(state, v.x); lua_pushnumber(state, v.y); lua_pushnumber(state, v.z);
    return 3;
}

static int superior(lua_State *state) {
    const Matrix4 *m = check(state, 1);
    const Vector3 v = m->superior();
    lua_pushnumber(state, v.x); lua_pushnumber(state, v.y); lua_pushnumber(state, v.z);
    return 3;
}

static int inferior(lua_State *state) {
    const Matrix4 *m = check(state, 1);
    const Vector3 v = m->inferior();
    lua_pushnumber(state, v.x); lua_pushnumber(state, v.y); lua_pushnumber(state, v.z);
    return 3;
}

static int dexter(lua_State *state) {
    const Matrix4 *m = check(state, 1);
    const Vector3 v = m->dexter();
    lua_pushnumber(state, v.x); lua_pushnumber(state, v.y); lua_pushnumber(state, v.z);
    return 3;
}

static int sinister(lua_State *state) {
    const Matrix4 *m = check(state, 1);
    const Vector3 v = m->sinister();
    lua_pushnumber(state, v.x); lua_pushnumber(state, v.y); lua_pushnumber(state, v.z);
    return 3;
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
    { "anterior",      anterior      },
    { "posterior",     posterior     },
    { "superior",      superior      },
    { "inferior",      inferior      },
    { "dexter",        dexter        },
    { "sinister",      sinister      },
    { nullptr, nullptr }
};

static const luaL_Reg functions[] = {
    { "new",          new_         },
    { "identity",     identity     },
    { "translate",    translate    },
    { "rotate",       rotate       },
    { "scale",        scale        },
    { "compose",      compose      },
    { "perspective",  perspective  },
    { "orthographic", orthographic },
    { "look",         look         },
    { nullptr, nullptr }
};

void register_matrix4(lua_State *state) {
    luaL_newmetatable(state, "Matrix4");

    lua_pushcfunction(state, index);    lua_setfield(state, -2, "__index");
    lua_pushcfunction(state, add);      lua_setfield(state, -2, "__add");
    lua_pushcfunction(state, subtract); lua_setfield(state, -2, "__sub");
    lua_pushcfunction(state, multiply); lua_setfield(state, -2, "__mul");
    lua_pushcfunction(state, eq);       lua_setfield(state, -2, "__eq");
    lua_pushcfunction(state, tostring); lua_setfield(state, -2, "__tostring");

    luaL_setfuncs(state, methods, 0);
    lua_pop(state, 1);

    luaL_newlib(state, functions);

    auto *identity_const = static_cast<Matrix4 *>(lua_newuserdata(state, sizeof(Matrix4)));
    *identity_const = Matrix4::IDENTITY;
    luaL_setmetatable(state, "Matrix4");
    lua_setfield(state, -2, "IDENTITY");

    lua_setglobal(state, "Matrix4");
}