#include "LocalMultiplayerManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "LMVector.h"
#include "MeshRenderer.h"
#include "ParticleSystem.h"
#include "GameplayManager.h"

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

int JuegoDePistolas::LocalMultiplayerManager::getNumPlayersConnected()
{
	int numPlayers = Input::InputManager::GetInstance()->getCurrentlyConnectedControllers().size();

	if (numPlayers > 4)
		numPlayers = 4;
	return numPlayers;
}

void JuegoDePistolas::LocalMultiplayerManager::awake()
{
	_instance = this;

	Scene* activeScene = SceneManager::GetInstance()->getActiveScene();

	if (activeScene == nullptr)
		return;

	// Asignar referencias de jugadores

	if (allPlayers.size() > 3) {
		allPlayers[0].gameObject = activeScene->getObjectByName("cube");
		allPlayers[1].gameObject = activeScene->getObjectByName("Player_2");
		allPlayers[2].gameObject = activeScene->getObjectByName("Player_3");
		allPlayers[3].gameObject = activeScene->getObjectByName("Player_4");
	}

	int currentPlayer = 0;
	for (PlayerData& playerData : allPlayers) {

		// Marcar el id del controlador como invalido
		playerData.controllerId = Input::InputManager::invalidControllerId();

		// Asignar referenciar de la clase PlayerController
		if (playerData.gameObject != nullptr) {

			playerData.playerController = playerData.gameObject->getComponent<PlayerController>();

			if (playerData.playerController != nullptr) {
				playerData.playerController->setPlayerId(currentPlayer);

				// Desactivar el objeto
				playerData.playerController->setPlayerActive(false);
			}
		}

		currentPlayer++;
	}


	GameObject* CharacterSpawnpoint_1 = activeScene->getObjectByName("CharacterSpawnpoint_1");
	GameObject* CharacterSpawnpoint_2 = activeScene->getObjectByName("CharacterSpawnpoint_2");
	GameObject* CharacterSpawnpoint_3 = activeScene->getObjectByName("CharacterSpawnpoint_3");
	GameObject* CharacterSpawnpoint_4 = activeScene->getObjectByName("CharacterSpawnpoint_4");

	if (CharacterSpawnpoint_1 != nullptr && CharacterSpawnpoint_2 != nullptr
		&& CharacterSpawnpoint_3 != nullptr && CharacterSpawnpoint_4 != nullptr) {

		Transform* CharacterSpawnpoint_1_transform = CharacterSpawnpoint_1->getComponent<Transform>();
		Transform* CharacterSpawnpoint_2_transform = CharacterSpawnpoint_2->getComponent<Transform>();
		Transform* CharacterSpawnpoint_3_transform = CharacterSpawnpoint_3->getComponent<Transform>();
		Transform* CharacterSpawnpoint_4_transform = CharacterSpawnpoint_4->getComponent<Transform>();

		if (CharacterSpawnpoint_1_transform != nullptr && CharacterSpawnpoint_2_transform != nullptr
			&& CharacterSpawnpoint_3_transform != nullptr && CharacterSpawnpoint_4_transform != nullptr) {

			spawnPoints = {
				CharacterSpawnpoint_1_transform->getPosition(),
				CharacterSpawnpoint_2_transform->getPosition(),
				CharacterSpawnpoint_3_transform->getPosition(),
				CharacterSpawnpoint_4_transform->getPosition(),
			};
		}
	}

	//  Mostrar los players de los mandos ya conectados al entrar a la escena
	std::list<LocoMotor::Input::InputManager::ControllerId>controllers = Input::InputManager::GetInstance()->getCurrentlyConnectedControllers();
	int it = 0;
	for (auto contrID : controllers) {

		// No se permitiran mas de 4 mandos conectados al mismo tiempo
		if (it > 4 || allPlayers.size() <= it)
			break;

		allPlayers[it].controllerId = contrID;

		if (allPlayers[it].playerController != nullptr) {
			// Activar el objeto jugador dependiendo del index del jugador
			allPlayers[it].playerController->setPlayerActive(true);
			// Asignar a la clase PlayerController, el controllerId necesario para vincularlo con el mando
			allPlayers[it].playerController->setControllerId(contrID);
		}

		if (allPlayers[it].gameObject != nullptr && allPlayers[it].gameObject->getComponent<Transform>() != nullptr) {

			if (spawnPoints.size() > it)
				allPlayers[it].gameObject->getComponent<Transform>()->setPosition(spawnPoints[it]);
		}


		it++;
	}
}

void LocalMultiplayerManager::start()
{
}

void LocalMultiplayerManager::update(float dT)
{
	// Check connections
	std::list<Input::InputManager::ControllerId> controllersAdded = Input::InputManager::GetInstance()->getOnConnectControllers();
	std::list<Input::InputManager::ControllerId> controllersRemoved = Input::InputManager::GetInstance()->getOnDisconnectControllers();

	// Conexion de usuarios
	for (const Input::InputManager::ControllerId& controllerId : controllersAdded) {

		bool playerAlreadyAdded = false;
		for (auto e : allPlayers) {
			if (e.controllerId == controllerId) {
				playerAlreadyAdded = true;
				break;
			}
		}
		if (playerAlreadyAdded)continue;

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

		if (allPlayers.size() <= playerIndex)
			break;

		// Asignar nuevo controllerId
		allPlayers[playerIndex].controllerId = controllerId;

		// Activar el objeto jugador dependiendo del index del jugador
		if (allPlayers[playerIndex].gameObject != nullptr) {
			allPlayers[playerIndex].gameObject->setActive(true);

			if (allPlayers[playerIndex].gameObject->getComponent<MeshRenderer>() != nullptr)
				allPlayers[playerIndex].gameObject->getComponent<MeshRenderer>()->setEnabled(true);

			if (allPlayers[playerIndex].gameObject->getComponent<ParticleSystem>() != nullptr)
				allPlayers[playerIndex].gameObject->getComponent<ParticleSystem>()->setEnabled(true);

			if (allPlayers[playerIndex].gameObject->getComponent<Transform>() != nullptr && spawnPoints.size() > playerIndex)
				allPlayers[playerIndex].gameObject->getComponent<Transform>()->setPosition(spawnPoints[playerIndex]);
		}

		// Asignar a la clase PlayerController, el controllerId necesario para vincularlo con el mando
		if (allPlayers[playerIndex].playerController != nullptr)
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

		if (allPlayers.size() <= playerIndex)
			break;

		// Invalidar el controllerId
		allPlayers[playerIndex].controllerId = Input::InputManager::invalidControllerId();

		// Desactivar el objeto jugador dependiendo del index del jugador
		if (allPlayers[playerIndex].gameObject != nullptr) {
			allPlayers[playerIndex].gameObject->setActive(false);

			if (allPlayers[playerIndex].gameObject->getComponent<MeshRenderer>() != nullptr)
				allPlayers[playerIndex].gameObject->getComponent<MeshRenderer>()->setEnabled(false);
		}

		// Borrar la referencia a la clase PlayerController
		if (allPlayers[playerIndex].playerController != nullptr)
			allPlayers[playerIndex].playerController->setControllerId(Input::InputManager::invalidControllerId());
	}
}

void LocalMultiplayerManager::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

