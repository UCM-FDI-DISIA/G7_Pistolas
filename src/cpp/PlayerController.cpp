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

	Scene* activeScene = SceneManager::GetInstance()->getActiveScene();

	if (activeScene != nullptr) {
		// Al saber el id de este jugador y por lo tanto, su color podemos crear la sombra que tendra
		GameObject* shadowLine = activeScene->addGameobjectRuntime("shadowLine_" + playerIndex);

		if (shadowLine != nullptr) {
			// Guardarse una referencia al transform de la sombra
			shadowLineTr = (Transform*)shadowLine->addComponent("Transform");
			shadowLineMesh = (MeshRenderer*)shadowLine->addComponent("MeshRenderer");

			if (shadowLineMesh != nullptr) {

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
			}
		}
	}
}

void PlayerController::setControllerId(Input::InputManager::ControllerId _controllerId)
{
	controllerId = _controllerId;
}

void JuegoDePistolas::PlayerController::bulletHit()
{
	GameplayManager* gameplayManager = GameplayManager::GetInstance();

	if (gameplayManager != nullptr && gameplayManager->isPlayerAlive(playerIndex)) {

		gameplayManager->playerDied(playerIndex);
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

	GameplayManager* gameplayManager = GameplayManager::GetInstance();
	if (gameplayManager != nullptr)
		gameplayManager->freeSpawnpoint(spawnId);
}

void JuegoDePistolas::PlayerController::dropWeapon()
{
	// Si este personaje tiene un arma
	if (hasWeapon) {

		Scene* activeScene = SceneManager::GetInstance()->getActiveScene();

		if (activeScene != nullptr) {

			// Eliminar el objeto arma
			GameObject* weaponGO = activeScene->getObjectByName(weaponName);
			if (weaponGO != nullptr) {
				Weapon* weaponComp = weaponGO->getComponent<Weapon>();
				if (weaponComp != nullptr)
					weaponComp->deleteWeapon();
			}
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
	if (mesh == nullptr && _gameObject != nullptr)
		mesh = _gameObject->getComponent<MeshRenderer>();

	if (mesh != nullptr) {
		mesh->setVisible(active);
		mesh->setEnabled(active);
	}

	if (tr == nullptr && _gameObject != nullptr)
		tr = _gameObject->getComponent<Transform>();

	if (tr != nullptr && !active)
		tr->setPosition(LMVector3(0, -500, 0));

	if (shadowLineMesh != nullptr) {
		shadowLineMesh->setVisible(active);
		shadowLineMesh->setEnabled(active);
	}
}

void JuegoDePistolas::PlayerController::OnCollisionEnter(GameObject* other)
{
	if (other != nullptr) {
		std::string otherName = other->getName();
		if (otherName.find("Bullet") != std::string::npos) {
			bulletHit();
		}
	}
}

void JuegoDePistolas::PlayerController::OnCollisionStay(GameObject* other)
{
	if (other != nullptr) {
		std::string otherName = other->getName();
		if (otherName.find("platform") != std::string::npos) {
			isOnFloor = true;
		}
	}
}

void JuegoDePistolas::PlayerController::OnCollisionExit(GameObject* other)
{
	if (other != nullptr) {
		std::string otherName = other->getName();
		if (otherName.find("platform") != std::string::npos) {
			isOnFloor = false;
		}
	}
}

void PlayerController::start()
{
	if (_gameObject != nullptr) {
		rb = _gameObject->getComponent<RigidBody>();
		if (rb != nullptr) {
			rb->FreezeRotation({ 0,0,0 });
			rb->UseGravity(LMVector3(0, -gravity, 0));
			rb->SetLinearDamping(linearDrag);
			rb->SetAngularDamping(0);
		}

		tr = _gameObject->getComponent<Transform>();
		if (tr != nullptr)
			tr->setSize(LMVector3(1.5f, 1.5f, 1.5f));

		mesh = _gameObject->getComponent<MeshRenderer>();
	}
}

void PlayerController::update(float dT)
{
	// Comprobar si esta desactivado
	if (!playerActive)
		return;

	// Mover la sombra debajo del jugador cada frame
	if (shadowLineTr != nullptr && tr != nullptr)
		shadowLineTr->setPosition(tr->getPosition());


	// Si hay un controllerId asignado
	if (canMove && controllerId != Input::InputManager::invalidControllerId()) {

		float joystickValue_x = Input::InputManager::GetInstance()->GetJoystickValue(controllerId, 0, Input::InputManager::Axis::Horizontal);
		float joystickValue_y = Input::InputManager::GetInstance()->GetJoystickValue(controllerId, 0, Input::InputManager::Axis::Vertical);

		if (rb != nullptr && _gameObject != nullptr) {

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

			if (joystickValue_x > 0.1f || joystickValue_x < -0.1f || joystickValue_y > 0.1f || joystickValue_y < -0.1f) {
				if (_gameObject->getComponent<ParticleSystem>() != nullptr)
					_gameObject->getComponent<ParticleSystem>()->play();
			}
			else {
				if (_gameObject->getComponent<ParticleSystem>() != nullptr)
					_gameObject->getComponent<ParticleSystem>()->stop();
			}

			float lookJoystickValue_x = Input::InputManager::GetInstance()->GetJoystickValue(controllerId, 1, Input::InputManager::Axis::Horizontal);
			float lookJoystickValue_y = Input::InputManager::GetInstance()->GetJoystickValue(controllerId, 1, Input::InputManager::Axis::Vertical);

			LMVector3 lookDirection = LMVector3(lookJoystickValue_x, 0, lookJoystickValue_y);

			if (lookDirection.magnitude() > .1f) {
				direction = lookDirection;
				direction.normalize();
			}
			lookDirection.rotate(LMVector3(0, 1, 0), 180);

			if (tr != nullptr) {
				tr->setForward(lookDirection);
				// Pruebas debug
				//tr->setForward(lookDirection);
				//tr->setForward(lookDirection);
				//tr->setForward(lookDirection);
				//tr->setForward(lookDirection);
				//tr->setForward(lookDirection);
				//tr->setForward(lookDirection);
				//tr->setForward(lookDirection);
				//tr->setForward(lookDirection);
				//tr->setForward(lookDirection);
				//tr->setForward(lookDirection);
			}

			if (mesh != nullptr) {
				// Cambiar la animacion si es necesario
				if (joystickValue_x != 0 || joystickValue_y != 0)
					mesh->playAnimation("Run", true);
				else
					mesh->playAnimation("Idle", true);
			}


			// Disparo

			if (hasWeapon && Input::InputManager::GetInstance()->GetButtonDown(controllerId, Input::LMC_RIGHTSHOULDER)) {

				Scene* activeScene = SceneManager::GetInstance()->getActiveScene();

				if (activeScene != nullptr) {
					GameObject* weaponGO = activeScene->getObjectByName(weaponName);
					if (weaponGO != nullptr) {
						Weapon* weaponComp = weaponGO->getComponent<Weapon>();
						if (weaponComp != nullptr) {
							weaponComp->shoot(playerIndex, bulletID);
							bulletID++;
						}
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
				}
				if (_gameObject != nullptr && _gameObject->getComponent<EventEmitter>() != nullptr) {
					_gameObject->getComponent<EventEmitter>()->play();
				}

			}
		}
	}

	// Actualizar animacion de este mesh
	if (mesh != nullptr)
		mesh->updateAnimation(dT / 1000);
}

void JuegoDePistolas::PlayerController::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}