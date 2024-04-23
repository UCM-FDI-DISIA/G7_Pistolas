#pragma once
#include "Component.h"
#include "InputManager.h"

namespace JuegoDePistolas {
	class MainMenuManager :public LocoMotor::Component
	{
	public: 
		MainMenuManager();
		~MainMenuManager();
	protected:
		void start() override;
		void update(float dT) override;
		void setParameters(std::vector<std::pair<std::string, std::string>>& params) override;

		struct PlayerData {
			LocoMotor::Input::InputManager::ControllerId controllerId;
			LocoMotor::GameObject* gameObject;
		};
		std::array<PlayerData, 4> players;
	};
}



