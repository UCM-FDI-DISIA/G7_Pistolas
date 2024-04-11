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
#include "LocalMultiplayerManager.h"

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

	// Comprobar si la ronda ha terminado
	// Contar el numero de jugadores que quedan vivos en la ronda
	int numPlayersAlive = 0;
	winPlayerIndex = -1;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (playersAlive[i]) {
			numPlayersAlive++;
			winPlayerIndex = i;
		}
	}

	std::cout << "Player " << playerIndex << " Died, " << numPlayersAlive << " players alive" << std::endl;

	// Si solo hay un jugador vivo
	if (numPlayersAlive == 1) {

		// Resetear el zoom de camara
		cameraZoom = 0;
		endRoundTime = 0;

		// Activar animacion de ronda ganada del personaje
		endRoundActive = true;

		// onlyPlayerAliveIndex;

		std::cout << "PLAYER WIN index = " << winPlayerIndex << std::endl;
	}

	// Desactivar el personaje que acaba de ser eliminado
	LocalMultiplayerManager::GetInstance()->
		getPlayers()[playerIndex].gameObject->getComponent<MeshRenderer>()->setVisible(false);
}

bool JuegoDePistolas::GameplayManager::isPlayerAlive(int playerIndex)
{
	return playersAlive[playerIndex];
}

void GameplayManager::startRound()
{
	// Actualizar variables
	startRoundActive = true;
	startRoundTime = 0;

	// Mover a los personajes a sus sitios de spawneo
}

void GameplayManager::start()
{
	_instance = this;

	// Asignar la referencia a la camara y a la posicion inicial
	camera = SceneManager::GetInstance()->getActiveScene()->
		getObjectByName("MainCamera")->getComponent<Transform>();
	initCameraPos = camera->GetPosition();
}

void GameplayManager::update(float dT)
{
	// Si se esta en la animacion de inicio de ronda
	if (startRoundActive) {

		// Los personajes spawnean en en el primer segundo

	}

	// Si esta durante una animacion de ronda ganada, mover la camara
	if (endRoundActive) {

		// Tiempo maximo de ronda
		if (endRoundTime < endRoundMaxTime)
			endRoundTime += dT / 1000;
		else
			endRoundTime = endRoundMaxTime;


		// Comprobar si el endRoundTime esta en el ultimo segundo de animacion
		if (endRoundTime > endRoundMaxTime - 1)
			cameraZoom = endRoundMaxTime - endRoundTime;

		// Si no esta en el ultimo segundo de animacion, aplicar camerazoom
		else {

			if (cameraZoom < 1)
				cameraZoom += dT / 1000;
			else
				cameraZoom = 1;
		}

		// Actualizar posicion de camara
		LMVector3 characterPosition = LocalMultiplayerManager::GetInstance()->
			getPlayers()[winPlayerIndex].gameObject->getComponent<Transform>()->GetPosition();
		LMVector3 closeToCharacterPos = characterPosition + LMVector3(0, 300, 300);
		LMVector3 currentCameraPosition = LMVector3::Lerp(initCameraPos, closeToCharacterPos, cameraZoom);

		camera->SetPosition(currentCameraPosition);



		std::cout << "endRoundTime = " << endRoundTime << std::endl;
		std::cout << "cameraZoom = " << cameraZoom << std::endl;
	}
}

void JuegoDePistolas::GameplayManager::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

