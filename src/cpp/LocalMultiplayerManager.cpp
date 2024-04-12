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

LocalMultiplayerManager* LocalMultiplayerManager::_instance = nullptr;

LocalMultiplayerManager::LocalMultiplayerManager()
{

}

LocalMultiplayerManager::~LocalMultiplayerManager()
{
}

LocalMultiplayerManager* LocalMultiplayerManager::GetInstance()
{
	return _instance;
}

std::array<LocalMultiplayerManager::PlayerData, 4> LocalMultiplayerManager::getPlayers()
{
	if (this == nullptr)
		return std::array<LocalMultiplayerManager::PlayerData, 4>();
	else
		return allPlayers;
}

void LocalMultiplayerManager::start()
{
	_instance = this;

	int distance = 80;

	// Definir spawn points para los jugadores
	spawnPoints = spawnPoints = {
	LMVector3{80, 40, 80},
	LMVector3{-80, 40, 80},
	LMVector3{80, 40, -80},
	LMVector3{-80, 40, -80}
	};

	// Asignar referencias de jugadores

	allPlayers[0].gameObject = SceneManager::GetInstance()->getActiveScene()->getObjectByName("cube");
	allPlayers[1].gameObject = SceneManager::GetInstance()->getActiveScene()->getObjectByName("Player_2");
	allPlayers[2].gameObject = SceneManager::GetInstance()->getActiveScene()->getObjectByName("Player_3");
	allPlayers[3].gameObject = SceneManager::GetInstance()->getActiveScene()->getObjectByName("Player_4");

	int currentPlayer = 0;
	for (PlayerData& playerData : allPlayers) {

		// Marcar el id del controlador como invalido
		playerData.controllerId = Input::InputManager::invalidControllerId();

		// Asignar referenciar de la clase PlayerController
		playerData.playerController = playerData.gameObject->getComponent<PlayerController>();
		playerData.playerController->setPlayerId(currentPlayer);

		// Desactivar el objeto
		playerData.gameObject->setActive(false);
		playerData.gameObject->getComponent<MeshRenderer>()->setEnabled(false);

		currentPlayer++;
	}

	Transform* tr = allPlayers[3].gameObject->getComponent<Transform>();
	tr->SetPosition(tr->GetPosition() + LMVector3(0, 50, 0));
}

void LocalMultiplayerManager::update(float dT)
{
	// Check connections
	std::list<Input::InputManager::ControllerId> controllersAdded = Input::InputManager::GetInstance()->getOnConnectControllers();
	std::list<Input::InputManager::ControllerId> controllersRemoved = Input::InputManager::GetInstance()->getOnDisconnectControllers();

	// Conexion de usuarios
	for (const Input::InputManager::ControllerId& controllerId : controllersAdded) {

		std::cout << "controllerAdded" << std::endl;

		int playerIndex = -1;

		// Calcular el index de jugador que tendra este mando
		for (int i = 0; i < allPlayers.size(); i++)
			if (allPlayers[i].controllerId == Input::InputManager::invalidControllerId()) {
				playerIndex = i;
				break;
			}

		std::cout << "playerIndex" << playerIndex << std::endl;

		// Comprobar si el index es valido
		// No se permitiran mas de 4 mandos conectados al mismo tiempo
		if (playerIndex < 0 || playerIndex > 4)
			break;

		// Asignar nuevo controllerId
		allPlayers[playerIndex].controllerId = controllerId;

		// Activar el objeto jugador dependiendo del index del jugador
		allPlayers[playerIndex].gameObject->setActive(true);
		allPlayers[playerIndex].gameObject->getComponent<MeshRenderer>()->setEnabled(true);

		allPlayers[playerIndex].gameObject->getComponent<Transform>()->SetPosition(spawnPoints[playerIndex]);

		// Asignar a la clase PlayerController, el controllerId necesario para vincularlo con el mando
		allPlayers[playerIndex].playerController->setControllerId(controllerId);
	}

	// Desconexion de usuarios
	for (const Input::InputManager::ControllerId& controllerId : controllersRemoved) {

		int playerIndex = -1;

		// Calcular el index de jugador que ha dejado este mando
		for (int i = 0; i < allPlayers.size(); i++)
			if (allPlayers[i].controllerId == controllerId) {
				playerIndex = i;
				break;
			}

		std::cout << "playerIndex" << playerIndex << std::endl;

		// Comprobar si el playerIndex es valido
		if (playerIndex < 0 || playerIndex > 4)
			break;

		// Invalidar el controllerId
		allPlayers[playerIndex].controllerId = Input::InputManager::invalidControllerId();

		// Desactivar el objeto jugador dependiendo del index del jugador
		allPlayers[playerIndex].gameObject->setActive(false);
		allPlayers[playerIndex].gameObject->getComponent<MeshRenderer>()->setEnabled(false);

		// Borrar la referencia a la clase PlayerController
		allPlayers[playerIndex].playerController->setControllerId(Input::InputManager::invalidControllerId());
	}
}

void LocalMultiplayerManager::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

