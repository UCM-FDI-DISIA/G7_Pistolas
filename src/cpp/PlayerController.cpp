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
	GameObject* revolver = SceneManager::GetInstance()->getActiveScene()->getObjectByName("revolver");
	revolver->getComponent<Transform>()->setPosition(tr->getPosition() + direction * 50);

	LMQuaternion revolverRotation = tr->getRotation().rotate(LMVector3(0, 1, 0), -90);
	//revolverRotation = revolverRotation.Rotate(LMVector3(0, 0, 1), 90);
	revolver->getComponent<Transform>()->setRotation(revolverRotation);



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

		if (Input::InputManager::GetInstance()->GetButtonDown(controllerId, Input::LMC_RIGHTSHOULDER)) {


			
			

			createBullet(_bullID, tr->getPosition() + direction * 200, tr->getRotation());
			_bullID++;
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
			_gameObject->getComponent<ParticleSystem>()->play();
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


GameObject* JuegoDePistolas::PlayerController::createBullet(int id, LMVector3 pos, LMQuaternion rot) {


	std::string bulletName = "Bullet" + std::to_string(playerIndex) + std::to_string(id);


	GameObject* nBullet = SceneManager::GetInstance()->getActiveScene()->addGameobjectRuntime(bulletName);

	Bullet* bullComp = (Bullet*)nBullet->addComponent("Bullet");
	Transform* transfComp = (Transform*)nBullet->addComponent("Transform");
	MeshRenderer* meshComp = (MeshRenderer*)nBullet->addComponent("MeshRenderer");

	meshComp->setMesh("Bullet.mesh");
	meshComp->setMaterial("Bullet");
	meshComp->setVisible(true);
	meshComp->setEnabled(true);
	bullComp->setBulletActive(true);
	bullComp->setVelocity(200);
	
	//transfComp->setPosition(pos);
	transfComp->setPosition({pos.getX()-4,pos.getY()-1,pos.getZ()});
	transfComp->setSize({ 2, 2, 2 });
	transfComp->setRotation(rot);
	bullComp->setDirection(transfComp->getEulerRotation());


	return nBullet;
}

