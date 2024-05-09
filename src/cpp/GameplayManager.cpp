#include "GameplayManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "LMVector.h"
#include "ParticleSystem.h"
#include "EventEmitter.h"

#include <iostream>
#include <cmath>

#include "InputManager.h"
#include "MeshRenderer.h"
#include "Weapon.h"
#include "LMInputs.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Spawner.h"
#include "Engine.h"

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
	// Si es duante la animacion de ronda final, no contar la muerte
	if (endRoundActive)
		return;

	// Si solo queda un jugador, no contar la muerte
	if (localMultiplayerManager != nullptr && localMultiplayerManager->getNumPlayersConnected() <= 1)
		return;

	if (playersAlive.size() > playerIndex)
		playersAlive[playerIndex] = false;

	// Comprobar si la ronda ha terminado
	// Contar el numero de jugadores que quedan vivos en la ronda
	int numPlayersAlive = 0;
	winPlayerIndex = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (playersAlive.size() > i && playersAlive[i]) {
			numPlayersAlive++;
			winPlayerIndex = i;
		}
	}

	std::cout << "Player " << playerIndex << " Died, " << numPlayersAlive << " players alive" << std::endl;

	Scene* activeScene = SceneManager::GetInstance()->getActiveScene();

	GameObject* bgMusic = nullptr;
	GameObject* deathSound = nullptr;
	GameObject* winSound = nullptr;

	if (activeScene != nullptr) {
		bgMusic = activeScene->getObjectByName("Emitter");
		deathSound = activeScene->getObjectByName("EmitterDeath");
		winSound = activeScene->getObjectByName("EmitterWin");

		if (bgMusic != nullptr && bgMusic->getComponent<EventEmitter>() != nullptr)
			bgMusic->getComponent<EventEmitter>()->setParameter("PlayersAlive", numPlayersAlive);

		if (deathSound != nullptr && deathSound->getComponent<EventEmitter>() != nullptr)
			deathSound->getComponent<EventEmitter>()->play();
	}

	// Si solo hay un jugador vivo
	if (numPlayersAlive == 1) {

		// Resetear variables
		cameraZoom = 0;
		endRoundTime = 0;

		if (backImage != nullptr)
			backImage->show();

		for (int i = 0; i < crosses.size(); i++) {

			if (crosses[i] == nullptr)
				continue;

			crosses[i]->show();
			crosses[i]->setDimensions(0, 0);

			// Tintar las cruces equivalentes a los puntos que tenga acumulados el juegador ganador de la ronda
			if (scores.size() > winPlayerIndex && i < scores[winPlayerIndex])
				crosses[i]->setImage(getMaterialFromPlayerIndex(winPlayerIndex));

			// Poner el resto de cruces blancas
			else
				crosses[i]->setImage("CrossMaterial");

			if (bgMusic != nullptr && bgMusic->getComponent<EventEmitter>() != nullptr)
				bgMusic->getComponent<EventEmitter>()->stop();

			if (winSound != nullptr && winSound->getComponent<EventEmitter>() != nullptr)
				winSound->getComponent<EventEmitter>()->play();
		}

		//backImage->setDimensions(0, initHeight);
		// Activar animacion de ronda ganada del personaje
		endRoundActive = true;

		// Sumar los puntos al jugador vivo
		if (scores.size() > winPlayerIndex)
			scores[winPlayerIndex]++;

		// Comprobar si este jugador ha llegado al score maximo
		if (scores.size() > winPlayerIndex &&
			scores[winPlayerIndex] >= winScore) {

			// Indicar a la logica que la partida ya ha terminado
			matchEnd = true;

			// Actualizar el texto con el ganador de la partida
			if (playerColorsName.size() > winPlayerIndex) {
				std::string message = playerColorsName[winPlayerIndex] + " WINS";

				if (winText != nullptr) {
					winText->setText(message);
					winText->setColor(playerColors[winPlayerIndex]);
				}
				if (winTextShade != nullptr)
					winTextShade->setText(message);
			}

			if (bgMusic->getComponent<EventEmitter>() != nullptr)
				bgMusic->getComponent<EventEmitter>()->stop();

			if (winSound->getComponent<EventEmitter>() != nullptr)
				winSound->getComponent<EventEmitter>()->play();

			// Debug
			std::cout << "PLAYER WIN index = " << winPlayerIndex << std::endl;
		}
	}

	// Desactivar el personaje que acaba de ser eliminado
	if (localMultiplayerManager != nullptr) {

		std::array<LocalMultiplayerManager::PlayerData, 4> allPlayers = localMultiplayerManager->getPlayers();

		if (allPlayers.size() > playerIndex) {

			GameObject* thisPlayerObj = allPlayers[playerIndex].gameObject;

			if (thisPlayerObj != nullptr) {
				MeshRenderer* thisPlayerMeshRenderer = thisPlayerObj->getComponent<MeshRenderer>();
				if (thisPlayerMeshRenderer != nullptr)
					thisPlayerMeshRenderer->setVisible(false);
			}
		}
	}
	//LocalMultiplayerManager::GetInstance()->
	//	getPlayers()[playerIndex].gameObject->getComponent<MeshRenderer>()->setVisible(false);
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
	std::array<LocalMultiplayerManager::PlayerData, 4> allPlayers;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int size = allPlayers.size();
	if (localMultiplayerManager == nullptr) {
		Engine::GetInstance()->showWindow(2, "Missing LocalMultiplayerManager component");
		Engine::GetInstance()->quit();
		return;
	}
	if (localMultiplayerManager != nullptr) {
		allPlayers = localMultiplayerManager->getPlayers();
	}

	for (int i = 0; i < allPlayers.size(); i++) {

		LocalMultiplayerManager::PlayerData thisPlayer = allPlayers[i];

		// Realizar cambios solo si es valido el controllerId
		if (thisPlayer.controllerId == Input::InputManager::invalidControllerId())
			continue;

		// Marcar a los personajes activos como vivos
		playersAlive[i] = true;

		if (thisPlayer.playerController != nullptr) {
			thisPlayer.playerController->setPlayerActive(true);
			thisPlayer.playerController->setCanMove(false);
			thisPlayer.playerController->dropWeapon();
		}
	}

	int numPlayersAlive = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (playersAlive.size() > i && playersAlive[i]) {
			numPlayersAlive++;
			winPlayerIndex = i;
		}
	}

	// Borrar las armas que haya por el suelo sin recogen
	GameplayManager* gameplayManager = GameplayManager::GetInstance();
	if (gameplayManager != nullptr) {

		GameObject* gameplayManagerObj = gameplayManager->_gameObject;

		if (gameplayManagerObj != nullptr) {
			Spawner* spawner = gameplayManagerObj->getComponent<Spawner>();
			if (spawner != nullptr)
				spawner->startRound();
		}
	}

	Scene* activeScene = SceneManager::GetInstance()->getActiveScene();
	if (activeScene != nullptr) {
		GameObject* winMusic = activeScene->getObjectByName("EmitterWin");
		if (winMusic != nullptr && winMusic->getComponent<EventEmitter>() != nullptr)
			winMusic->getComponent<EventEmitter>()->stop();

		GameObject* bgMusic = SceneManager::GetInstance()->getActiveScene()->getObjectByName("Emitter");
		if (bgMusic != nullptr && bgMusic->getComponent<EventEmitter>() != nullptr) {
			bgMusic->getComponent<EventEmitter>()->play();
			bgMusic->getComponent<EventEmitter>()->setParameter("PlayersAlive", numPlayersAlive);
		}
	}

	cdEmit = false;
}

void GameplayManager::start()
{
	_instance = this;

	// Asignar referencia al multiplayerManager
	localMultiplayerManager = LocalMultiplayerManager::GetInstance();

	// Referencia a la escena actual
	Scene* activeScene = SceneManager::GetInstance()->getActiveScene();
	if (activeScene == nullptr)
		return;

	// Asignar la referencia a la camara y a la posicion inicial
	GameObject* cameraObj = activeScene->getObjectByName("MainCamera");
	if (cameraObj != nullptr) {

		camera = cameraObj->getComponent<Transform>();
		if (camera != nullptr)
			initCameraPos = camera->getPosition();
	}


	// Definir spawn points para los jugadores
	GameObject* CharacterSpawnpoint_1 = activeScene->getObjectByName("CharacterSpawnpoint_1");
	GameObject* CharacterSpawnpoint_2 = activeScene->getObjectByName("CharacterSpawnpoint_2");
	GameObject* CharacterSpawnpoint_3 = activeScene->getObjectByName("CharacterSpawnpoint_3");
	GameObject* CharacterSpawnpoint_4 = activeScene->getObjectByName("CharacterSpawnpoint_4");

	if (CharacterSpawnpoint_1 != nullptr && CharacterSpawnpoint_2 != nullptr
		&& CharacterSpawnpoint_3 != nullptr && CharacterSpawnpoint_4 != nullptr) {

		Transform* CharacterSpawnpoint_1_transform = CharacterSpawnpoint_1->getComponent<Transform>();
		Transform* CharacterSpawnpoint_2_transform = CharacterSpawnpoint_2->getComponent<Transform>();
		Transform* CharacterSpawnpoint_3_transform = CharacterSpawnpoint_3->getComponent<Transform>();
		Transform* CharacterSpawnpoint_4_transform = CharacterSpawnpoint_4->getComponent<Transform>();

		if (CharacterSpawnpoint_1_transform != nullptr && CharacterSpawnpoint_2_transform != nullptr
			&& CharacterSpawnpoint_3_transform != nullptr && CharacterSpawnpoint_4_transform != nullptr) {

			spawnPoints = {
				CharacterSpawnpoint_1_transform->getPosition(),
				CharacterSpawnpoint_2_transform->getPosition(),
				CharacterSpawnpoint_3_transform->getPosition(),
				CharacterSpawnpoint_4_transform->getPosition(),
			};
		}
	}

	// Asignar colores de los jugadores
	playerColors = {
		LMVector3{0.3294f, 0.7372f, 0.8352f},
		LMVector3{0.8078f, 0.2745f, 0.2745f},
		LMVector3{0.3764f, 0.8078f, 0.2784f},
		LMVector3{0.7137f, 0.4078f, 0.8352f}
	};

	// Asignar nombres de colores de los jugadores
	playerColorsName = {
		"BLUE", "RED", "GREEN", "PURPLE"
	};

	GameObject* UIPlayerWinObj = activeScene->getObjectByName("UIPlayerWin");
	if (UIPlayerWinObj != nullptr) {
		winText = UIPlayerWinObj->getComponent<UIText>();
		if (winText != nullptr) {
			winTextY = winText->getPositionY();
			// Ocultar textos
			winText->setText("");
			// Situar el texto en la posicion inicio de la animacion
			winText->setPosition(winText->getPositionX(), -500);
		}
	}
	GameObject* UIPlayerWinShadeObj = activeScene->getObjectByName("UIPlayerWinShade");
	if (UIPlayerWinShadeObj != nullptr) {
		winTextShade = UIPlayerWinShadeObj->getComponent<UIText>();
		if (winTextShade != nullptr) {
			// Ocultar textos
			winTextShade->setText("");
			// Situar el texto en la posicion inicio de la animacion
			winTextShade->setPosition(winText->getPositionX(), -500);
		}
	}

	// Referencias a la interfaz
	GameObject* scoreBackgroundObj = activeScene->getObjectByName("ScoreBackground");
	if (scoreBackgroundObj != nullptr)
		backImage = scoreBackgroundObj->getComponent<UIImage>();

	if (crosses.size() > 2) {
		GameObject* cross_01Obj = activeScene->getObjectByName("Cross_01");
		if (cross_01Obj != nullptr)
			crosses[0] = cross_01Obj->getComponent<UIImage>();

		GameObject* cross_02Obj = activeScene->getObjectByName("Cross_02");
		if (cross_02Obj != nullptr)
			crosses[1] = cross_02Obj->getComponent<UIImage>();

		GameObject* cross_03Obj = activeScene->getObjectByName("Cross_03");
		if (cross_03Obj != nullptr)
			crosses[2] = cross_03Obj->getComponent<UIImage>();
	}

	// Inicializar valores
	if (backImage != nullptr) {
		initScoreBackWidth = backImage->getWidth();
		initScoreBackHeight = backImage->getHeight();
		backImage->hide();
	}

	if (crosses.size() > 0 && crosses[0] != nullptr)
		initCrossSize = crosses[0]->getWidth();

	for (int i = 0; i < crosses.size(); i++)
		if (crosses[i] != nullptr)
			crosses[i]->hide();

	// Referencia al texto countdown
	GameObject* UICountdownObj = activeScene->getObjectByName("UICountdown");
	if (UICountdownObj != nullptr) {
		countdownText = UICountdownObj->getComponent<UIText>();
		if (countdownText != nullptr)
			countdownText->setText("");
	}

	GameObject* UICountdownShadowObj = activeScene->getObjectByName("UICountdownShadow");
	if (UICountdownShadowObj != nullptr) {
		countdownTextShadow = UICountdownShadowObj->getComponent<UIText>();
		if (countdownTextShadow != nullptr)
			countdownTextShadow->setText("");
	}

	GameObject* cdSound = activeScene->getObjectByName("EmitterCD");

	if (cdSound != nullptr && cdSound->getComponent<EventEmitter>() != nullptr)
		cdSound->getComponent<EventEmitter>()->stop();

	startRound();
}

void GameplayManager::update(float dT)
{
	if (localMultiplayerManager == nullptr)
		return;

	// Si no hay jugadores conectados, no actualizar la logica
	if (localMultiplayerManager->getNumPlayersConnected() <= 0)
		return;

	//if (Input::InputManager::GetInstance()->GetKeyDown(Input::LMKS_8)) {
	//	SceneManager::GetInstance()->loadScene("Assets/Scenes/Menu.lua", "Menu");
	//	SceneManager::GetInstance()->changeScene("Menu");
	//}

	//if (Input::InputManager::GetInstance()->GetKeyDown(Input::LMScanCode::LMKS_P))
	//	startRound();

	// Si se esta en la animacion de inicio de ronda
	if (startRoundActive) {

		// Los personajes spawnean en en el primer segundo
		if (startRoundTime < startRoundMaxTime)
			startRoundTime += dT / 1000;
		else {
			// Detectar el final de la animacion de inicio de ronda
			startRoundTime = startRoundMaxTime;
			startRoundActive = false;

			// Habilitar control a los personsajes para que se puedan mover
			std::array<LocalMultiplayerManager::PlayerData, 4> allPlayers = localMultiplayerManager->getPlayers();
			for (int i = 0; i < allPlayers.size(); i++) {

				LocalMultiplayerManager::PlayerData thisPlayer = allPlayers[i];

				// Realizar cambios solo si es valido el controllerId
				if (thisPlayer.controllerId == Input::InputManager::invalidControllerId())
					continue;

				if (thisPlayer.playerController != nullptr)
					thisPlayer.playerController->setCanMove(true);
			}
		}

		float spawnCharactersDuration = 1;

		// Si se esta en el primer segundo de animacion
		if (startRoundTime < spawnCharactersDuration) {

			spawnCharactersProgress += dT / 1000;
			if (spawnCharactersProgress > spawnCharactersDuration)
				spawnCharactersProgress = spawnCharactersDuration;

			float t = spawnCharactersProgress / spawnCharactersDuration;

			LMVector3 startRotation = LMVector3(0, 0, 0);
			LMVector3 endRotation = LMVector3(0, 360, 0);
			LMVector3 currentCharacterRotation = LMVector3::lerp(startRotation, endRotation, t);

			// Mover a los personajes a sus sitios de spawneo
			std::array<LocalMultiplayerManager::PlayerData, 4> allPlayers = localMultiplayerManager->getPlayers();
			for (int i = 0; i < MAX_PLAYERS; i++) {

				if (allPlayers.size() <= i)
					return;

				LocalMultiplayerManager::PlayerData thisPlayer = allPlayers[i];

				if (thisPlayer.controllerId != Input::InputManager::GetInstance()->invalidControllerId()) {

					if (thisPlayer.gameObject != nullptr) {

						Transform* thisPlayerTr = thisPlayer.gameObject->getComponent<Transform>();

						if (thisPlayerTr != nullptr) {
							thisPlayerTr->setRotation(currentCharacterRotation.asRotToQuaternion());

							if (spawnPoints.size() > i) {
								LMVector3 startPosition = spawnPoints[i] + LMVector3(0, -6, 0);
								LMVector3 endPosition = spawnPoints[i];
								LMVector3 currentCharacterPosition = LMVector3::lerp(startPosition, endPosition, t);

								thisPlayerTr->setPosition(currentCharacterPosition);
							}
						}
					}
				}
			}
		}


		// Mostrar cuenta atras de 3 a 1 segundos y despues GO
		float countdownStart = 1;
		float countdownDuration = 2;

		if (startRoundTime > countdownStart && startRoundTime < countdownStart + countdownDuration) {
			if (!cdEmit) {

				Scene* activeSceme = SceneManager::GetInstance()->getActiveScene();
				if (activeSceme != nullptr) {

					GameObject* cdSound = activeSceme->getObjectByName("EmitterCD");
					if (cdSound != nullptr && cdSound->getComponent<EventEmitter>() != nullptr)
						cdSound->getComponent<EventEmitter>()->play();
				}

				cdEmit = true;
			}
			// valor que va de 0 a 1 que representa esta seccion de tiempo
			float t = (startRoundTime - countdownStart) / countdownDuration;
			//std::cout << "t = " << t << std::endl;

			float eachNumberDuration = 1.f / 4.f;

			// Amarillo
			LMVector3 topTextColor = LMVector3(0.9960f, 0.7882f, 0.0156f);
			LMVector3 bottomTextColor = LMVector3(0.9882f, 0.6627f, 0.0078f);
			std::string currentCountDown = "-1";
			if (t < eachNumberDuration)
				currentCountDown = "3";
			else if (t < eachNumberDuration * 2)
				currentCountDown = "2";
			else if (t < eachNumberDuration * 3)
				currentCountDown = "1";
			else {
				currentCountDown = "GO";

				topTextColor = LMVector3(0.2039f, 0.8117f, 0.2431f);
				bottomTextColor = LMVector3(0.1294f, 0.6274f, 0.1607f);
			}

			if (countdownText != nullptr) {
				countdownText->setColorTop(topTextColor);
				countdownText->setColorBottom(bottomTextColor);

				countdownText->setText(currentCountDown);
			}

			if (countdownTextShadow != nullptr)
				countdownTextShadow->setText(currentCountDown);
		}
		else {
			if (countdownText != nullptr)
				countdownText->setText("");

			if (countdownTextShadow != nullptr)
				countdownTextShadow->setText("");
		}
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

bool JuegoDePistolas::GameplayManager::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
	return true;
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

	// Acceder a la posicion del jugador
	LMVector3 characterPosition = LMVector3(0, 0, 0);
	if (localMultiplayerManager != nullptr) {
		std::array<LocalMultiplayerManager::PlayerData, 4> allPlayers = localMultiplayerManager->getPlayers();
		// Comprobar si hay suficientes jugadores en el array antes de acceder a el
		if (allPlayers.size() > winPlayerIndex) {
			GameObject* thisPlayerObj = allPlayers[winPlayerIndex].gameObject;
			if (thisPlayerObj != nullptr) {
				Transform* thisPlayerObjTr = thisPlayerObj->getComponent<Transform>();
				if (thisPlayerObjTr != nullptr)
					characterPosition = thisPlayerObjTr->getPosition();
			}
		}
	}

	LMVector3 closeToCharacterPos = characterPosition + LMVector3(0, 30, 30);
	LMVector3 currentCameraPosition = LMVector3::lerp(initCameraPos, closeToCharacterPos, cameraZoom);

	if (camera != nullptr)
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
		if (backImage != nullptr)
			backImage->setDimensions(currentWidth, initScoreBackHeight);
	}
	else if (endRoundTime > scoreBack_end_time && endRoundTime < scoreBack_end_time + scoreBack_end_duration) {

		// Actualizar UI
		float startValue = initScoreBackWidth;
		float endValue = 0;
		float t = (endRoundTime - scoreBack_end_time) / scoreBack_end_duration;
		int currentWidth = lerp(startValue, endValue, t);
		if (backImage != nullptr)
			backImage->setDimensions(currentWidth, initScoreBackHeight);
	}
	// Desde que se inicia hasta que termina
	else if (endRoundTime >= scoreBack_init_time + scoreBack_init_duration && endRoundTime <= scoreBack_end_time)
	{
	}
	// Si es antes de la animacion init o despues de la animacion end, esconderlo
	else {
		if (backImage != nullptr)
			backImage->setDimensions(0, initScoreBackHeight);
	}
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

		for (int i = 0; i < crosses.size(); i++) {
			if (crosses[i] != nullptr)
				crosses[i]->setDimensions(currentSize, currentSize);
		}
	}

	// Animacion de la cruz del punto decreciendo, cambiando de color y volviendo a crecer
	else if (endRoundTime > winCross_time && endRoundTime < winCross_time + winCross_duration) {

		int crossIndex = scores[winPlayerIndex] - 1;
		//for (int i = 0; i < 4; i++)
			//std::cout << "score_" << i << " = " << scores[i] << std::endl;

		//std::cout << "crossIndex = " << crossIndex << std::endl;
		UIImage* winCross = crosses[crossIndex];

		// Valor de 0 a 1 que define esta fragmento de tiempo
		float totalT = (endRoundTime - winCross_time) / winCross_duration;

		// Si se pasa de la mitad, cambiar de color
		if (totalT >= .5f) {
			//winPlayerIndex
			if (winCross != nullptr)
				winCross->setImage(getMaterialFromPlayerIndex(winPlayerIndex));
		}

		// Primera parte de la animacion
		if (totalT < .5f) {

			float firstT = totalT / 0.5f;
			float startValue_size = initCrossSize;
			float endValue_size = 0;
			int currentSize = lerp(startValue_size, endValue_size, firstT);

			if (crosses.size() > crossIndex && crosses[crossIndex] != nullptr)
				crosses[crossIndex]->setDimensions(currentSize, currentSize);
		}
		// Segunda parte de la animacion
		else {
			float secondT = (totalT - .5f) / 0.5f;
			float startValue_size = 0;
			float endValue_size = initCrossSize;
			int currentSize = lerp(startValue_size, endValue_size, secondT);

			if (crosses.size() > crossIndex && crosses[crossIndex] != nullptr)
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

		for (int i = 0; i < crosses.size(); i++) {
			if (crosses.size() > i && crosses[i] != nullptr)
				crosses[i]->setDimensions(currentSize, currentSize);
		}
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
		if (winText != nullptr)
			winText->setPosition(winText->getPositionX(), currentY);

		if (winTextShade != nullptr)
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

	Spawner* weaponspawnpoints = nullptr;

	if (_gameObject != nullptr)
		weaponspawnpoints = _gameObject->getComponent<Spawner>();

	if (weaponspawnpoints != nullptr)
		weaponspawnpoints->setSpawnerAvailable(spawnId, true);
}

std::array<LMVector3, 4> JuegoDePistolas::GameplayManager::getSpawnPoints()
{
	return spawnPoints;
}

bool JuegoDePistolas::GameplayManager::duringStartRoundAnimation()
{
	return startRoundActive;
}
