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
	shadowLineMesh = (MeshRenderer*)shadowLine->addComponent("MeshRenderer");

	shadowLineMesh->setMesh("ShadowLine.mesh");
	// Asignar el material adecuado
	switch (playerIndex)
	{
	case 0:
		shadowLineMesh->setMaterial("ShadowLineBlue");
		break;
	case 1:
		shadowLineMesh->setMaterial("ShadowLineRed");
		break;
	case 2:
		shadowLineMesh->setMaterial("ShadowLineGreen");
		break;
	case 3:
		shadowLineMesh->setMaterial("ShadowLinePurple");
		break;
	default:
		break;
	}

	//shadowLineMesh->setVisible(true);
	//shadowLineMesh->setEnabled(true);
}

void PlayerController::setControllerId(Input::InputManager::ControllerId _controllerId)
{
	controllerId = _controllerId;
}

void JuegoDePistolas::PlayerController::bulletHit()
{
	if (GameplayManager::GetInstance()->isPlayerAlive(playerIndex)) {

		GameplayManager::GetInstance()->playerDied(playerIndex);
		setPlayerActive(false);

		// Eliminar el arma actual si existe
		dropWeapon();
	}
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
	// Si este personaje tiene un arma
	if (hasWeapon) {

		// Eliminar el objeto arma
		GameObject* weaponGO = SceneManager::GetInstance()->getActiveScene()->getObjectByName(weaponName);
		if (weaponGO != nullptr) {
			Weapon* weaponComp = weaponGO->getComponent<Weapon>();
			if (weaponComp != nullptr)
				weaponComp->deleteWeapon();
		}

		// Actualizar variables
		weaponName = "";
		hasWeapon = false;
	}
}

void JuegoDePistolas::PlayerController::setCanMove(bool _canMove)
{
	canMove = _canMove;
}

LMVector3 JuegoDePistolas::PlayerController::getDirection()
{
	return direction;
}

void JuegoDePistolas::PlayerController::setPlayerActive(bool active)
{
	playerActive = active;

	// Desactivar el objeto
	//playerData.gameObject->setActive(active);
	if (mesh == nullptr)
		mesh = _gameObject->getComponent<MeshRenderer>();
	mesh->setVisible(active);
	mesh->setEnabled(active);

	if (tr == nullptr)
		tr = _gameObject->getComponent<Transform>();

	if (!active)
		tr->setPosition(LMVector3(0, -500, 0));

	shadowLineMesh->setVisible(active);
	shadowLineMesh->setEnabled(active);
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

	tr = _gameObject->getComponent<Transform>();
	tr->setSize(LMVector3(1.5f, 1.5f, 1.5f));

	mesh = _gameObject->getComponent<MeshRenderer>();
}

void PlayerController::update(float dT)
{
	// Comprobar si esta desactivado
	if (!playerActive)
		return;

	//std::cout << "PlayerControllerUpdate" << std::endl;


	// Mover la sombra debajo del jugador cada frame
	shadowLineTr->setPosition(tr->getPosition());



	// Si hay un controllerId asignado
	if (canMove && controllerId != Input::InputManager::invalidControllerId()) {

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
		}

		// Salto
		bool jumpPressed = (Input::InputManager::GetInstance()->GetButtonDown(controllerId, Input::LMC_LEFTSHOULDER)
			|| Input::InputManager::GetInstance()->GetButtonDown(controllerId, Input::LMC_A));

		if (jumpPressed && isOnFloor) {
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

	// Actualizar animacion de este mesh
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


}

void JuegoDePistolas::PlayerController::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}


