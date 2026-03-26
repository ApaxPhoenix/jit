-- create an entity
local player = Entity.new("Player")

-- add components
local transform = Transform.new()
transform.position = Vector3.new(0, 10, 0)
transform.rotation = Vector3.new(0, 0, 0)
transform.scale = Vector3.new(1, 1, 1)

player:add(transform)

-- find entity in scene
local found = Scene.find("Player")
print(found.name)

-- get component from entity
local t = found:get(Transform)
print(t.position)

-- remove component
player:remove(Transform)

-- destroy entity
player:destroy()

-- loop all entities
for _, entity in Scene.all() do
    local t = entity:get(Transform)
    if t then
        print(entity.name, t.position)
    end
end

-- events
Scene.OnStep:Connect(function(dt)
    local t = player:get(Transform)
    t.position = t.position + Vector3.new(0, dt, 0)
end)
```

So next files to build:
```
1. Entity       ← name, id, add/get/remove components
2. Transform    ← position, rotation, scale
3. Scene        ← holds all entities, find/all/OnStep
4. Events       ← Connect, Fire, Disconnect