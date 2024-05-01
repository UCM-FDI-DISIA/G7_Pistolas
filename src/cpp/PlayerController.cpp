#include "PlayerController.h"
#include "GameObject.h"
#include "Transform.h"
#include "LMVector.h"
#include "ParticleSystem.h"
#include "EventEmitter.h"

#include <iostream>
#include <cmath>
#include <string>

#include "InputManager.h"
#include "MeshRenderer.h"
#include "LMInputs.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Bullet.h"

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


	// Al saber el id de este jugador y por lo tanto, su color podemos crear la sombra que tendra
	GameObject* shadowLine = SceneManager::GetInstance()->getActiveScene()->addGameobjectRuntime("shadowLine_" + playerIndex);

	// Guardarse una referencia al transform de la sombra
	shadowLineTr = (Transform*)shadowLine->addComponent("Transform");
	MeshRenderer* meshComp = (MeshRenderer*)shadowLine->addComponent("MeshRenderer");

	meshComp->setMesh("ShadowLine.mesh");
	// Asignar el material adecuado
	switch (playerIndex)
	{
	case 0:
		meshComp->setMaterial("ShadowLineBlue");
		break;
	case 1:
		meshComp->setMaterial("ShadowLineRed");
		break;
	case 2:
		meshComp->setMaterial("ShadowLineGreen");
		break;
	case 3:
		meshComp->setMaterial("ShadowLinePurple");
		break;
	default:
		break;
	}

	meshComp->setVisible(true);
	meshComp->setEnabled(true);
}

void PlayerController::setControllerId(Input::InputManager::ControllerId _controllerId)
{
	controllerId = _controllerId;
}

void JuegoDePistolas::PlayerController::bulletHit()
{
	if (GameplayManager::GetInstance()->isPlayerAlive(playerIndex))
		GameplayManager::GetInstance()->playerDied(playerIndex);
}

bool JuegoDePistolas::PlayerController::getHasWeapon()
{
	return hasWeapon;
}

void JuegoDePistolas::PlayerController::pickWeapon(std::string name, int spawnId)
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
	std::string otherName = other->getName();
	if (otherName.find("Bullet") != std::string::npos) {
		bulletHit();
	}
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
	rb = _gameObject->getComponent<RigidBody>();
	rb->FreezeRotation({ 0,0,0 });
	rb->UseGravity(LMVector3(0, -gravity, 0));
	rb->SetLinearDamping(linearDrag);
	rb->SetAngularDamping(0);

	Transform* tr = _gameObject->getComponent<Transform>();
	tr->setSize(LMVector3(1.5f, 1.5f, 1.5f));
}

void PlayerController::update(float dT)
{
	//std::cout << "PlayerControllerUpdate" << std::endl;

	MeshRenderer* mesh = _gameObject->getComponent<MeshRenderer>();
	Transform* tr = _gameObject->getComponent<Transform>();


	// Mover la sombra debajo del jugador cada frame
	shadowLineTr->setPosition(tr->getPosition());



	// Si hay un controllerId asignado
	if (controllerId != Input::InputManager::invalidControllerId()) {

		float joystickValue_x = Input::InputManager::GetInstance()->GetJoystickValue(controllerId, 0, Input::InputManager::Axis::Horizontal);
		float joystickValue_y = Input::InputManager::GetInstance()->GetJoystickValue(controllerId, 0, Input::InputManager::Axis::Vertical);

		// Movimiento horizontal

		LMVector3 currentDirection = LMVector3(joystickValue_x, 0, joystickValue_y);
		rb->AddForce(currentDirection * velocity * dT / 1000);
		
		// Limitar la velocidad maxima horizontal a la que puede moverse el personaje
		LMVector3 currentVelocity = rb->GetLinearVelocity();
		float velocityY = currentVelocity.getY();
		currentVelocity = LMVector3(currentVelocity.getX(), 0, currentVelocity.getZ());
		float linearVelocity = currentVelocity.magnitude();

		float maxVelocity;
		if (isOnFloor)
			maxVelocity = floorMaxHorizontalVelocity;
		else 
			maxVelocity = airMaxHorizontalVelocity;

		if (linearVelocity > maxVelocity)
		{
			// Calcular velocidad maxima en la direccion actual
			currentVelocity.normalize();
			LMVector3 maxHorVelocity = currentVelocity * maxVelocity;
			maxHorVelocity = LMVector3(maxHorVelocity.getX(), velocityY, maxHorVelocity.getZ());
			rb->SetLinearVelocity(maxHorVelocity);
		}

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

	bool buttonPressed = (Input::InputManager::GetInstance()->GetButtonDown(controllerId, Input::LMC_LEFTSHOULDER) 
		|| Input::InputManager::GetInstance()->GetButtonDown(controllerId, Input::LMC_A));

	if (buttonPressed && isOnFloor) {
		if (rb != nullptr) {
			if (rb->GetLinearVelocity().getY() < 5)
				rb->ApplyCentralImpulse({ 0,jumpForce,0 });
			//_gameObject->getComponent<RigidBody>()->UseGravity({ 0,0,0 });
		}
		if (_gameObject->getComponent<EventEmitter>() != nullptr) {
			_gameObject->getComponent<EventEmitter>()->play();
		}

	}


}

void JuegoDePistolas::PlayerController::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}


