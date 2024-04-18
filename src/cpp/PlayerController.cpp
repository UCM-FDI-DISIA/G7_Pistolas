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

void JuegoDePistolas::PlayerController::pickWeapon(std::string name)
{
	weaponName = name;
	hasWeapon = true;
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

void PlayerController::start()
{
	Transform* tr = _gameObject->getComponent<Transform>();
	
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



	float velocity = 130;

	// Si hay un controllerId asignado
	if (controllerId != Input::InputManager::invalidControllerId()) {

		float joystickValue_x = Input::InputManager::GetInstance()->GetJoystickValue(controllerId, 0, Input::InputManager::Axis::Horizontal);
		float joystickValue_y = Input::InputManager::GetInstance()->GetJoystickValue(controllerId, 0, Input::InputManager::Axis::Vertical);

		// Movimiento horizontal

		LMVector3 currentDirection = LMVector3(joystickValue_x, 0, joystickValue_y);
		tr->setPosition(tr->getPosition() + currentDirection * velocity * dT / 1000);

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
					weaponComp->shoot(playerIndex, bulletID,_gameObject);
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

	if (Input::InputManager::GetInstance()->GetKeyDown(Input::LMKS_8)) {
		if (_gameObject->getComponent<ParticleSystem>() != nullptr)
			//_gameObject->getComponent<ParticleSystem>()->play();
		{
			SceneManager::GetInstance()->loadScene("Assets/Scenes/Game.lua", "Game");
			SceneManager::GetInstance()->changeScene("Game");
		}
	}
	if (Input::InputManager::GetInstance()->GetKeyDown(Input::LMKS_0)) {
		if (_gameObject->getComponent<RigidBody>() != nullptr) {
			_gameObject->getComponent<RigidBody>()->ApplyCentralImpulse({ 0,50,0 });
			_gameObject->getComponent<RigidBody>()->FreezeRotation({ 0,0,0 });
			_gameObject->getComponent<RigidBody>()->UseGravity({ 0,0,0 });
		}
			
	}

}

void JuegoDePistolas::PlayerController::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}


