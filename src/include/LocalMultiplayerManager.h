#pragma once
#include "Component.h"
#include <unordered_map>
#include "InputManager.h"
#include "LMVector.h"
#include "PlayerController.h"
#include <utility>


using namespace LocoMotor;

namespace JuegoDePistolas {

	class LocalMultiplayerManager : public LocoMotor::Component
	{
	public:
		LocalMultiplayerManager();
		~LocalMultiplayerManager();
	protected:
		void start() override;
		void update(float dT) override;
		void setParameters(std::vector<std::pair<std::string, std::string>>& params) override;

	private:

		struct PlayerData {
			Input::InputManager::ControllerId controllerId;
			PlayerController* playerController;
			GameObject* gameObject;
		};

		std::array<LMVector3, 4> spawnPoints;

		// Stores all the player references by their controllerId
		std::array<PlayerData, 4> allPlayers;
	};
}



