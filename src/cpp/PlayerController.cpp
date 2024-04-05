#include "PlayerController.h"
#include "GameObject.h"
#include "Transform.h"
#include "LMVector.h"

#include <iostream>

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
	std::cout << "PlayerControllerUpdate" << std::endl;

	Transform* cubeTrnsf = _gameObject->getComponent<Transform>();

	if (cubeTrnsf != nullptr)
		cubeTrnsf->SetRotation(LMVector3(cubeTrnsf->GetRotation().GetX(),
			cubeTrnsf->GetRotation().GetY() - 60 * dT / 1000, cubeTrnsf->GetRotation().GetZ()));
}

void JuegoDePistolas::PlayerController::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

