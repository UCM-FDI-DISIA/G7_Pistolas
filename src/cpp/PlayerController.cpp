#include "PlayerController.h"
#include "GameObject.h"
#include "Transform.h"
#include "LMVector.h"
#include "ParticleSystem.h"

#include <iostream>

#include "InputManager.h"
#include "MeshRenderer.h"
#include "LMInputs.h"
#include "SceneManager.h"

using namespace JuegoDePistolas;
using namespace LocoMotor;

PlayerController::PlayerController()
{

}

PlayerController::~PlayerController()
{
}

void PlayerController::setControllerId(Input::InputManager::ControllerId _controllerId)
{
	controllerId = _controllerId;
}

void PlayerController::start()
{
}

void PlayerController::update(float dT)
{
	//std::cout << "PlayerControllerUpdate" << std::endl;

	MeshRenderer* mesh = _gameObject->getComponent<MeshRenderer>();
	Transform* tr = _gameObject->getComponent<Transform>();


	float velocity = 130;

	// Si hay un controllerId asignado
	if (controllerId != Input::InputManager::invalidControllerId()) {

		float joystickValue_x = Input::InputManager::GetInstance()->GetJoystickValue(controllerId, 0, Input::InputManager::Axis::Horizontal);
		float joystickValue_y = Input::InputManager::GetInstance()->GetJoystickValue(controllerId, 0, Input::InputManager::Axis::Vertical);


		tr->SetPosition(tr->GetPosition() + LMVector3(joystickValue_x, 0, joystickValue_y) * velocity * dT/1000);

		if (joystickValue_x != 0 || joystickValue_y != 0)
			mesh->playAnimation("Run", true);
		else
			mesh->playAnimation("Idle", true);
	}

	mesh->updateAnimation(dT/1000);

	//if (cubeTrnsf != nullptr)
	//	cubeTrnsf->SetRotation(LMVector3(cubeTrnsf->GetRotation().GetX(),
	//		cubeTrnsf->GetRotation().GetY() - 60 * dT / 1000, cubeTrnsf->GetRotation().GetZ()));



	//_gameObject->getComponent<MeshRenderer>()->updateAnimation(dT / 1000);
	//_gameObject->getComponent<MeshRenderer>()->playAnimation("Idle", true);


	//if (Input::InputManager::GetInstance()->GetKeyDown(Input::LMKS_9)) {
	//	SceneManager::GetInstance()->loadScene("Assets/Scenes/Game.lua", "Game");
	//	SceneManager::GetInstance()->changeScene("Game");
	//}

	if (Input::InputManager::GetInstance()->GetKeyDown(Input::LMKS_8)) {
		if (_gameObject->getComponent<ParticleSystem>() != nullptr)
			_gameObject->getComponent<ParticleSystem>()->play();
	}

}

void JuegoDePistolas::PlayerController::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

