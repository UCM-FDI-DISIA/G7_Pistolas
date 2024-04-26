#include "PlayerController.h"
#include "GameObject.h"
#include "Transform.h"
#include "LMVector.h"
#include "ParticleSystem.h"

#include <iostream>
#include <cmath>
#include <string>

#include "InputManager.h"
#include "MeshRenderer.h"
#include "LMInputs.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Bullet.h"
#include "RigidBody.h"

#include "GameplayManager.h"
#include "Weapon.h"

using namespace JuegoDePistolas;
using namespace LocoMotor;

PlayerController::PlayerController()
{

}

PlayerController::~PlayerController()
{
}

void JuegoDePistolas::PlayerController::setPlayerId(int _playerIndex)
{
	playerIndex = _playerIndex;
}

void PlayerController::setControllerId(Input::InputManager::ControllerId _controllerId)
{
	controllerId = _controllerId;
}

void JuegoDePistolas::PlayerController::bulletHit()
{
	//GameplayManager.GetInstance()

	GameplayManager::GetInstance()->playerDied(playerIndex);
}

bool JuegoDePistolas::PlayerController::getHasWeapon()
{
	return hasWeapon;
}

void JuegoDePistolas::PlayerController::pickWeapon(std::string name,int spawnId)
{
	weaponName = name;
	hasWeapon = true;
	GameplayManager::GetInstance()->freeSpawnpoint(spawnId);
}

void JuegoDePistolas::PlayerController::dropWeapon()
{
	weaponName = "";
	hasWeapon = false;
}

LMVector3 JuegoDePistolas::PlayerController::getDirection()
{
	return direction;
}

void JuegoDePistolas::PlayerController::OnCollisionEnter(GameObject* other)
{

}

void JuegoDePistolas::PlayerController::OnCollisionStay(GameObject* other)
{
	std::string otherName = other->getName();
	if (otherName.find("platform") != std::string::npos) {
		isOnFloor = true;
	}
}

void JuegoDePistolas::PlayerController::OnCollisionExit(GameObject* other)
{
	std::string otherName = other->getName();
	if (otherName.find("platform") != std::string::npos) {
		isOnFloor = false;
	}
}

void PlayerController::start()
{
	Transform* tr = _gameObject->getComponent<Transform>();

	_gameObject->getComponent<RigidBody>()->FreezeRotation({ 0,0,0 });
	//tr->SetSize(LMVector3(.5f, .5f, .5f));
}

void PlayerController::update(float dT)
{
	//std::cout << "PlayerControllerUpdate" << std::endl;
	
	MeshRenderer* mesh = _gameObject->getComponent<MeshRenderer>();
	Transform* tr = _gameObject->getComponent<Transform>();

	////tr = _gameObject->getComponent<Transform>();
	//tr->SetRotation(LMVector3(0, debugParameter, 0));
	//debugParameter += dT / 1000 * 20;

	//return;


		// Mover revolver
	/*GameObject* revolver = SceneManager::GetInstance()->getActiveScene()->getObjectByName("revolver");
	revolver->getComponent<Transform>()->setPosition(tr->getPosition() + direction * 5);*/

	//LMQuaternion revolverRotation = tr->getRotation().rotate(LMVector3(0, 1, 0), -90);
	////revolverRotation = revolverRotation.Rotate(LMVector3(0, 0, 1), 90);
	//revolver->getComponent<Transform>()->setRotation(revolverRotation);



	float velocity = 1500;

	// Si hay un controllerId asignado
	if (controllerId != Input::InputManager::invalidControllerId()) {

		float joystickValue_x = Input::InputManager::GetInstance()->GetJoystickValue(controllerId, 0, Input::InputManager::Axis::Horizontal);
		float joystickValue_y = Input::InputManager::GetInstance()->GetJoystickValue(controllerId, 0, Input::InputManager::Axis::Vertical);

		// Movimiento horizontal

		LMVector3 currentDirection = LMVector3(joystickValue_x, 0, joystickValue_y);
		_gameObject->getComponent<RigidBody>()->AddForce(currentDirection * velocity * dT / 1000);

		if (joystickValue_x > 0.1f || joystickValue_x < -0.1f || joystickValue_y > 0.1f || joystickValue_y < -0.1f)
			_gameObject->getComponent<ParticleSystem>()->play();
		else
			_gameObject->getComponent<ParticleSystem>()->stop();

		float lookJoystickValue_x = Input::InputManager::GetInstance()->GetJoystickValue(controllerId, 1, Input::InputManager::Axis::Horizontal);
		float lookJoystickValue_y = Input::InputManager::GetInstance()->GetJoystickValue(controllerId, 1, Input::InputManager::Axis::Vertical);

		LMVector3 lookDirection = LMVector3(lookJoystickValue_x, 0, lookJoystickValue_y);

		if (lookDirection.magnitude() > .1f) {
			direction = lookDirection;
			direction.normalize();
		}
		lookDirection.rotate(LMVector3(0, 1, 0), 180);
		tr->setForward(lookDirection);
		tr->setForward(lookDirection);
		tr->setForward(lookDirection);
		tr->setForward(lookDirection);
		tr->setForward(lookDirection);
		tr->setForward(lookDirection);
		tr->setForward(lookDirection);
		tr->setForward(lookDirection);
		tr->setForward(lookDirection);
		tr->setForward(lookDirection);
		tr->setForward(lookDirection);


		// Cambiar la animacion si es necesario
		if (joystickValue_x != 0 || joystickValue_y != 0)
			mesh->playAnimation("Run", true);
		else
			mesh->playAnimation("Idle", true);



		// Disparo

		if (hasWeapon && Input::InputManager::GetInstance()->GetButtonDown(controllerId, Input::LMC_RIGHTSHOULDER)) {
			//createBullet(_bulletID, tr->getPosition() + direction * 200, tr->getRotation());
			GameObject* weaponGO = SceneManager::GetInstance()->getActiveScene()->getObjectByName(weaponName);
			if (weaponGO != nullptr) {
				Weapon* weaponComp = weaponGO->getComponent<Weapon>();
				if (weaponComp != nullptr) {
					weaponComp->shoot(playerIndex, bulletID);
					bulletID++;
				}
			}
			/*GameObject* bullet = SceneManager::GetInstance()->getActiveScene()->getObjectByName("Bullet");
			bullet->getComponent<Bullet>()->setBulletActive(true);
			
			Transform* bulletTr = (Transform*)bullet->addComponent("Transform");
			bulletTr->setPosition(tr->getPosition() + direction * 50);
			if (bulletTr == nullptr)
				std::cout << "bulletTr NULL" << std::endl;

			bulletTr->setRotation();
			bullet->getComponent<Bullet>()->setDirection(direction);*/
		}
	}

	mesh->updateAnimation(dT / 1000);



	//_gameObject->getComponent<MeshRenderer>()->updateAnimation(dT / 1000);
	//_gameObject->getComponent<MeshRenderer>()->playAnimation("Idle", true);


	//if (Input::InputManager::GetInstance()->GetKeyDown(Input::LMKS_9)) {
	//	SceneManager::GetInstance()->loadScene("Assets/Scenes/Game.lua", "Game");
	//	SceneManager::GetInstance()->changeScene("Game");d
	//}

	//if (Input::InputManager::GetInstance()->GetKeyDown(Input::LMKS_8)) {
	//	if (_gameObject->getComponent<ParticleSystem>() != nullptr)
	//		//_gameObject->getComponent<ParticleSystem>()->play();
	//	{
	//		SceneManager::GetInstance()->loadScene("Assets/Scenes/Menu.lua", "Menu");
	//		SceneManager::GetInstance()->changeScene("Menu");
	//	}
	//}

	if (Input::InputManager::GetInstance()->GetButtonDown(controllerId, Input::LMC_A) && isOnFloor) {
		if (_gameObject->getComponent<RigidBody>() != nullptr) {
			if(_gameObject->getComponent<RigidBody>()->GetLinearVelocity().getY() < 5)
			_gameObject->getComponent<RigidBody>()->ApplyCentralImpulse({ 0,20,0 });
			//_gameObject->getComponent<RigidBody>()->UseGravity({ 0,0,0 });
		}
			
	}


}

void JuegoDePistolas::PlayerController::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}


