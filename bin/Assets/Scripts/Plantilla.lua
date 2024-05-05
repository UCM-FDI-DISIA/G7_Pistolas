Plantilla = {behaviour = behaviour}
Plantilla.__index = Plantilla

function Plantilla:awake()
end
function Plantilla:update(dt)
end

function Plantilla:start()
end

function Plantilla:setParameters(params) 
    print("SetParameters")
    for name, param in pairs(params) do
        print(name)
        print(param)
    end 
    print(params["parametro"])
end

