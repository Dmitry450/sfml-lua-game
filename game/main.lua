dofile("builtin.lua")

local ET_MYENTITY = Entity.newType({
    width = 32, height = 32,
    texture = "resources/char.png",
    animations = {
        walk = {
            time_per_frame = 0.15,
            frames = 3,
            x = 0,
            y = 0,
            width = 32,
            height = 32,
        },
    }
})


local myentity = Entity.new({
    x = 40, y = 40,
    type = ET_MYENTITY,
})


local player = Player.new()

player:addAnimation("stand", {
    time_per_frame = 999,
    frames = 1,
    x = 0,
    y = 0,
    width = 32,
    height = 32,
})

local x = 0

addHook("update", function(dtime)
    local collide = myentity:getCollisionInfo()
    
    if not collide.blockd then
        local v = myentity:getVelocity()
        myentity:setVelocity(0, v.y + 32*10 * dtime)
    end
    
    local v = player:getVelocity()
    
    if v.x == 0 then
        player:setAnimation("stand", true)
    end
    
    local player_mouse = player_getMouse()
    
    if player_mouse.lmb then
        myentity:setPosition(player_mouse.x - 16, player_mouse.y - 16)
    end
    
    x = x + 1
    
    if x % 60 == 0 then
        myentity:mirror()
    end
    
    if myentity:isCollide(player) then
        local v = myentity:getVelocity()
        myentity:setVelocity(0, v.y - 32*20 * dtime)
    end
end)
