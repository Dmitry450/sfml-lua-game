-- Wraper for entity id. For easier entity control.
-- I will change this to userdata. Later.
local Entity = {
    types = {}, -- Map for pairs 'entity type : init arguments'
    entities = {}, -- Weak table for all created entities
}

setmetatable(Entity.entities, {__mode = 'v'})

local types = 0

-- Create new type of entity. If initargs is not nil, it will be used as args
-- every time you call Entity.new with args "type" key.
-- Return
function Entity.newType(initargs)
    types = types + 1
    Entity.types[types] = initargs
    return types
end

--[[ args are:
{
    type = int, -- If given, missing arguments would be replaced by values from Entity.types[type]
    x, y, width, height = float, -- Rect for entity
    texture = string, -- Texture name
    animations = AnimationsMap, -- Table with entity's animations
}
--]]
function Entity.new(args)
    if args ~= nil and args.type ~= nil and Entity.types[args.type] ~= nil then
        local nargs = Entity.types[args.type] -- New args
        
        args.x = args.x or nargs.x
        args.y = args.y or nargs.y
        args.width = args.width or nargs.width
        args.height = args.height or nargs.height
        args.texture = args.texture or nargs.texture
        args.animations = args.animations or nargs.animations
    end

    local entity = Entity.fromid(entity_newEntity(), args)
    
    Entity.entities[entity.id] = entity
    
    return entity
end

function Entity.fromid(id, args)
    if Entity.entities[id] ~= nil then return Entity.entities[id] end

    args = args or {}

    local entity = {
        id = id,
        mirrored = false,
        type = args.type or 0
    }
    
    entity = setmetatable(entity, {__index = Entity, __gc = Entity.kill})
    
    if not args.keep_attributes then
        if args.x ~= nil and args.y ~= nil then
            entity:setPosition(args.x, args.y)
        end
        
        if args.width ~= nil and args.height ~= nil then
            entity:setSize(args.width, args.height)
        end
        
        if args.texture ~= nil then
            entity:setTexture(args.texture)
        end
        
        if args.animations ~= nil then
            entity:setAnimations(args.animations)
        end
    end
    
    return entity
end

function Entity.findByType(type)
    local found = {}
    
    for _, entity in ipairs(Entity.entities) do
        if entity.type == type then
            found[#found+1] = entity
        end
    end
    
    return found
end

function Entity:setTexture(name)
    entity_setTexture(self.id, name)
end

function Entity:setAnimations(animations)
    entity_setAnimationsManager(self.id, animations)
end

function Entity:setAnimation(name, ignore_mirrored)
    if ignore_mirrored then
        entity_setAnimation(self.id, name)
    else
        entity_setAnimation(self.id, name, self.mirrored)
    end
end

function Entity:addAnimation(name, animation)
    entity_addAnimation(self.id, name, animation)
end

function Entity:mirror(mirror)
    if mirror == nil then
        self.mirrored = not self.mirrored
        entity_mirror(self.id)
    else
        self.mirrored = mirror
        entity_mirror(self.id, mirror)
    end
end

function Entity:getCollisionInfo()
    return entity_getCollisionInfo(self.id)
end

function Entity:setPosition(x, y)
    entity_setPosition(self.id, x, y)
end

function Entity:getPosition()
    return entity_getPosition(self.id)
end

function Entity:setVelocity(x, y)
    entity_setVelocity(self.id, x, y)
end

function Entity:getVelocity()
    return entity_getVelocity(self.id)
end

function Entity:setSize(width, height)
    entity_setSize(self.id, width, height)
end

function Entity:getSize()
    return entity_getSize(self.id)
end

function Entity:exists()
    return entity_exists(self.id)
end

function Entity:kill()
    entity_delEntity(self.id)
    self.id = 0
end

return Entity
