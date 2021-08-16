local rhooks = {}

-- Simple function to add hooks easily
function addHook(name, func)
    if hooks[name] == nil then error("no such hook: \""..name.."\"") end

    hooks[name][#hooks[name]+1] = func
end

-- Add hook that can be replaced by replaceHook
function addReplacableHook(name, func, hookname)
    if rhooks[hookname] ~= nil then error("replacable hook \""..name.."\" already exists. Use replaceHook to replace it") end

    local hook = {func = func}
    setmetatable(hook, {
        __call = function(self, ...)
            return self.func(...)
        end
    })
    
    rhooks[hookname] = hook
    
    addHook(name, hook)
end

-- Replace hook and return old one
function replaceHook(hookname, func)
    if rhooks[hookname] == nil then error("no such replacable hook: \""..hookname.."\"") end
    
    local old = rhooks[hookname].func
    rhooks[hookname].func = func
    return old
end
