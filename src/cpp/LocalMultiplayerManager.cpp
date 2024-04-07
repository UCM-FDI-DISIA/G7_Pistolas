#include "LocalMultiplayerManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "LMVector.h"
#include "MeshRenderer.h"

#include <iostream>
#include <SceneManager.h>
#include <Scene.h>

using namespace JuegoDePistolas;
using namespace LocoMotor;

LocalMultiplayerManager::LocalMultiplayerManager()
{

}

LocalMultiplayerManager::~LocalMultiplayerManager()
{
}

void LocalMultiplayerManager::start()
{
	//for (int i = 0; i < allPlayers.size(); i++)
	//	allPlayers[i] = nullptr;
}

void LocalMultiplayerManager::update(float dT)
{

	// Check connections

	std::list<Input::InputManager::ControllerId> controllersAdded = Input::InputManager::GetInstance()->getOnConnectControllers();
	std::list<Input::InputManager::ControllerId> controllersRemoved = Input::InputManager::GetInstance()->getOnDisconnectControllers();

	// Conexion de usuarios
	for (const Input::InputManager::ControllerId& controllerId : controllersAdded) {

		int playerIndex = -1;

		// Calcular el index de jugador que tendra este mando
		for (int i = 0; i < allPlayers.size(); i++)
			if (allPlayers[0].playerController == nullptr) {
				playerIndex = i;
				break;
			}


		//// Crear el objeto jugador dependiendo del index del jugador
		//GameObject* newPlayer = SceneManager::GetInstance()->getActiveScene()->addGameobject("Player_" + playerIndex);
		//Transform* playerTransform = (Transform*)newPlayer->addComponent("Transform");

		//MeshRenderer* playerMeshRenderer = (MeshRenderer*)newPlayer->addComponent("MeshRenderer");

		//playerMeshRenderer->init("cubeMesh", "", false);
		//playerMeshRenderer->setMesh("CubemanMesh.mesh");



		//if (firstController == Input::InputManager::invalidControllerId())
		//	firstController = controllerId;

		//else if (secondController == Input::InputManager::invalidControllerId())
		//	secondController = controllerId;

		//else if (thirdController == Input::InputManager::invalidControllerId())
		//	thirdController = controllerId;
	}

	// Desconexion de usuarios
	for (const Input::InputManager::ControllerId& controllerId : controllersRemoved) {

		//// Si se ha desconectado el primer usuario
		//if (firstController == controllerId)
		//	firstController = Input::InputManager::invalidControllerId();

		//else if (secondController == controllerId)
		//	secondController = Input::InputManager::invalidControllerId();

		//else if (thirdController == controllerId)
		//	thirdController = Input::InputManager::invalidControllerId();
	}


	std::cout << "LocalMultiplayerUpdate" << std::endl;
}

void LocalMultiplayerManager::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

