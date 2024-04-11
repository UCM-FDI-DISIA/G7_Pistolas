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

GameplayManager* GameplayManager::_instance = nullptr;

GameplayManager::GameplayManager()
{

}

GameplayManager::~GameplayManager()
{
}

GameplayManager* JuegoDePistolas::GameplayManager::GetInstance()
{
	return _instance;
}

void JuegoDePistolas::GameplayManager::playerDied(int playerIndex)
{
	playersAlive[playerIndex] = false;

	return;

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

	std::cout << "Player " << playerIndex << " Died, " << numPlayersAlive << " players alive" << std::endl;
}

void GameplayManager::start()
{
	_instance = this;
}

void GameplayManager::update(float dT)
{
}

void JuegoDePistolas::GameplayManager::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

