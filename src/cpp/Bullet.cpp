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

	// Collision
	for (int i = 0; i < allPlayers.size(); ++i) {

		GameObject* thisPlayerObj = allPlayers[i].gameObject;

		if (LMVector3::Distance(thisPlayerObj->getComponent<Transform>()->GetPosition(), tr->GetPosition()) < 10) {

			thisPlayerObj->getComponent<PlayerController>()->bulletHit();
		}

		// Acceder a los miembros de cada jugador usando allPlayers[i]
		// Ejemplo: std::cout << "Player " << i << ": ID - " << allPlayers[i].id << ", Score - " << allPlayers[i].score << std::endl;
	}
}

void JuegoDePistolas::Bullet::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

