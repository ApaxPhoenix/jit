extern "C" {
#include "sandbox.h"
#include "bridge.h"
#include "platform.h"
#include <lua.h>
#include <lauxlib.h>
}

#include "bindings/vector3.hpp"
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
    register_vector3(sandbox->state);
}

int main() {
    platform_sandbox();

    Sandbox *sandbox = sandbox_create();
    if (sandbox == nullptr) return 1;

    bridge_register(sandbox);

    sandbox_run(sandbox, R"(
        local pos = Vector3.new(0, 10, 0)
        print(pos)
        print(pos.x)
        print(pos.y)
        print(pos.z)

        local a = Vector3.new(1, 2, 3)
        local b = Vector3.new(4, 5, 6)

        print(a + b)
        print(a - b)
        print(a * 2)
        print(a / 2)
        print(-a)

        print(a == b)
        print(a ~= b)

        print(a:length())
        print(a:normalize())
        print(a:distance(b))
        print(a:dot(b))
        print(a:cross(b))
        print(a:lerp(b, 0.5))
        print(a:angle(b))
        print(a:abs())
        print(a:clamp(Vector3.zero(), Vector3.one()))

        print(Vector3.zero())
        print(Vector3.one())
        print(Vector3.up())
        print(Vector3.down())
        print(Vector3.left())
        print(Vector3.right())
        print(Vector3.forward())
        print(Vector3.back())
    )");

    sandbox_destroy(sandbox);
    return 0;
}
