// dllmain.cpp : Define el punto de entrada de la aplicación DLL.
#include "pch.h"
#include <iostream>
#include "Engine.h"
#include "SceneManager.h"
#include "ComponentsFactory.h"

// Componentes
#include "PlayerController.h"
#include "LocalMultiplayerManager.h"

using namespace LocoMotor;
using namespace JuegoDePistolas;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) const char* InitJuego(LocoMotor::Engine* motor)
{
    LocoMotor::ComponentsFactory::GetInstance()->registerComponent<PlayerController>("PlayerController");
    LocoMotor::ComponentsFactory::GetInstance()->registerComponent<LocalMultiplayerManager>("LocalMultiplayerManager");

    motor->setWindowName("Juego de pistolas");

    LocoMotor::SceneManager::GetInstance()->loadScene("Assets/Scenes/Scene.lua", "scene");

#ifdef _DEBUG
    return "Juego de pistolas: Running in DEBUG";
#endif // _DEBUG
    return "Juego de pistolas: Running in RELEASE";
}