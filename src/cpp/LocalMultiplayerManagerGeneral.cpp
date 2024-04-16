#include "LocalMultiplayerManagerGeneral.h"
#include "GameObject.h"
#include "Transform.h"
#include "LMVector.h"
#include "MeshRenderer.h"

#include <iostream>
#include <SceneManager.h>
#include <Scene.h>

using namespace JuegoDePistolas;
using namespace LocoMotor;

LocalMultiplayerManagerGeneral* LocalMultiplayerManagerGeneral::_instance = nullptr;

LocalMultiplayerManagerGeneral::LocalMultiplayerManagerGeneral()
{ }

LocalMultiplayerManagerGeneral::~LocalMultiplayerManagerGeneral()
{ }

LocalMultiplayerManagerGeneral* LocalMultiplayerManagerGeneral::GetInstance()
{
	return _instance;
}

std::array<Input::InputManager::ControllerId, 4> LocalMultiplayerManagerGeneral::getPlayers()
{
	if (this == nullptr)
		return std::array<Input::InputManager::ControllerId, 4>();
	else
		return allPlayers;
}

void LocalMultiplayerManagerGeneral::init()
{
	_instance = this;

	// Inicializar valores
	int currentPlayer = 0;
	for (Input::InputManager::ControllerId& controllerId : allPlayers) {

		// Marcar el id del controlador como invalido
		controllerId = Input::InputManager::invalidControllerId();
		currentPlayer++;
	}

	//// Suscribirse al InputManager para saber cuando se ha conectado/desconectado un mando
	//Input::InputManager::GetInstance()->addListener([this]() {
	//	this->onControllersChange();
	//});
}

void LocalMultiplayerManagerGeneral::onControllersChange()
{

}
