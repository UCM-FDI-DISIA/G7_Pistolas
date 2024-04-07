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

		void setControllerId(Input::InputManager::ControllerId controllerId);

	protected:
		void start() override;
		void update(float dT) override;
		void setParameters(std::vector<std::pair<std::string, std::string>>& params) override;

	private:
		Input::InputManager::ControllerId controllerId;

		LMVector3 direction;
	};
}



