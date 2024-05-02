#pragma once
#include "Component.h"
#include "InputManager.h"
#include "Transform.h"
#include "LMVector.h"
#include "UIImage.h"
#include "UIText.h"

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
		void freeSpawnpoint(int spawnId);
		std::array<LMVector3, 4> getSpawnPoints();

	protected:
		void start() override;
		void update(float dT) override;
		void setParameters(std::vector<std::pair<std::string, std::string>>& params) override;

	private:

		void updateCameraAnimations(float dT);
		void updateBackScoreAnimations();
		void updateCrossAnimations();
		void updateWinText();

		std::string getMaterialFromPlayerIndex(int playerIndex);

		float lerp(float a, float b, float t);

		static GameplayManager* _instance;


		const int MAX_PLAYERS = 4;

		// Almacena las puntuaciones actuales de todos los jugadores
		std::array<int, 4> scores = { 0, 0, 0, 0 };

		// Los jugadores que siguen vivos en esta ronda
		std::array<bool, 4> playersAlive = { true, true, false, false };

		// Puntos de spawneo
		std::array<LMVector3, 4> spawnPoints;

		// Colores para los colores de las interfaces
		std::array<LMVector3, 4> playerColors;

		// Colores para los colores de las interfaces
		std::array<std::string, 4> playerColorsName;

		// Referencia a los textos de win y su sombra
		UIText* winText;
		UIText* winTextShade;
		float winTextY;
		float winTextX;

		int winPlayerIndex = -1;
		// Se cambiara a true solo cuando un jugador haya ganado la partida, es decir, al ganar X rondas
		bool matchEnd = false;

		// Numero de rondas necesarias para que un jugador gane la partida
		const int winScore = 3;

		bool sceneChanged = false;

		// Valor entre 0 y 1 que se usa para determinar como de cerca debe estar la camara del personaje ganador
		float cameraZoom = 0;

		// Valor entre 0 y 1 que se usa en la animacion de principio de ronda para las animaciones de los personajes
		// Spawneando
		float spawnCharactersProgress = 0;
		// Escala inicial de los personajes
		int initCharacterScale;

		bool startRoundActive = false;
		float startRoundTime = 0;
		float startRoundMaxTime = 3;

		bool endRoundActive = false;
		float endRoundTime = 0;
		float endRoundMaxTime = 5;

		

		// Referencia al componente Transform de la camara de la escena
		Transform* camera;
		// Posicion inicial de la camara
		LMVector3 initCameraPos;


		UIImage* backImage;
		std::array<UIImage*, 3> crosses;

		int initScoreBackWidth;
		int initScoreBackHeight;

		int initCrossSize;

		UIText* countdownText;

	};
}



