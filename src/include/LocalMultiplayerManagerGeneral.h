#pragma once
#include "Component.h"
#include <unordered_map>
#include "InputManager.h"
#include "LMVector.h"
#include "PlayerController.h"
#include <utility>


using namespace LocoMotor;

namespace JuegoDePistolas {

	class LocalMultiplayerManagerGeneral
	{
	public:
		LocalMultiplayerManagerGeneral();
		~LocalMultiplayerManagerGeneral();

		static LocalMultiplayerManagerGeneral* GetInstance();

		void init();

		void onControllersChange();

		std::array<Input::InputManager::ControllerId, 4> getPlayers();

	private:

		static LocalMultiplayerManagerGeneral* _instance;

		// Stores all the player references by their controllerId
		std::array<Input::InputManager::ControllerId, 4> allPlayers;
	};
}



