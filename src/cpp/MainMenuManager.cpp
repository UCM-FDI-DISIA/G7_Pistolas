#include "MainMenuManager.h"
#include "InputManager.h"
#include "LMInputs.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "MeshRenderer.h"

using namespace LocoMotor;

JuegoDePistolas::MainMenuManager::MainMenuManager()
{
}

JuegoDePistolas::MainMenuManager::~MainMenuManager()
{
}

void JuegoDePistolas::MainMenuManager::start()
{
	players[0].gameObject = SceneManager::GetInstance()->getActiveScene()->getObjectByName("Player_01");
	players[1].gameObject = SceneManager::GetInstance()->getActiveScene()->getObjectByName("Player_02");
	players[2].gameObject = SceneManager::GetInstance()->getActiveScene()->getObjectByName("Player_03");
	players[3].gameObject = SceneManager::GetInstance()->getActiveScene()->getObjectByName("Player_04");

	for (PlayerData& playerData : players) {
		playerData.controllerId = Input::InputManager::invalidControllerId();
		if (playerData.gameObject->getComponent<MeshRenderer>() != nullptr)
			playerData.gameObject->getComponent<MeshRenderer>()->setEnabled(false);
	}

	//  Mostrar los players de los mandos ya conectados al entrar a la escena
	std::list<LocoMotor::Input::InputManager::ControllerId>controllers = Input::InputManager::GetInstance()->getCurrentlyConnectedControllers();
	int it = 0;
	for (auto contrID : controllers) {
		// No se permitiran mas de 4 mandos conectados al mismo tiempo
		if (it > 4)
			break;
		players[it].controllerId = contrID;
		if (players[it].gameObject->getComponent<MeshRenderer>() != nullptr)
			players[it].gameObject->getComponent<MeshRenderer>()->setEnabled(true);
		it++;
	}
}

void JuegoDePistolas::MainMenuManager::update(float dT)
{
	std::list<Input::InputManager::ControllerId> controllersAdded = Input::InputManager::GetInstance()->getOnConnectControllers();
	std::list<Input::InputManager::ControllerId> controllersRemoved = Input::InputManager::GetInstance()->getOnDisconnectControllers();

	// Mostrar los players de los mandos conectados
	for (auto contrID : controllersAdded) {
		int freePlayer;
		for (freePlayer = 0; freePlayer < players.size(); freePlayer++) {
			// Si la conexion del mando ya se habia detectado en el start la ignoramos
			if (players[freePlayer].controllerId == contrID) {
				freePlayer = -1;
				break;
			}
			if (players[freePlayer].controllerId == Input::InputManager::invalidControllerId())
				break;
		}
		if (freePlayer == -1)continue;
		players[freePlayer].controllerId = contrID;
		if (players[freePlayer].gameObject->getComponent<MeshRenderer>() != nullptr)
			players[freePlayer].gameObject->getComponent<MeshRenderer>()->setEnabled(true);
	}
	// Ocultar los players de los mandos desconectados
	for (auto contrID : controllersRemoved) {
		int playerRemoved;
		for (playerRemoved = 0; playerRemoved < players.size(); playerRemoved++) {
			if (players[playerRemoved].controllerId == contrID)
				break;
		}
		players[playerRemoved].controllerId = Input::InputManager::invalidControllerId();
		if (players[playerRemoved].gameObject->getComponent<MeshRenderer>() != nullptr)
			players[playerRemoved].gameObject->getComponent<MeshRenderer>()->setEnabled(false);
	}
	// Comprobar el cambio de escena
	bool changeScene = false;
	if (Input::InputManager::GetInstance()->GetKeyDown(Input::LMKS_SPACE)) {
		changeScene = true;
	}
	std::list<LocoMotor::Input::InputManager::ControllerId>controllers = Input::InputManager::GetInstance()->getCurrentlyConnectedControllers();
	for (auto contrID : controllers) {
		if (Input::InputManager::GetInstance()->GetButtonDown(contrID, Input::LMC_A) ||
			Input::InputManager::GetInstance()->GetButtonDown(contrID, Input::LMC_B) ||
			Input::InputManager::GetInstance()->GetButtonDown(contrID, Input::LMC_X) ||
			Input::InputManager::GetInstance()->GetButtonDown(contrID, Input::LMC_Y)) {
			changeScene = true;
			break;
		}
	}
	if (changeScene) {
		SceneManager::GetInstance()->loadScene("Assets/Scenes/Scene.lua", "Scene");
		SceneManager::GetInstance()->changeScene("Scene");
	}
}

void JuegoDePistolas::MainMenuManager::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}
