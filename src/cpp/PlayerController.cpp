#include "PlayerController.h"
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
#include <Bullet.h>

using namespace JuegoDePistolas;
using namespace LocoMotor;

PlayerController::PlayerController()
{

}

PlayerController::~PlayerController()
{
}

void PlayerController::setControllerId(Input::InputManager::ControllerId _controllerId)
{
	controllerId = _controllerId;
}

void PlayerController::start()
{
}

void PlayerController::update(float dT)
{
	//std::cout << "PlayerControllerUpdate" << std::endl;

	MeshRenderer* mesh = _gameObject->getComponent<MeshRenderer>();
	Transform* tr = _gameObject->getComponent<Transform>();


	float velocity = 130;

	// Si hay un controllerId asignado
	if (controllerId != Input::InputManager::invalidControllerId()) {

		float joystickValue_x = Input::InputManager::GetInstance()->GetJoystickValue(controllerId, 0, Input::InputManager::Axis::Horizontal);
		float joystickValue_y = Input::InputManager::GetInstance()->GetJoystickValue(controllerId, 0, Input::InputManager::Axis::Vertical);

		// Movimiento horizontal

		LMVector3 currentDirection = LMVector3(joystickValue_x, 0, joystickValue_y);
		tr->SetPosition(tr->GetPosition() + currentDirection * velocity * dT / 1000);

		if (currentDirection.Magnitude() > .1f) {
			direction = currentDirection;
			direction.Normalize();
		}

		// Rotacion
		//LMVector3 upVector = LMVector3(0, 1, 0);

		//float length = std::sqrt(currentDirection.GetX() * currentDirection.GetX() 
		//	+ currentDirection.GetY() * currentDirection.GetY() + currentDirection.GetZ() * currentDirection.GetZ());
		//currentDirection.SetX(currentDirection.GetX() / length);
		//currentDirection.SetY(currentDirection.GetY() / length);
		//currentDirection.SetZ(currentDirection.GetZ() / length);

		//float dotProduct = currentDirection.GetX() * upVector.GetX() + currentDirection.GetY() * upVector.GetY() + currentDirection.GetZ() * upVector.GetZ();
		//float angle = std::acos(dotProduct);


		//std::cout << "--------------------------------------------------" << std::endl;
		//std::cout << "currentDirection X = " << currentDirection.GetX() << " , Y = " << currentDirection.GetY() << " , Z = " << currentDirection.GetZ() << std::endl;
		//currentDirection.Normalize();
		//std::cout << "currentDirection X = " << currentDirection.GetX() << " , Y = " << currentDirection.GetY() << " , Z = " << currentDirection.GetZ() << std::endl;
		//std::cout << "--------------------------------------------------" << std::endl;

		//float angleY = atan2(currentDirection.GetZ(), currentDirection.GetX());
		////float angleX = 0.0f; // En este caso, no estamos interesados en rotar en los ejes X y Z

		////float angleY = math.acos(dy)

		//float halfAngleY = angleY * 0.5f;
		//float cosHalfAngleY = cos(halfAngleY);
		//float sinHalfAngleY = sin(halfAngleY);

		//LMQuaternion quaternion;
		//quaternion.SetW(cosHalfAngleY);
		//quaternion.SetX(0.0f);
		//quaternion.SetY(sinHalfAngleY);
		//quaternion.SetZ(0.0f);

		//angle = 180;
		//float angle = currentDirection.Angle(LMVector3(1, 0, 0));
		//tr->SetRotation(LMVector3(0, angleY, 0));


		if (Input::InputManager::GetInstance()->GetButton(controllerId, Input::LMC_A)) {
			tr->SetRotation(LMVector3(tr->GetRotation().GetX(),
				tr->GetRotation().GetY() - 150 * dT / 1000, tr->GetRotation().GetZ()));
		}
		else if (Input::InputManager::GetInstance()->GetButton(controllerId, Input::LMC_B)) {
			tr->SetRotation(LMVector3(tr->GetRotation().GetX(),
				tr->GetRotation().GetY() + 150 * dT / 1000, tr->GetRotation().GetZ()));
		}

		//currentDirection.Normalize();


		//// Calcular el eje de rotación
		//LMVector3 axisVector = LMVector3(currentDirection.GetY() * upVector.GetZ()  - currentDirection.GetZ() * upVector.GetY(),
		//	currentDirection.GetZ() * upVector.GetX() - currentDirection.GetX() * upVector.GetZ(),
		//	currentDirection.GetX() * upVector.GetY() - currentDirection.GetY() * upVector.GetX());
		//axisVector.Normalize();
		//float half_angle = angle / 2;
		//float cos_half_angle = std::cos(half_angle);
		//float sin_half_angle = std::sin(half_angle);
		//LMQuaternion finalQuaternion = LMQuaternion(cos_half_angle, axisVector.GetX() * sin_half_angle,
		//	axisVector.GetY() * sin_half_angle, axisVector.GetZ() * sin_half_angle);
		//tr->SetRotation(finalQuaternion);


		// Cambiar la animacion si es necesario
		if (joystickValue_x != 0 || joystickValue_y != 0)
			mesh->playAnimation("Run", true);
		else
			mesh->playAnimation("Idle", true);


		// Disparo

		if (Input::InputManager::GetInstance()->GetButtonDown(controllerId, Input::LMC_RIGHTSHOULDER)) {

			//std::cout << "Shoot" << std::endl;

			//GameObject* bullet = SceneManager::GetInstance()->getActiveScene()->addGameobject("bullet");

			//Transform* bulletTr = (Transform*)bullet->addComponent("Transform");
			//bulletTr->SetPosition(tr->GetPosition() + currentDirection);
			//if (bulletTr == nullptr)
			//	std::cout << "bulletTr NULL" << std::endl;

			//MeshRenderer* meshRenderer = (MeshRenderer*) bullet->addComponent("MeshRenderer");
			//if (meshRenderer == nullptr)
			//	std::cout << "meshRenderer NULL" << std::endl;

			//meshRenderer->setMesh("Revolver.mesh");
			//meshRenderer->setMaterial("Revolver");


			GameObject* bullet = SceneManager::GetInstance()->getActiveScene()->getObjectByName("Bullet");

			Transform* bulletTr = (Transform*)bullet->addComponent("Transform");
			bulletTr->SetPosition(tr->GetPosition() + direction * 30);
			if (bulletTr == nullptr)
				std::cout << "bulletTr NULL" << std::endl;

			bulletTr->SetRotation(tr->GetRotation());
			bullet->getComponent<Bullet>()->setDirection(direction);
		}
	}

	mesh->updateAnimation(dT / 1000);

	//if (cubeTrnsf != nullptr)
	//	cubeTrnsf->SetRotation(LMVector3(cubeTrnsf->GetRotation().GetX(),
	//		cubeTrnsf->GetRotation().GetY() - 60 * dT / 1000, cubeTrnsf->GetRotation().GetZ()));



	//_gameObject->getComponent<MeshRenderer>()->updateAnimation(dT / 1000);
	//_gameObject->getComponent<MeshRenderer>()->playAnimation("Idle", true);


	//if (Input::InputManager::GetInstance()->GetKeyDown(Input::LMKS_9)) {
	//	SceneManager::GetInstance()->loadScene("Assets/Scenes/Game.lua", "Game");
	//	SceneManager::GetInstance()->changeScene("Game");
	//}

	if (Input::InputManager::GetInstance()->GetKeyDown(Input::LMKS_8)) {
		if (_gameObject->getComponent<ParticleSystem>() != nullptr)
			_gameObject->getComponent<ParticleSystem>()->play();
	}

}

void JuegoDePistolas::PlayerController::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

