#pragma once
#include "Component.h"
#include "InputManager.h"
#include "Transform.h"
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

		bool isPlayerAlive(int playerIndex);

		void startRound();

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
		std::array<bool, 4> playersAlive = { true, true, false, false };

		// Puntos de spawneo
		std::array<LMVector3, 4> spawnPoints;


		int winPlayerIndex = -1;


		// Valor entre 0 y 1 que se usa para determinar como de cerca debe estar la camara del personaje ganador
		float cameraZoom = 0;

		bool startRoundActive = false;
		float startRoundTime = 0;
		float startRoundMaxTime = 4;

		bool endRoundActive = false;
		float endRoundTime = 0;
		float endRoundMaxTime = 4;



		// Referencia al componente Transform de la camara de la escena
		Transform* camera;
		// Posicion inicial de la camara
		LMVector3 initCameraPos;
	};
}



