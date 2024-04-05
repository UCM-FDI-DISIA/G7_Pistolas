#include "LocalMultiplayerManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "LMVector.h"

#include <iostream>

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
}

void LocalMultiplayerManager::update(float dT)
{
	Transform* cubeTrnsf = _gameObject->getComponent<Transform>();

	if (cubeTrnsf != nullptr)
		cubeTrnsf->SetRotation(LMVector3(cubeTrnsf->GetRotation().GetX() + 60 * dT / 1000,
			cubeTrnsf->GetRotation().GetY() + 60 * dT / 1000, cubeTrnsf->GetRotation().GetZ()));
}

void LocalMultiplayerManager::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

