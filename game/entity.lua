-- Wraper for entity id. For easier entity control.
-- I will change this to userdata. Later.
local Entity = {
}

function Entity.new(args)
    return Entity.fromid(entity_newEntity(), args)
end

--[[ args are:
{
    is_ref = bool, -- If true, when __gc called do not delete entity and ignore all next arguments
    x, y, width, height = float, -- Rect for entity
    texture = string, -- Texture name
    animations = AnimationsMap, -- Table with entity's animations
}
--]]

function Entity.fromid(id, args)
    local entity = {
        id = id,
        mirrored = false,
        is_ref = args.is_ref,
    }
    
    setmetatable(entity, {__index = Entity})
    
    args = args or {}
    args.x = args.x or 0
    args.y = args.y or 0
    args.width = args.width or 32
    args.height = args.height or 32
    
    if not args.is_ref then
        entity:setPosition(args.x, args.y)
        
        entity:setSize(args.width, args.height)
        
        if args.texture ~= nil then
            entity:setTexture(args.texture)
        end
        
        if args.animations ~= nil then
            entity:setAnimations(args.animations)
        end
    end
    
    return entity
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

function Entity:createRef()
    return Entity.fromid(self.id, {is_ref = true})
end

function Entity:exists()
    return entity_exists(self.id)
end

function Entity:__gc()
    if not self.is_ref then
        entity_delEntity(self.id)
    end
end

return Entity
