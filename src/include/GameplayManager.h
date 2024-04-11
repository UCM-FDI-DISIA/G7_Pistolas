#pragma once
#include "Component.h"
#include "InputManager.h"
#include "LMVector.h"

using namespace LocoMotor;

namespace JuegoDePistolas {

	class GameplayManager :public LocoMotor::Component
	{
	public:
		GameplayManager();
		~GameplayManager();

		static GameplayManager* GetInstance();

		void playerDied(int playerIndex);

	protected:
		void start() override;
		void update(float dT) override;
		void setParameters(std::vector<std::pair<std::string, std::string>>& params) override;

	private:

		static GameplayManager* _instance;


		const int MAX_PLAYERS = 4;

		// Almacena las puntuaciones actuales de todos los jugadores
		std::array<int, 4> scores = { 0, 0, 0, 0 };

		// Los jugadores que siguen vivos en esta ronda
		std::array<bool, 4> playersAlive = { false, false, false, false };
	};
}



