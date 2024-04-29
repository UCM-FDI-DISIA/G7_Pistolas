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
	if (bulletActive == false)
		return;
	_gameObject->getComponent<RigidBody>()->BeATrigger();
	

	Transform* tr = _gameObject->getComponent<Transform>();
	_gameObject->getComponent<RigidBody>()->SetLinearVelocity(direction * velocity * dT);
	/*if (direction.magnitude() > .1f)
		tr->setPosition(tr->getPosition() + direction * velocity * dT/1000);*/

	//std::array<LocalMultiplayerManager::PlayerData, 4> allPlayers = LocalMultiplayerManager::GetInstance()->getPlayers();

	//allPlayers[0].gameObject;

	// Collision
	//for (int i = 0; i < 4; ++i) {

	//	// Primero comprobar si esta vivo antes de intentar matarlo otra vez
	//	if (!GameplayManager::GetInstance()->isPlayerAlive(i))
	//		continue;

	//	// Si esta vivo, comprobar si esta lo suficientemente cerca de esta bala
	//	GameObject* thisPlayerObj = allPlayers[i].gameObject;
	//	float distance = LMVector3::distance(thisPlayerObj->getComponent<Transform>()->getPosition(), tr->getPosition());
	//	if (LMVector3::distance(thisPlayerObj->getComponent<Transform>()->getPosition(), tr->getPosition()) < 3) {

	//		thisPlayerObj->getComponent<PlayerController>()->bulletHit();
	//	}
	//}
	timeAlive+=dT;
	if (timeAlive>timeToLive) {
		destroyBullet();
		timeAlive = 0;
	}
}

void JuegoDePistolas::Bullet::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

void JuegoDePistolas::Bullet::awake()
{
	_gameObject->addComponent("RigidBody");
	_gameObject->getComponent<RigidBody>()->SetMass(0.5);
	_gameObject->getComponent<RigidBody>()->SetSize({ 1,1,1 });

}


void JuegoDePistolas::Bullet::destroyBullet() {
	timeset = false;
	LocoMotor::SceneManager::GetInstance()->getActiveScene()->removeGameobject(this->_gameObject->getName());
}

void JuegoDePistolas::Bullet::OnCollisionEnter(GameObject* other)
{
	/*std::string otherName = other->getName();
	std::cout << otherName << std::endl;*/
}

void JuegoDePistolas::Bullet::setVelocity(int vel) {
	velocity = vel;
}

