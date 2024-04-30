
Prueba = {behaviour = behaviour}
Prueba.__index = Prueba

function Prueba:new()
    local obj = {}
    setmetatable(obj,Prueba);
    return obj;
end
function Prueba:update(dt)
    ---print(dt)
    ---print("Hola Buenas desde el update")
    
end

function Prueba:start()
    ---self:foo()
    v = Vector3.new(0,0,0)
    ---print("Hola Buenas desde el update")
end

function Prueba:setParameters(params) 
    print("SetParameters")
    for n,p in pairs(params) do
        print(n)
        print(p)
    end 
    print(params["parametro"])
end

function Prueba:foo()
    print("Funcion desde lua")
end