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

		static LocalMultiplayerManager* GetInstance();

		struct PlayerData {
			Input::InputManager::ControllerId controllerId;
			PlayerController* playerController;
			GameObject* gameObject;
		};

		std::array<PlayerData, 4> getPlayers();

		int getNumPlayersConnected();

	protected:
		void awake() override;
		void start() override;
		void update(float dT) override;
		bool setParameters(std::vector<std::pair<std::string, std::string>>& params) override;

	private:

		static LocalMultiplayerManager* _instance;

		// Stores all the player references by their controllerId
		std::array<PlayerData, 4> allPlayers;

		// The positions where players will spawn
		std::array<LMVector3, 4> spawnPoints;
	};
}



