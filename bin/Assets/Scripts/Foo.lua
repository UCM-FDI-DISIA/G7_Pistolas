Foo = {behaviour = behaviour}
Foo.__index = Foo

function Foo:start()
    print("Hola Foo")
    behaviour:gameObject():getBehaviour("Prueba"):getScript():foo()
end