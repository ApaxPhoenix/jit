#include "bindings.hpp"
#include "color4.hpp"
#include <cassert>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

static Color4 *check(lua_State *state, const int index) {
    return static_cast<Color4 *>(luaL_checkudata(state, index, "Color4"));
}

// Color4.new(r, g, b, a)
// Color4.new() -- defaults to (0, 0, 0, 1)
static int new_(lua_State *state) {
    const auto r = static_cast<float>(luaL_optnumber(state, 1, 0.0));
    const auto g = static_cast<float>(luaL_optnumber(state, 2, 0.0));
    const auto b = static_cast<float>(luaL_optnumber(state, 3, 0.0));
    const auto a = static_cast<float>(luaL_optnumber(state, 4, 1.0));
    auto *color = static_cast<Color4 *>(lua_newuserdata(state, sizeof(Color4)));
    *color = Color4(r, g, b, a);
    luaL_setmetatable(state, "Color4");
    return 1;
}

// Color4.rgb(r, g, b, a) -- 0-255 range
static int rgb(lua_State *state) {
    const auto r = static_cast<uint8_t>(luaL_checkinteger(state, 1));
    const auto g = static_cast<uint8_t>(luaL_checkinteger(state, 2));
    const auto b = static_cast<uint8_t>(luaL_checkinteger(state, 3));
    const auto a = static_cast<uint8_t>(luaL_optinteger(state, 4, 255));
    auto *color = static_cast<Color4 *>(lua_newuserdata(state, sizeof(Color4)));
    *color = Color4::rgb(r, g, b, a);
    luaL_setmetatable(state, "Color4");
    return 1;
}

// Color4.hsv(h, s, v, a) -- h: [0, 360), s: [0, 1], v: [0, 1], a: [0, 1]
static int hsv(lua_State *state) {
    const auto h = static_cast<float>(luaL_checknumber(state, 1));
    const auto s = static_cast<float>(luaL_checknumber(state, 2));
    const auto v = static_cast<float>(luaL_checknumber(state, 3));
    const auto a = static_cast<float>(luaL_optnumber(state, 4, 1.0));
    auto *color = static_cast<Color4 *>(lua_newuserdata(state, sizeof(Color4)));
    *color = Color4::hsv(h, s, v, a);
    luaL_setmetatable(state, "Color4");
    return 1;
}

// Color4.fromHex(hex) -- e.g. 0xFF8800FF
static int hex(lua_State *state) {
    const auto hex = static_cast<unsigned int>(luaL_checkinteger(state, 1));
    auto *color = static_cast<Color4 *>(lua_newuserdata(state, sizeof(Color4)));
    *color = Color4::hex(hex);
    luaL_setmetatable(state, "Color4");
    return 1;
}

// __index: color.red, color.green, color.blue, color.alpha, color:method()
static int index(lua_State *state) {
    const Color4 *color = check(state, 1);
    const char *key = luaL_checkstring(state, 2);

    if (strcmp(key, "red")   == 0) { lua_pushnumber(state, color->red); }
    if (strcmp(key, "green") == 0) { lua_pushnumber(state, color->green); }
    if (strcmp(key, "blue")  == 0) { lua_pushnumber(state, color->blue); }
    if (strcmp(key, "alpha") == 0) { lua_pushnumber(state, color->alpha); }

    // fall back to metatable for methods
    luaL_getmetatable(state, "Color4");
    lua_pushvalue(state, 2);
    lua_rawget(state, -2);
    return 1;
}

// __newindex: color.red = 1.0 etc
static int newindex(lua_State *state) {
    Color4 *color = check(state, 1);
    const char *key = luaL_checkstring(state, 2);
    const auto value = static_cast<float>(luaL_checknumber(state, 3));

    if (strcmp(key, "red")   == 0) color->red   = value;
    else if (strcmp(key, "green") == 0) color->green = value;
    else if (strcmp(key, "blue")  == 0) color->blue  = value;
    else if (strcmp(key, "alpha") == 0) color->alpha = value;
    else luaL_error(state, "Color4: unknown field '%s'", key);
    return 0;
}

static int add(lua_State *state) {
    const Color4 *alpha = check(state, 1);
    const Color4 *beta  = check(state, 2);
    auto *result = static_cast<Color4 *>(lua_newuserdata(state, sizeof(Color4)));
    *result = *alpha + *beta;
    luaL_setmetatable(state, "Color4");
    return 1;
}

static int sub(lua_State *state) {
    const Color4 *alpha = check(state, 1);
    const Color4 *beta  = check(state, 2);
    auto *result = static_cast<Color4 *>(lua_newuserdata(state, sizeof(Color4)));
    *result = *alpha - *beta;
    luaL_setmetatable(state, "Color4");
    return 1;
}

static int mul(lua_State *state) {
    auto *result = static_cast<Color4 *>(lua_newuserdata(state, sizeof(Color4)));
    if (lua_isnumber(state, 1)) {
        const auto scalar   = static_cast<float>(lua_tonumber(state, 1));
        const Color4 *color = check(state, 2);
        *result = *color * scalar;
    } else if (lua_isnumber(state, 2)) {
        const Color4 *color = check(state, 1);
        const auto scalar   = static_cast<float>(lua_tonumber(state, 2));
        *result = *color * scalar;
    } else {
        const Color4 *alpha = check(state, 1);
        const Color4 *beta  = check(state, 2);
        *result = *alpha * *beta;
    }
    luaL_setmetatable(state, "Color4");
    return 1;
}

static int div(lua_State *state) {
    const Color4 *color = check(state, 1);
    const auto scalar   = static_cast<float>(luaL_checknumber(state, 2));
    auto *result = static_cast<Color4 *>(lua_newuserdata(state, sizeof(Color4)));
    *result = *color / scalar;
    luaL_setmetatable(state, "Color4");
    return 1;
}

static int eq(lua_State *state) {
    const Color4 *alpha = check(state, 1);
    const Color4 *beta  = check(state, 2);
    lua_pushboolean(state, *alpha == *beta);
    return 1;
}

static int tostring(lua_State *state) {
    const Color4 *color = check(state, 1);
    lua_pushfstring(state, "Color4(%f, %f, %f, %f)", color->red, color->green, color->blue, color->alpha);
    return 1;
}

static int inverted(lua_State *state) {
    const Color4 *color = check(state, 1);
    auto *result = static_cast<Color4 *>(lua_newuserdata(state, sizeof(Color4)));
    *result = color->inverted();
    luaL_setmetatable(state, "Color4");
    return 1;
}

static int clamped(lua_State *state) {
    const Color4 *color = check(state, 1);
    auto *result = static_cast<Color4 *>(lua_newuserdata(state, sizeof(Color4)));
    *result = color->clamped();
    luaL_setmetatable(state, "Color4");
    return 1;
}

static int interpolate(lua_State *state) {
    const Color4 *alpha = check(state, 1);
    const Color4 *beta  = check(state, 2);
    const auto factor  = static_cast<float>(luaL_checknumber(state, 3));
    auto *result      = static_cast<Color4 *>(lua_newuserdata(state, sizeof(Color4)));
    *result = alpha->interpolate(*beta, factor);
    luaL_setmetatable(state, "Color4");
    return 1;
}

static int gamma(lua_State *state) {
    Color4 *color        = check(state, 1);
    const auto exponent = static_cast<float>(luaL_checknumber(state, 2));
    auto *result       = static_cast<Color4 *>(lua_newuserdata(state, sizeof(Color4)));
    *result = color->gamma(exponent);
    luaL_setmetatable(state, "Color4");
    return 1;
}

static int premultiplied(lua_State *state) {
    const Color4 *color = check(state, 1);
    auto *result      = static_cast<Color4 *>(lua_newuserdata(state, sizeof(Color4)));
    *result = color->premultiplied();
    luaL_setmetatable(state, "Color4");
    return 1;
}

static int luminance(lua_State *state) {
    Color4 *color = check(state, 1);
    lua_pushnumber(state, static_cast<float>(color->luminance()));
    return 1;
}

static int difference(lua_State *state) {
    Color4 *alpha = check(state, 1);
    Color4 *beta  = check(state, 2);
    lua_pushnumber(state, static_cast<float>(alpha->difference(*beta)));
    return 1;
}

static int approximately(lua_State *state) {
    Color4 *alpha       = check(state, 1);
    Color4 *beta        = check(state, 2);
    const auto epsilon = static_cast<float>(luaL_optnumber(state, 3, 1e-5));
    lua_pushboolean(state, alpha->approximately(*beta, epsilon));
    return 1;
}

// push a Color4 constant as userdata into the table on top of the stack
static void push_constant(lua_State *state, const char *name, const Color4 &value) {
    auto *color = static_cast<Color4 *>(lua_newuserdata(state, sizeof(Color4)));
    *color        = value;
    luaL_setmetatable(state, "Color4");
    lua_setfield(state, -2, name);
}

static const luaL_Reg methods[] = {
    { "inverted",      inverted      },
    { "clamped",       clamped       },
    { "interpolate",   interpolate   },
    { "gamma",         gamma         },
    { "premultiplied", premultiplied },
    { "luminance",     luminance     },
    { "difference",    difference    },
    { "approximately", approximately },
    { nullptr, nullptr }
};

static const luaL_Reg functions[] = {
    { "new",     new_ },
    { "rgb",     rgb  },
    { "hsv",     hsv  },
    { "hex", hex  },
    { nullptr, nullptr }
};

void register_color4(lua_State *state) {
    luaL_newmetatable(state, "Color4");

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

    push_constant(state, "WHITE",       Color4::WHITE);
    push_constant(state, "BLACK",       Color4::BLACK);
    push_constant(state, "RED",         Color4::RED);
    push_constant(state, "GREEN",       Color4::GREEN);
    push_constant(state, "BLUE",        Color4::BLUE);
    push_constant(state, "YELLOW",      Color4::YELLOW);
    push_constant(state, "CYAN",        Color4::CYAN);
    push_constant(state, "MAGENTA",     Color4::MAGENTA);
    push_constant(state, "TRANSPARENT", Color4::TRANSPARENT);

    lua_setglobal(state, "Color4");
}