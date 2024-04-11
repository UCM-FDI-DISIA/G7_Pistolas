#include "Bullet.h"
#include "GameObject.h"
#include "Transform.h"
#include "LMVector.h"
#include "ParticleSystem.h"

#include <iostream>
#include <cmath>

#include "InputManager.h"
#include "MeshRenderer.h"
#include "LMInputs.h"
#include "SceneManager.h"
#include "Scene.h"
#include "LocalMultiplayerManager.h"
#include "GameplayManager.h"

using namespace JuegoDePistolas;
using namespace LocoMotor;

Bullet::Bullet()
{

}

Bullet::~Bullet()
{
}

void JuegoDePistolas::Bullet::setDirection(LMVector3 _direction)
{
	direction = _direction;
}

void JuegoDePistolas::Bullet::setBulletActive(bool _bulletActive)
{
	bulletActive = _bulletActive;
}


void Bullet::start()
{
}

void Bullet::update(float dT)
{
	// Si esta desactivado, no ejecutar el update
	if (bulletActive == false)
		return;

	Transform* tr = _gameObject->getComponent<Transform>();

	if (direction.Magnitude() > .1f)
		tr->SetPosition(tr->GetPosition() + direction * velocity * dT/1000);

	std::array<LocalMultiplayerManager::PlayerData, 4> allPlayers = LocalMultiplayerManager::GetInstance()->getPlayers();

	allPlayers[0].gameObject;

	// Collision
	for (int i = 0; i < 4; ++i) {

		// Primero comprobar si esta vivo antes de intentar matarlo otra vez
		if (!GameplayManager::GetInstance()->isPlayerAlive(i))
			continue;

		// Si esta vivo, comprobar si esta lo suficientemente cerca de esta bala
		GameObject* thisPlayerObj = allPlayers[i].gameObject;
		float distance = LMVector3::Distance(thisPlayerObj->getComponent<Transform>()->GetPosition(), tr->GetPosition());
		if (LMVector3::Distance(thisPlayerObj->getComponent<Transform>()->GetPosition(), tr->GetPosition()) < 30) {

			thisPlayerObj->getComponent<PlayerController>()->bulletHit();
		}
	}
}

void JuegoDePistolas::Bullet::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

