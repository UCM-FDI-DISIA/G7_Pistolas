#include "GameplayManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "LMVector.h"
#include "ParticleSystem.h"

#include <iostream>
#include <cmath>

#include "InputManager.h"
#include "MeshRenderer.h"
#include "LMInputs.h"
#include "SceneManager.h"
#include "Scene.h"

using namespace JuegoDePistolas;
using namespace LocoMotor;

GameplayManager::GameplayManager()
{

}

GameplayManager::~GameplayManager()
{
}

void JuegoDePistolas::GameplayManager::playerDied(int playerIndex)
{
	playersAlive[playerIndex] = false;

	// Comprobar si la ronda ha terminado
	// Contar el numero de jugadores que quedan vivos en la ronda
	int numPlayersAlive = 0;
	int onlyPlayerAliveIndex = -1;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (playersAlive[i]) {
			numPlayersAlive++;
			onlyPlayerAliveIndex = i;
		}
	}

	// Si solo hay un jugador vivo
	if (numPlayersAlive == 1) {

		// Animacion de ronda ganada del personaje
		//onlyPlayerAliveIndex;
	}
}


void GameplayManager::start()
{
}

void GameplayManager::update(float dT)
{
}

void JuegoDePistolas::GameplayManager::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

