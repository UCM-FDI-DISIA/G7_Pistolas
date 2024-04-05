#include "PlayerController.h"
#include "GameObject.h"
#include "Transform.h"
#include "LMVector.h"
#include "ParticleSystem.h"

#include <iostream>

#include "InputManager.h"
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

void PlayerController::start()
{
}

void PlayerController::update(float dT)
{
	//std::cout << "PlayerControllerUpdate" << std::endl;

	Transform* cubeTrnsf = _gameObject->getComponent<Transform>();

	if (cubeTrnsf != nullptr)
		cubeTrnsf->SetRotation(LMVector3(cubeTrnsf->GetRotation().GetX(),
			cubeTrnsf->GetRotation().GetY() - 60 * dT / 1000, cubeTrnsf->GetRotation().GetZ()));
	if (Input::InputManager::GetInstance()->GetKeyDown(Input::LMKS_9)) {
		SceneManager::GetInstance()->loadScene("Assets/Scenes/Game.lua", "Game");
		SceneManager::GetInstance()->changeScene("Game");
	}

	if (Input::InputManager::GetInstance()->GetKeyDown(Input::LMKS_8)) {
		if (_gameObject->getComponent<ParticleSystem>() != nullptr)
			_gameObject->getComponent<ParticleSystem>()->play();
	}

}

void JuegoDePistolas::PlayerController::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

