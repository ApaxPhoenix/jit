extern "C" {
#include "sandbox.h"
#include "bridge.h"
#include "platform.h"
#include <lua.h>
#include <lauxlib.h>
}
#include "../include/bindings.hpp"
#include <cstdio>

static int print_func(lua_State *state) {
    const int args = lua_gettop(state);
    for (int i = 1; i <= args; i++) {
        if (lua_isboolean(state, i)) {
            fprintf(stdout, "%s", lua_toboolean(state, i) ? "true" : "false");
        } else if (luaL_callmeta(state, i, "__tostring")) {
            fprintf(stdout, "%s", lua_tostring(state, -1));
            lua_pop(state, 1);
        } else {
            fprintf(stdout, "%s", lua_tostring(state, i));
        }
        if (i < args) fprintf(stdout, "\t");
    }
    fprintf(stdout, "\n");
    return 0;
}

extern "C" void bridge_register(const Sandbox *sandbox) {
    lua_register(sandbox->state, "print", print_func);
    register_vector2(sandbox->state);
    register_vector3(sandbox->state);
    register_quaternion(sandbox->state);
    register_matrix3(sandbox->state);
    register_matrix4(sandbox->state);
    register_transform(sandbox->state);
    register_color3(sandbox->state);
    register_color4(sandbox->state);
}

int main() {
    platform_sandbox();

    Sandbox *sandbox = sandbox_create();
    if (sandbox == nullptr) return 1;

    bridge_register(sandbox);

    sandbox_run(sandbox, "tests/main.lua");

    sandbox_destroy(sandbox);
    return 0;
}