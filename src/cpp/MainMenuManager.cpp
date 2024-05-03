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
	GameObject* playTextObj = SceneManager::GetInstance()->getActiveScene()->getObjectByName("PlayText");
	if (playTextObj != nullptr)
		playText = playTextObj->getComponent<UIText>();

	players[0].gameObject = SceneManager::GetInstance()->getActiveScene()->getObjectByName("Player_01");
	players[1].gameObject = SceneManager::GetInstance()->getActiveScene()->getObjectByName("Player_02");
	players[2].gameObject = SceneManager::GetInstance()->getActiveScene()->getObjectByName("Player_03");
	players[3].gameObject = SceneManager::GetInstance()->getActiveScene()->getObjectByName("Player_04");

	MeshRenderer* playerMesh_1 = players[0].gameObject->getComponent<MeshRenderer>();
	if (playerMesh_1 != nullptr)
		playerMesh_1->playAnimation("Idle", true);
	MeshRenderer* playerMesh_2 = players[0].gameObject->getComponent<MeshRenderer>();
	if (playerMesh_2 != nullptr)
		playerMesh_2->playAnimation("Idle", true);
	MeshRenderer* playerMesh_3 = players[0].gameObject->getComponent<MeshRenderer>();
	if (playerMesh_3 != nullptr)
		playerMesh_3->playAnimation("Idle", true);
	MeshRenderer* playerMesh_4 = players[0].gameObject->getComponent<MeshRenderer>();
	if (playerMesh_4 != nullptr)
		playerMesh_4->playAnimation("Idle", true);

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

		MeshRenderer* playerMesh = players[it].gameObject->getComponent<MeshRenderer>();
		if (playerMesh != nullptr) {
			playerMesh->setEnabled(true);
			playerMesh->playAnimation("Idle", true);
		}

		it++;
	}
}

void JuegoDePistolas::MainMenuManager::update(float dT)
{
	// Actualizar texto que parpadea
	playTextTimer += dT / 1000;
	int playTextTimerInt = floorf(playTextTimer);
	if (playTextTimerInt % 2 == 0)
		playText->setText(" ");
	else
		playText->setText("Press any button to start round");

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
		if (players[freePlayer].gameObject->getComponent<MeshRenderer>() != nullptr) {

			players[freePlayer].gameObject->getComponent<MeshRenderer>()->setEnabled(true);
			players[freePlayer].gameObject->getComponent<MeshRenderer>()->playAnimation("Idle", true);
		}
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
	int it = 0;
	for (auto contrID : controllers) {

		if (it > 4)
			break;

		if (Input::InputManager::GetInstance()->GetButtonDown(contrID, Input::LMC_A) ||
			Input::InputManager::GetInstance()->GetButtonDown(contrID, Input::LMC_B) ||
			Input::InputManager::GetInstance()->GetButtonDown(contrID, Input::LMC_X) ||
			Input::InputManager::GetInstance()->GetButtonDown(contrID, Input::LMC_Y)) {
			changeScene = true;
			break;
		}

		MeshRenderer* playerMesh = players[it].gameObject->getComponent<MeshRenderer>();
		if (playerMesh != nullptr)
			playerMesh->updateAnimation(dT / 1000);

		it++;
	}
	if (changeScene) {
		SceneManager::GetInstance()->loadScene("Assets/Scenes/Scene.lua", "Scene");
		SceneManager::GetInstance()->changeScene("Scene");
	}



	// 
	//std::list<LocoMotor::Input::InputManager::ControllerId>  = Input::InputManager::GetInstance()->getCurrentlyConnectedControllers();
	//int it = 0;
	//for (auto contrID : controllers) {
	//	// No se permitiran mas de 4 mandos conectados al mismo tiempo
	//	if (it > 4)
	//		break;
	//	players[it].controllerId = contrID;

	//	MeshRenderer* playerMesh = players[it].gameObject->getComponent<MeshRenderer>();
	//	if (playerMesh != nullptr)
	//		playerMesh->updateAnimation(dT / 1000);

	//	it++;
	//}
}

void JuegoDePistolas::MainMenuManager::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}
