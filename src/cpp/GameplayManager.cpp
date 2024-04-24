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

			// Tintar las cruces equivalentes a los puntos que tenga acumulados el juegador ganador de la ronda
			if (i < scores[winPlayerIndex])
				crosses[i]->setImage(getMaterialFromPlayerIndex(winPlayerIndex));

			// Poner el resto de cruces blancas
			else
				crosses[i]->setImage("CrossMaterial");
		}

		//backImage->setDimensions(0, initHeight);
		// Activar animacion de ronda ganada del personaje
		endRoundActive = true;

		// Sumar los puntos al jugador vivo
		scores[winPlayerIndex]++;

		// Comprobar si este jugador ha llegado al score maximo
		if (scores[winPlayerIndex] >= winScore) {

			// Indicar a la logica que la partida ya ha terminado
			matchEnd = true;

			// Actualizar el texto con el ganador de la partida
			std::string message = playerColorsName[winPlayerIndex] + " WINS";
			winText->setText(message);
			winTextShade->setText(message);
			winText->setColor(playerColors[winPlayerIndex]);

			// Debug
			std::cout << "PLAYER WIN index = " << winPlayerIndex << std::endl;
		}
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
	spawnPoints = {
		scene->getObjectByName("CharacterSpawnpoint_1")->getComponent<Transform>()->getPosition(),
		scene->getObjectByName("CharacterSpawnpoint_2")->getComponent<Transform>()->getPosition(),
		scene->getObjectByName("CharacterSpawnpoint_3")->getComponent<Transform>()->getPosition(),
		scene->getObjectByName("CharacterSpawnpoint_4")->getComponent<Transform>()->getPosition(),
	};

	// Asignar colores de los jugadores
	playerColors = {
		LMVector3{0.3294f, 0.7372f, 0.8352f},
		LMVector3{0.8078f, 0.2745f, 0.2745f},
		LMVector3{0.3764f, 0.8078f, 0.2784f},
		LMVector3{0.7137f, 0.4078f, 0.8352f}
	};

	// Asignar colores de los jugadores
	playerColorsName = {
		"BLUE", "RED", "GREEN", "PURPLE"
	};

	winText = scene->getObjectByName("UIPlayerWin")->getComponent<UIText>();
	winTextShade = scene->getObjectByName("UIPlayerWinShade")->getComponent<UIText>();
	winTextY = winText->getPositionY();

	// Ocultar textos
	winText->setText("");
	winTextShade->setText("");
	// Situar el texto en la posicion inicio de la animacion
	winText->setPosition(winText->getPositionX(), -500);
	winTextShade->setPosition(winText->getPositionX(), -500);

	//spawnPoints = spawnPoints = {
	//LMVector3{8, 2, 8},
	//LMVector3{-8, 2, 8},
	//LMVector3{8, 2, -8},
	//LMVector3{-8, 2, -8}
	//};

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

	// Referencia al texto countdown
	countdownText = scene->getObjectByName("UICountdown")->getComponent<UIText>();
	countdownText->setText("");

	startRound();
}

void GameplayManager::update(float dT)
{
	// DEBUG

	//if (matchEnd && endRoundTime > 6)
	//{
	//	if (!sceneChanged) {

	//		sceneChanged = true;
	//		SceneManager::GetInstance()->loadScene("Assets/Scenes/Menu.lua", "Menu");
	//		SceneManager::GetInstance()->changeScene("Menu");
	//	}
	//}

	if (Input::InputManager::GetInstance()->GetKeyDown(Input::LMKS_8)) {
		SceneManager::GetInstance()->loadScene("Assets/Scenes/Menu.lua", "Menu");
		SceneManager::GetInstance()->changeScene("Menu");
	}

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


		// Mostrar cuenta atras de 3 a 1 segundos y despues GO
		float countdownStart = 1;
		float countdownDuration = 1.5f;

		if (startRoundTime > countdownStart && startRoundTime < countdownStart + countdownDuration) {

			// valor que va de 0 a 1 que representa esta seccion de tiempo
			float t = (startRoundTime - countdownStart) / countdownDuration;
			std::cout << "t = " << t << std::endl;

			float eachNumberDuration = 0.2;

			std::string currentCountDown = "-1";
			if (t < eachNumberDuration)
				currentCountDown = "3";
			else if (t < eachNumberDuration * 2)
				currentCountDown = "2";
			else if (t < eachNumberDuration * 3)
				currentCountDown = "1";
			else
				currentCountDown = "GO";

			//std::cout << "TIMER = " << currentTimerState << std::endl;
			countdownText->setText(currentCountDown);
		}
		else
			countdownText->setText("");

		//std::cout << "spawnCharactersProgress = " << spawnCharactersProgress << std::endl;
	}


	// Si esta durante una animacion de ronda ganada, mover la camara
	if (endRoundActive) {

		// Si no es la ultima ronda de la partida, en la que ha ganado uno de los jugadores
		// Calcular el tiempo respetando el limite de animacion y llamando a startRound cuando ya esta hecho
		if (!matchEnd) {
			// Tiempo maximo de ronda
			if (endRoundTime < endRoundMaxTime)
				endRoundTime += dT / 1000;
			else {
				endRoundTime = endRoundMaxTime;

				// Terminar animacion de final de ronda
				endRoundActive = false;
				startRound();
			}
		}
		// En la ronda ganadora, simplemente actualizar el valor que mueve las animaciones 
		// hasta llegar al final de la animacion ganadora
		else
			endRoundTime += dT / 1000;

		// Actualizar animaciones
		updateCameraAnimations(dT);
		updateBackScoreAnimations();
		updateCrossAnimations();

		// Solo animar el texto con el nombre del jugador ganador si es el final de partida
		if (matchEnd)
			updateWinText();

		// Si es la ronda ganadora, esperar un rato antes de cambiar de escena de vuelta al menu
		if (matchEnd && endRoundTime > 6)
		{
			if (!sceneChanged) {

				sceneChanged = true;

				SceneManager::GetInstance()->loadScene("Assets/Scenes/Menu.lua", "Menu");
				SceneManager::GetInstance()->changeScene("Menu");
			}
		}
	}
}

void JuegoDePistolas::GameplayManager::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

void JuegoDePistolas::GameplayManager::updateCameraAnimations(float dT)
{
	if (matchEnd) {
		if (cameraZoom < 1)
			cameraZoom += dT / 1000;
		else
			cameraZoom = 1;
	}
	else {
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

		int crossIndex = scores[winPlayerIndex] - 1;
		for (int i = 0; i < 4; i++)
			std::cout << "score_" << i << " = " << scores[i] << std::endl;

		std::cout << "crossIndex = " << crossIndex << std::endl;
		UIImage* winCross = crosses[crossIndex];

		// Valor de 0 a 1 que define esta fragmento de tiempo
		float totalT = (endRoundTime - winCross_time) / winCross_duration;

		// Si se pasa de la mitad, cambiar de color
		if (totalT >= .5f) {
			//winPlayerIndex
			winCross->setImage(getMaterialFromPlayerIndex(winPlayerIndex));
		}

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

void JuegoDePistolas::GameplayManager::updateWinText() {

	float winText_init_time = 1;
	float winText_init_duration = 1;

	if (endRoundTime > winText_init_time && endRoundTime < winText_init_time + winText_init_duration) {

		// Actualizar UI
		float startValue = -500;
		float endValue = winTextY;
		float t = (endRoundTime - winText_init_time) / winText_init_duration;
		int currentY = lerp(startValue, endValue, t);
		winText->setPosition(winText->getPositionX(), currentY);

		winTextShade->setPosition(winText->getPositionX(), currentY + 5);
	}
}

std::string JuegoDePistolas::GameplayManager::getMaterialFromPlayerIndex(int playerIndex)
{
	switch (playerIndex)
	{
	case 0:
		return "CrossMaterialBlue";
		break;
	case 1:
		return "CrossMaterialRed";
		break;
	case 2:
		return "CrossMaterialGreen";
		break;
	case 3:
		return "CrossMaterialPurple";
		break;

	default:
		break;
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