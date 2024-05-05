#pragma once
#include "Component.h"
#include "InputManager.h"
#include "LMVector.h"
#include "Transform.h"
#include "RigidBody.h"
#include "MeshRenderer.h"

using namespace LocoMotor;

namespace JuegoDePistolas {
	class PlayerController:public LocoMotor::Component
	{
	public:
		PlayerController();
		~PlayerController();

		void setPlayerId(int _playerIndex);
		void setControllerId(Input::InputManager::ControllerId controllerId);

		// Se llama desde una bala cuando esta colisiona con este personaje
		void playerDie();
		bool getHasWeapon();
		void pickWeapon(std::string name,int spawnId);
		// Elimina el arma que tiene este jugador ahora mismo, si no tiene arma, no tiene ningun efecto
		void dropWeapon();
		void setCanMove(bool _canMove);

		LMVector3 getDirection();

		void setPlayerActive(bool active);

		void OnCollisionEnter(GameObject* other)override;
		void OnCollisionStay(GameObject* other)override;
		void OnCollisionExit(GameObject* other)override;

	protected:
		void start() override;
		void update(float dT) override;
		bool setParameters(std::vector<std::pair<std::string, std::string>>& params) override;

	private:

		// Id del jugador al que representa este personaje, es decir, al cubo, triangulo, ...
		int playerIndex;

		// Define si este jugador puede ser controlado por el usurio o no
		bool canMove = false;

		Input::InputManager::ControllerId controllerId;

		LMVector3 direction;

		float debugParameter = 0;
		int bulletID = 0;

		bool hasWeapon = false;
		bool isOnFloor = false;
		std::string weaponName = "";

		// Referencia al transform de la sombra de este personaje
		Transform* shadowLineTr;
		MeshRenderer* shadowLineMesh;

		MeshRenderer* mesh;
		Transform* tr;
		RigidBody* rb;

		float jumpForce = 50;
		float gravity = 100;

		float velocity = 5000;
		float floorMaxHorizontalVelocity = 30;
		float airMaxHorizontalVelocity = 25;
		float linearDrag = .9f;

		bool playerActive;

		float deadZone = -25;
	};
}