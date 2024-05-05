#include "Bullet.h"
#include "GameObject.h"
#include "Transform.h"
#include "LMVector.h"
#include "ParticleSystem.h"
#include "RigidBody.h"

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
	if (!bulletActive)
		return;

	if (_gameObject != nullptr) {

		RigidBody* rb = _gameObject->getComponent<RigidBody>();
		if (rb != nullptr) {
			rb->BeATrigger();

			if (direction.magnitude() > .1f)
				rb->SetLinearVelocity(direction * velocity * dT);
		}

		timeAlive += dT;
		if (timeAlive > timeToLive) {
			destroyBullet();
			timeAlive = 0;
		}
	}
}

bool JuegoDePistolas::Bullet::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
	return true;
}

void JuegoDePistolas::Bullet::awake()
{
	if (_gameObject != nullptr) {
		_gameObject->addComponent("RigidBody");

		RigidBody* rb = _gameObject->getComponent<RigidBody>();
		if (rb != nullptr) {
			rb->SetMass(0.5);
			rb->SetSize({ 1,0.2,1 });
		}
	}
}


void JuegoDePistolas::Bullet::destroyBullet() {
	timeset = false;

	Scene* activeScene = LocoMotor::SceneManager::GetInstance()->getActiveScene();
	if (activeScene != nullptr && _gameObject != nullptr)
		activeScene->removeGameobject(this->_gameObject->getName());
}

void JuegoDePistolas::Bullet::OnCollisionEnter(GameObject* other)
{
	if (other != nullptr) {
		std::string otherName = other->getName();
		destroyBullet();
	}
}

void JuegoDePistolas::Bullet::setVelocity(int vel) {
	velocity = vel;
}