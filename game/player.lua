-- Make it local
local Entity = Entity

if Entity == nil then
    Entity = require("entity.lua")
end

local Player = {}
setmetatable(Player, {__index = Entity})

function Player.new()
    local player = Entity.fromid(player_getEntity(), {is_ref = true})
    setmetatable(player, {__index = Player})
    
    return player
end

-- Not clear, but i have no better idea
Player.fromid = function() return Player.new() end

-- For now these function are useless wrappers,
-- but they will be more useful when i add multiplayer (if i will)
function Player:getKeys()
    return player_getKeys()
end

function Player:getMouse()
    return player_getMouse()
end

return Player
