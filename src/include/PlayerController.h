#pragma once
#include "Component.h"
#include "InputManager.h"
#include "LMVector.h"

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
		void bulletHit();


		GameObject* createBullet(int id,LMVector3 pos, LMQuaternion rot);

	protected:
		void start() override;
		void update(float dT) override;
		void setParameters(std::vector<std::pair<std::string, std::string>>& params) override;

	private:

		// Id del jugador al que representa este personaje, es decir, al cubo, triangulo, ...
		int playerIndex;

		Input::InputManager::ControllerId controllerId;

		LMVector3 direction;

		float debugParameter = 0;
		int _bullID=0;
	};
}



