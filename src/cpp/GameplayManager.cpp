#include "GameplayManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "LMVector.h"
#include "ParticleSystem.h"

#include <iostream>
#include <cmath>

#include "InputManager.h"
#include "MeshRenderer.h"
#include "Weapon.h"
#include "LMInputs.h"
#include "SceneManager.h"
#include "Scene.h"
#include "LocalMultiplayerManager.h"
#include "Spawner.h"

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

		// Resetear variables
		cameraZoom = 0;
		endRoundTime = 0;
		backImage->show();
		for (int i = 0; i < crosses.size(); i++) {

			crosses[i]->show();
			crosses[i]->setDimensions(0, 0);
		}

		//backImage->setDimensions(0, initHeight);
		// Activar animacion de ronda ganada del personaje
		endRoundActive = true;

		// Sumar los puntos al jugador vivo
		scores[winPlayerIndex]++;

		// Actualizar puntos
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
	spawnCharactersProgress = 0;

	// Mover a los personajes a sus sitios de spawneo
	std::array<LocalMultiplayerManager::PlayerData, 4> allPlayers = LocalMultiplayerManager::GetInstance()->getPlayers();
	for (int i = 0; i < allPlayers.size(); i++) {

		LocalMultiplayerManager::PlayerData thisPlayer = allPlayers[i];

		// Realizar cambios solo si es valido el controllerId
		if (thisPlayer.controllerId == Input::InputManager::invalidControllerId())
			continue;

		thisPlayer.gameObject->getComponent<Transform>()->setPosition(spawnPoints[i]);
		thisPlayer.gameObject->getComponent<MeshRenderer>()->setVisible(true);

		// Marcar a los personajes activos como vivos
		playersAlive[i] = true;
	}



}

void GameplayManager::start()
{
	_instance = this;

	// Referencia a la escena actual
	Scene* scene = SceneManager::GetInstance()->getActiveScene();

	// Asignar la referencia a la camara y a la posicion inicial
	camera = scene->getObjectByName("MainCamera")->getComponent<Transform>();
	initCameraPos = camera->getPosition();

	// Asignar escala inicial de los personajes
	//initCharacterScale = LocalMultiplayerManager::GetInstance()->getPlayers()[0].gameObject->getComponent<Transform>()->GetSize().GetX();

	// Definir spawn points para los jugadores
	spawnPoints = spawnPoints = {
		LMVector3{8, 2, 8},
		LMVector3{-8, 2, 8},
		LMVector3{8, 2, -8},
		LMVector3{-8, 2, -8}
	};

	// Referencias a la interfaz 
	backImage = scene->getObjectByName("ScoreBackground")->getComponent<UIImage>();

	crosses[0] = scene->getObjectByName("Cross_01")->getComponent<UIImage>();
	crosses[1] = scene->getObjectByName("Cross_02")->getComponent<UIImage>();
	crosses[2] = scene->getObjectByName("Cross_03")->getComponent<UIImage>();

	// Inicializar valores
	initScoreBackWidth = backImage->getWidth();
	initScoreBackHeight = backImage->getHeight();
	initCrossSize = crosses[0]->getWidth();

	backImage->hide();
	for (int i = 0; i < crosses.size(); i++)
		crosses[i]->hide();
}

void GameplayManager::update(float dT)
{
	// DEBUG

	if (Input::InputManager::GetInstance()->GetKeyDown(Input::LMScanCode::LMKS_P))
		startRound();


	// Si se esta en la animacion de inicio de ronda
	if (startRoundActive) {

		// Los personajes spawnean en en el primer segundo

		if (startRoundTime < startRoundMaxTime)
			startRoundTime += dT / 1000;
		else
			startRoundTime = startRoundMaxTime;

		if (startRoundTime > startRoundMaxTime)
			startRoundActive = false;

		// Si se esta en el primer segundo de animacion
		if (startRoundTime < 1) {

			spawnCharactersProgress += dT / 1000;
			if (spawnCharactersProgress > 1)
				spawnCharactersProgress = 1;

			LMVector3 startRotation = LMVector3(0, 0, 0);
			LMVector3 endRotation = LMVector3(0, 360, 0);
			LMVector3 currentCharacterRotation = LMVector3::lerp(startRotation, endRotation, spawnCharactersProgress);

			// Mover a los personajes a sus sitios de spawneo
			std::array<LocalMultiplayerManager::PlayerData, 4> allPlayers = LocalMultiplayerManager::GetInstance()->getPlayers();
			for (int i = 0; i < 4; i++) {

				LocalMultiplayerManager::PlayerData thisPlayer = allPlayers[i];

				if (thisPlayer.controllerId != Input::InputManager::GetInstance()->invalidControllerId()) {

					thisPlayer.gameObject->getComponent<Transform>()->setRotation(currentCharacterRotation);

					LMVector3 startPosition = spawnPoints[i] + LMVector3(0, -9, 0);
					LMVector3 endPosition = spawnPoints[i];
					LMVector3 currentCharacterPosition = LMVector3::lerp(startPosition, endPosition, spawnCharactersProgress);

					thisPlayer.gameObject->getComponent<Transform>()->setPosition(currentCharacterPosition);
				}
			}
		}
		else {

			// Mostrar cuenta atras de 3 a 0
			int currentTimerState = floorf(3 - startRoundTime - 1);
			//std::cout << "TIMER = " << currentTimerState << std::endl;
		}

		//std::cout << "spawnCharactersProgress = " << spawnCharactersProgress << std::endl;
	}


	// Si esta durante una animacion de ronda ganada, mover la camara
	if (endRoundActive) {

		// Tiempo maximo de ronda
		if (endRoundTime < endRoundMaxTime)
			endRoundTime += dT / 1000;
		else {
			endRoundTime = endRoundMaxTime;

			// Terminar animacion de final de ronda
			endRoundActive = false;
			startRound();
		}

		// Actualizar animaciones
		updateCameraAnimations(dT);
		updateCrossAnimations();
		updateBackScoreAnimations();


		//std::cout << "endRoundTime = " << endRoundTime << std::endl;
		//std::cout << "cameraZoom = " << cameraZoom << std::endl;
	}
}

void JuegoDePistolas::GameplayManager::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{


}

void JuegoDePistolas::GameplayManager::updateCameraAnimations(float dT)
{
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
		getPlayers()[winPlayerIndex].gameObject->getComponent<Transform>()->getPosition();
	LMVector3 closeToCharacterPos = characterPosition + LMVector3(0, 30, 30);
	LMVector3 currentCameraPosition = LMVector3::lerp(initCameraPos, closeToCharacterPos, cameraZoom);

	camera->setPosition(currentCameraPosition);
}

void JuegoDePistolas::GameplayManager::updateBackScoreAnimations()
{
	float scoreBack_init_time = 1;
	float scoreBack_init_duration = .5f;

	float scoreBack_end_time = 3.5f;
	float scoreBack_end_duration = .5f;

	if (endRoundTime > scoreBack_init_time && endRoundTime < scoreBack_init_time + scoreBack_init_duration) {

		// Actualizar UI
		float startValue = 0;
		float endValue = initScoreBackWidth;
		float t = (endRoundTime - scoreBack_init_time) / scoreBack_init_duration;
		int currentWidth = lerp(startValue, endValue, t);
		backImage->setDimensions(currentWidth, initScoreBackHeight);
	}
	else if (endRoundTime > scoreBack_end_time && endRoundTime < scoreBack_end_time + scoreBack_end_duration) {

		// Actualizar UI
		float startValue = initScoreBackWidth;
		float endValue = 0;
		float t = (endRoundTime - scoreBack_end_time) / scoreBack_end_duration;
		int currentWidth = lerp(startValue, endValue, t);
		backImage->setDimensions(currentWidth, initScoreBackHeight);
	}
	// Desde que se inicia hasta que termina
	else if (endRoundTime >= scoreBack_init_time + scoreBack_init_duration && endRoundTime <= scoreBack_end_time)
	{
	}
	// Si es antes de la animacion init o despues de la animacion end, esconderlo
	else
		backImage->setDimensions(0, initScoreBackHeight);
}

void JuegoDePistolas::GameplayManager::updateCrossAnimations()
{
	// Todas las cruces escalandose al principio de la animacion
	float allCrosses_init_time = 1;
	float allCrosses_init_duration = .5f;

	// Animacion de la cruz decreciendo, cambiando de color, y volviendo a crecer
	// En la duracion se incluye todo este proceso
	float winCross_time = 2;
	float winCross_duration = .75f;

	// Todas las cruces decreciendo al final de la animacion
	float allCrosses_end_time = 3.5f;
	float allCrosses_end_duration = .5f;


	// Animacion de todas las cruces creciendo
	if (endRoundTime > allCrosses_init_time && endRoundTime < allCrosses_init_time + allCrosses_init_duration) {

		// Animacion tamaño
		float startValue_size = 0;
		float endValue_size = initCrossSize;
		float t_size = (endRoundTime - allCrosses_init_time) / allCrosses_init_duration;
		int currentSize = lerp(startValue_size, endValue_size, t_size);

		for (int i = 0; i < crosses.size(); i++)
			crosses[i]->setDimensions(currentSize, currentSize);
	}

	// Animacion de la cruz del punto decreciendo, cambiando de color y volviendo a crecer
	else if (endRoundTime > winCross_time && endRoundTime < winCross_time + winCross_duration) {

		int crossIndex = 0;
		UIImage* winCross = crosses[crossIndex];

		// Valor de 0 a 1 que define esta fragmento de tiempo
		float totalT = (endRoundTime - winCross_time) / winCross_duration;

		// Si se pasa de la mitad, cambiar de color
		if (totalT >= .5f)
			winCross->setImage("CrossMaterialBlue");

		// Primera parte de la animacion
		if (totalT < .5f) {

			float firstT = totalT / 0.5f;
			float startValue_size = initCrossSize;
			float endValue_size = 0;
			int currentSize = lerp(startValue_size, endValue_size, firstT);
			crosses[crossIndex]->setDimensions(currentSize, currentSize);
		}
		// Segunda parte de la animacion
		else {
			float secondT = (totalT - .5f) / 0.5f;
			float startValue_size = 0;
			float endValue_size = initCrossSize;
			int currentSize = lerp(startValue_size, endValue_size, secondT);
			crosses[crossIndex]->setDimensions(currentSize, currentSize);
		}

		std::cout << "totalT = " << totalT << std::endl;
	}

	// Animacion de todas las cruces decreciendo
	else if (endRoundTime > allCrosses_end_time && endRoundTime < allCrosses_end_time + allCrosses_end_duration) {

		// Actualizar UI
		float startValue_size = initCrossSize;
		float endValue_size = 0;
		float t_size = (endRoundTime - allCrosses_end_time) / allCrosses_end_duration;
		int currentSize = lerp(startValue_size, endValue_size, t_size);

		for (int i = 0; i < crosses.size(); i++)
			crosses[i]->setDimensions(currentSize, currentSize);
	}
}

float JuegoDePistolas::GameplayManager::lerp(float a, float b, float t)
{
	return a + t * (b - a);
}


void JuegoDePistolas::GameplayManager::freeSpawnpoint(int spawnId) {
	Spawner* weaponspawnpoints = this->_gameObject->getComponent<Spawner>();
	if (weaponspawnpoints != nullptr) {
		weaponspawnpoints->setSpawnerAvailable(spawnId, true);
	}
}

std::array<LMVector3, 4> JuegoDePistolas::GameplayManager::getSpawnPoints()
{
	return spawnPoints;
}