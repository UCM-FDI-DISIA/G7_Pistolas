@echo off

set MotorBin=.\LocoMotor\bin
set Bin=.\bin

REM Un prompt para preguntar si queremos limpiar todas las dependencias del motor que haya generadas
SET LIMPIAR=n
setlocal
:PROMPT
SET /P LIMPIAR=Quieres limpiar todos los archivos generables de las dependencias y crearlos de cero (s/[n])? 

REM Si no dice que si, nos saltamos el limpiado (:SALTARSECLEAN, linea 19)
IF /I NOT "%LIMPIAR%" == "s" GOTO SALTARSECLEAN

cd LocoMotor
call .\LimpiarTodo.bat
cd ..

:SALTARSECLEAN

REM Borramos los archivos generables de bin siempre
cd bin
del /s *.dll
del /s *.exe
cd ..

cd LocoMotor

call .\CompilarLocoMotor.bat

cd ..

msbuild G7_JuegoDePistolas.sln /p:configuration=Debug
msbuild G7_JuegoDePistolas.sln /p:configuration=Release

xcopy %MotorBin% %Bin% /h /i /c /k /e /r /y

cd bin

start LocoMotor_Main.exe

cd ..
cd ..

@echo on
