
#include "color3.hpp"
#include <cassert>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

static Color3 *check(lua_State *state, const int index) {
    return static_cast<Color3 *>(luaL_checkudata(state, index, "Color3"));
}

// Color3.new(r, g, b)
// Color3.new() -- defaults to (0, 0, 0)
static int new_(lua_State *state) {
    const auto r = static_cast<float>(luaL_optnumber(state, 1, 0.0));
    const auto g = static_cast<float>(luaL_optnumber(state, 2, 0.0));
    const auto b = static_cast<float>(luaL_optnumber(state, 3, 0.0));
    auto *color = static_cast<Color3 *>(lua_newuserdata(state, sizeof(Color3)));
    *color = Color3(r, g, b);
    luaL_setmetatable(state, "Color3");
    return 1;
}

// Color3.rgb(r, g, b) -- 0-255 range
static int rgb(lua_State *state) {
    const float r = static_cast<float>(luaL_checknumber(state, 1)) / 255.0f;
    const float g = static_cast<float>(luaL_checknumber(state, 2)) / 255.0f;
    const float b = static_cast<float>(luaL_checknumber(state, 3)) / 255.0f;
    auto *color  = static_cast<Color3 *>(lua_newuserdata(state, sizeof(Color3)));
    *color = Color3(r, g, b);
    luaL_setmetatable(state, "Color3");
    return 1;
}

// Color3.hsv(h, s, v) -- h: [0, 360), s: [0, 1], v: [0, 1]
static int hsv(lua_State *state) {
    const auto h = static_cast<float>(luaL_checknumber(state, 1));
    const auto s = static_cast<float>(luaL_checknumber(state, 2));
    const auto v = static_cast<float>(luaL_checknumber(state, 3));
    auto *color = static_cast<Color3 *>(lua_newuserdata(state, sizeof(Color3)));
    *color = Color3::hsv(h, s, v);
    luaL_setmetatable(state, "Color3");
    return 1;
}

// Color3.fromHex(hex) -- e.g. 0xFF8800
static int hex(lua_State *state) {
    const auto hex = static_cast<unsigned int>(luaL_checkinteger(state, 1));
    auto *color = static_cast<Color3 *>(lua_newuserdata(state, sizeof(Color3)));
    *color = Color3::hex(hex);
    luaL_setmetatable(state, "Color3");
    return 1;
}

// __index: color.red, color.green, color.blue, color:method()
static int index(lua_State *state) {
    const Color3 *color = check(state, 1);
    const char *key = luaL_checkstring(state, 2);

    if (strcmp(key, "red")   == 0) { lua_pushnumber(state, color->red); }
    if (strcmp(key, "green") == 0) { lua_pushnumber(state, color->green); }
    if (strcmp(key, "blue")  == 0) { lua_pushnumber(state, color->blue); }

    // fall back to metatable for methods
    luaL_getmetatable(state, "Color3");
    lua_pushvalue(state, 2);
    lua_rawget(state, -2);
    return 1;
}

// __newindex: color.red = 1.0 etc
static int newindex(lua_State *state) {
    Color3 *color = check(state, 1);
    const char *key = luaL_checkstring(state, 2);
    const auto value = static_cast<float>(luaL_checknumber(state, 3));

    if (strcmp(key, "red")   == 0) color->red   = value;
    else if (strcmp(key, "green") == 0) color->green = value;
    else if (strcmp(key, "blue")  == 0) color->blue  = value;
    else luaL_error(state, "Color3: unknown field '%s'", key);
    return 0;
}

static int add(lua_State *state) {
    const Color3 *alpha = check(state, 1);
    const Color3 *beta  = check(state, 2);
    auto *result = static_cast<Color3 *>(lua_newuserdata(state, sizeof(Color3)));
    *result = *alpha + *beta;
    luaL_setmetatable(state, "Color3");
    return 1;
}

static int sub(lua_State *state) {
    const Color3 *alpha = check(state, 1);
    const Color3 *beta  = check(state, 2);
    auto *result = static_cast<Color3 *>(lua_newuserdata(state, sizeof(Color3)));
    *result = *alpha - *beta;
    luaL_setmetatable(state, "Color3");
    return 1;
}

static int mul(lua_State *state) {
    auto *result = static_cast<Color3 *>(lua_newuserdata(state, sizeof(Color3)));
    if (lua_isnumber(state, 1)) {
        const auto scalar   = static_cast<float>(lua_tonumber(state, 1));
        const Color3 *color = check(state, 2);
        *result = *color * scalar;
    } else if (lua_isnumber(state, 2)) {
        const Color3 *color = check(state, 1);
        const auto scalar   = static_cast<float>(lua_tonumber(state, 2));
        *result = *color * scalar;
    } else {
        const Color3 *alpha = check(state, 1);
        const Color3 *beta  = check(state, 2);
        *result = *alpha * *beta;
    }
    luaL_setmetatable(state, "Color3");
    return 1;
}

static int div(lua_State *state) {
    const Color3 *color = check(state, 1);
    const auto scalar   = static_cast<float>(luaL_checknumber(state, 2));
    auto *result = static_cast<Color3 *>(lua_newuserdata(state, sizeof(Color3)));
    *result = *color / scalar;
    luaL_setmetatable(state, "Color3");
    return 1;
}

static int eq(lua_State *state) {
    const Color3 *alpha = check(state, 1);
    const Color3 *beta  = check(state, 2);
    lua_pushboolean(state, *alpha == *beta);
    return 1;
}

static int tostring(lua_State *state) {
    const Color3 *color = check(state, 1);
    lua_pushfstring(state, "Color3(%f, %f, %f)", color->red, color->green, color->blue);
    return 1;
}

static int inverted(lua_State *state) {
    const Color3 *color = check(state, 1);
    auto *result = static_cast<Color3 *>(lua_newuserdata(state, sizeof(Color3)));
    *result = color->inverted();
    luaL_setmetatable(state, "Color3");
    return 1;
}

static int clamped(lua_State *state) {
    const Color3 *color = check(state, 1);
    auto *result = static_cast<Color3 *>(lua_newuserdata(state, sizeof(Color3)));
    *result = color->clamped();
    luaL_setmetatable(state, "Color3");
    return 1;
}

static int interpolate(lua_State *state) {
    const Color3 *alpha = check(state, 1);
    const Color3 *beta  = check(state, 2);
    const auto factor  = static_cast<float>(luaL_checknumber(state, 3));
    auto *result      = static_cast<Color3 *>(lua_newuserdata(state, sizeof(Color3)));
    *result = alpha->interpolate(*beta, factor);
    luaL_setmetatable(state, "Color3");
    return 1;
}

static int gamma(lua_State *state) {
    Color3 *color        = check(state, 1);
    const auto exponent = static_cast<float>(luaL_checknumber(state, 2));
    auto *result       = static_cast<Color3 *>(lua_newuserdata(state, sizeof(Color3)));
    *result = color->gamma(exponent);
    luaL_setmetatable(state, "Color3");
    return 1;
}

static int luminance(lua_State *state) {
    const Color3 *color = check(state, 1);
    lua_pushnumber(state, static_cast<float>(color->luminance()));
    return 1;
}

static int difference(lua_State *state) {
    const Color3 *alpha = check(state, 1);
    const Color3 *beta  = check(state, 2);
    lua_pushnumber(state, static_cast<float>(alpha->difference(*beta)));
    return 1;
}

static int approximately(lua_State *state) {
    const Color3 *alpha       = check(state, 1);
    const Color3 *beta        = check(state, 2);
    const auto epsilon = static_cast<float>(luaL_optnumber(state, 3, 1e-5));
    lua_pushboolean(state, alpha->approximately(*beta, epsilon));
    return 1;
}

// push a Color3 constant as userdata into the table on top of the stack
static void push_constant(lua_State *state, const char *name, const Color3 &value) {
    auto *color = static_cast<Color3 *>(lua_newuserdata(state, sizeof(Color3)));
    *color        = value;
    luaL_setmetatable(state, "Color3");
    lua_setfield(state, -2, name);
}

static const luaL_Reg methods[] = {
    { "inverted",      inverted      },
    { "clamped",       clamped       },
    { "interpolate",   interpolate   },
    { "gamma",         gamma         },
    { "luminance",     luminance     },
    { "difference",    difference    },
    { "approximately", approximately },
    { nullptr, nullptr }
};

static const luaL_Reg functions[] = {
    { "new",     new_      },
    { "rgb",     rgb       },
    { "hsv",     hsv       },
    { "hex",     hex       },
    { nullptr, nullptr }
};

void register_color3(lua_State *state) {
    luaL_newmetatable(state, "Color3");

    lua_pushcfunction(state, index);     lua_setfield(state, -2, "__index");
    lua_pushcfunction(state, newindex);  lua_setfield(state, -2, "__newindex");
    lua_pushcfunction(state, add);       lua_setfield(state, -2, "__add");
    lua_pushcfunction(state, sub);       lua_setfield(state, -2, "__sub");
    lua_pushcfunction(state, mul);       lua_setfield(state, -2, "__mul");
    lua_pushcfunction(state, div);       lua_setfield(state, -2, "__div");
    lua_pushcfunction(state, eq);        lua_setfield(state, -2, "__eq");
    lua_pushcfunction(state, tostring);  lua_setfield(state, -2, "__tostring");

    luaL_setfuncs(state, methods, 0);
    lua_pop(state, 1);

    luaL_newlib(state, functions);

    push_constant(state, "WHITE",   Color3::WHITE);
    push_constant(state, "BLACK",   Color3::BLACK);
    push_constant(state, "RED",     Color3::RED);
    push_constant(state, "GREEN",   Color3::GREEN);
    push_constant(state, "BLUE",    Color3::BLUE);
    push_constant(state, "YELLOW",  Color3::YELLOW);
    push_constant(state, "CYAN",    Color3::CYAN);
    push_constant(state, "MAGENTA", Color3::MAGENTA);

    lua_setglobal(state, "Color3");
}