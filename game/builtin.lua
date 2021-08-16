-- To add hooks easily
function addHook(name, func)
    hooks[name][#hooks[name]+1] = func
end

Entity = require("entity")
Player = require("player")
