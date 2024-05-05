#include "Weapon.h"
#include "LocalMultiplayerManager.h"
#include "GameplayManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Bullet.h"
#include "MeshRenderer.h"
#include "EventEmitter.h"
#include <string>
#include <Spawner.h>

JuegoDePistolas::Weapon::Weapon()
{
}

JuegoDePistolas::Weapon::~Weapon()
{
}

void JuegoDePistolas::Weapon::shoot(int playerId, int bulletId)
{
	if (_gameObject != nullptr) {

		Transform* weaponTr = _gameObject->getComponent<Transform>();
		if (weaponTr == nullptr)
			return;

		Scene* activeScene = SceneManager::GetInstance()->getActiveScene();
		if (activeScene == nullptr)
			return;

		GameObject* holderPlayerObj = activeScene->getObjectByName(holderPlayerName);
		if (holderPlayerObj == nullptr)
			return;

		Transform* holderPlayerTr = holderPlayerObj->getComponent<Transform>();
		if (holderPlayerTr == nullptr)
			return;



		std::string bulletName = "Bullet" + std::to_string(playerId) + std::to_string(bulletId);

		GameObject* nBullet = activeScene->addGameobjectRuntime(bulletName);

		if (nBullet != nullptr) {

			Bullet* bullComp = (Bullet*)nBullet->addComponent("Bullet");
			Transform* transfComp = (Transform*)nBullet->addComponent("Transform");
			MeshRenderer* meshComp = (MeshRenderer*)nBullet->addComponent("MeshRenderer");
			EventEmitter* emitComp = (EventEmitter*)nBullet->addComponent("EventEmitter");

			if (bullComp != nullptr) {

				bullComp->setBulletActive(true);
				bullComp->setVelocity(40);
				bullComp->setDirection(holderPlayerTr->getRotation().forward() * -1);
			}

			if (transfComp != nullptr) {
				transfComp->setPosition({ weaponTr->getPosition().getX(),weaponTr->getPosition().getY(),weaponTr->getPosition().getZ() });
				transfComp->setSize({ 2, 2, 2 });
				transfComp->setRotation(holderPlayerTr->getRotation());
			}

			if (meshComp != nullptr) {
				meshComp->setMesh("Bullet.mesh");
				meshComp->setMaterial("Bullet");
				meshComp->setVisible(true);
				meshComp->setEnabled(true);
			}

			if (emitComp != nullptr) {
				emitComp->setEvent("event:/Shot");
				emitComp->play();
			}

			ammo--;
		}
	}
}

void JuegoDePistolas::Weapon::start()
{
	if (_gameObject != nullptr) {

		tr = _gameObject->getComponent<Transform>();

		if (tr != nullptr) {

			initialPosY = tr->getPosition().getY();

			Scene* activeScene = SceneManager::GetInstance()->getActiveScene();

			if (activeScene != nullptr) {

				GameObject* shadowLine = activeScene->addGameobjectRuntime("shadowLine_" + _gameObject->getName());

				if (shadowLine != nullptr) {

					// Guardarse una referencia al transform de la sombra
					shadowLineTr = (Transform*)shadowLine->addComponent("Transform");

					MeshRenderer* shadowLineMesh = (MeshRenderer*)shadowLine->addComponent("MeshRenderer");

					if (shadowLineMesh != nullptr) {

						shadowLineMesh->setMesh("ShadowLine.mesh");
						shadowLineMesh->setMaterial("ShadowLineWhite");
					}

					rotationSpeed = rotationSpeedSpawning;
				}
			}
		}
	}
}

void JuegoDePistolas::Weapon::update(float dT)
{

	if (!isPicked) { // El arma esta en el mapa esperando a ser recogida

		// Animacion de spawn
		if (spawnAnimationTimer < spawnAnimationDuration)
		{
			spawnAnimationTimer += dT / 1000;

			// Actualizar posicion de sombra
			if (shadowLineTr != nullptr && tr != nullptr)
				shadowLineTr->setPosition(tr->getPosition());

			if (spawnAnimationTimer > spawnAnimationDuration) {
				spawnAnimationTimer = spawnAnimationDuration;

				// Ultima iteracion de la animacion de spawneo

				// Borrar sombra
				deleteShadow();

				// Ralentizar rotacion del arma
				rotationSpeed = rotationSpeedIdle;
			}

			float t = spawnAnimationTimer / spawnAnimationDuration;

			// Animacion tamaño
			float startValue = initialPosY + spawnHeight;
			float endValue = initialPosY;
			float currentValue = lerp(startValue, endValue, t);

			if (_gameObject != nullptr) {

				Transform* tr = _gameObject->getComponent<Transform>();
				if (tr != nullptr)
					tr->setPosition(LMVector3(tr->getPosition().getX(), currentValue, tr->getPosition().getZ()));
			}
		}

		currentRotation += rotationSpeed * dT / 1000;

		// Animacion rotacion
		if (tr != nullptr)
			tr->setRotationWithVector(LMVector3(0, currentRotation, 0));

		if (LocalMultiplayerManager::GetInstance() == nullptr)
			return;
		std::array<LocalMultiplayerManager::PlayerData, 4> allPlayers = LocalMultiplayerManager::GetInstance()->getPlayers();

		// Comprobar si algun player la recoge
		for (int i = 0; i < allPlayers.size(); i++) {
			LocalMultiplayerManager::PlayerData thisPlayer = allPlayers[i];
			// Realizar cambios solo si es valido el controllerId
			if (thisPlayer.controllerId == Input::InputManager::invalidControllerId())
				continue;
			GameObject* thisPlayerObj = thisPlayer.gameObject;
			if (thisPlayerObj == nullptr)
				continue;

			if (_gameObject != nullptr) {

				Transform* tr = _gameObject->getComponent<Transform>();
				Transform* playerTr = thisPlayerObj->getComponent<Transform>();
				PlayerController* playerContr = thisPlayerObj->getComponent<PlayerController>();

				if (tr == nullptr || playerTr == nullptr || playerContr == nullptr)
					return;

				if (!playerContr->getHasWeapon() && LMVector3::distance(playerTr->getPosition(), tr->getPosition()) < 10) {
					playerContr->pickWeapon(_gameObject->getName(), spawnInd);
					isPicked = true;
					holderPlayerName = thisPlayerObj->getName();
					deleteShadow();
				}
			}
		}
	}
	else {
		// El arma la lleva un jugador
		// Mover revolver

		Scene* activeScene = SceneManager::GetInstance()->getActiveScene();
		if (activeScene != nullptr || _gameObject != nullptr) {

			GameObject* holderPlayerObj = activeScene->getObjectByName(holderPlayerName);
			if (holderPlayerObj != nullptr) {

				PlayerController* playerContr = holderPlayerObj->getComponent<PlayerController>();
				Transform* playerTr = holderPlayerObj->getComponent<Transform>();
				Transform* tr = _gameObject->getComponent<Transform>();

				if (tr == nullptr || playerContr == nullptr || playerTr == nullptr)
					return;

				tr->setPosition(playerTr->getPosition() + playerContr->getDirection() * 5);
				LMQuaternion newRotation = playerTr->getRotation().rotate(LMVector3(0, 1, 0), -90);
				tr->setRotation(newRotation);

				// Si no queda municion se destruye el arma
				if (ammo <= 0)
					playerContr->dropWeapon();
			}
		}
	}
}

bool JuegoDePistolas::Weapon::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
	return true;
}

float JuegoDePistolas::Weapon::lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void JuegoDePistolas::Weapon::deleteShadow()
{
	Scene* activeScene = SceneManager::GetInstance()->getActiveScene();
	if (activeScene != nullptr) {

		// Borrar sombra
		if (shadowLineTr != nullptr) {
			GameObject* shadowLineObj = shadowLineTr->getGameObject();
			if (shadowLineObj != nullptr) {

				shadowLineTr = nullptr;
				activeScene->removeGameobject(shadowLineObj->getName());
				activeScene->removeGameobject(shadowLineObj->getName());
			}
		}
	}
}


int JuegoDePistolas::Weapon::getSpawnPoint() {
	return spawnInd;
}

void JuegoDePistolas::Weapon::setSpawnPoint(int spawnpoint) {
	spawnInd = spawnpoint;
}

void JuegoDePistolas::Weapon::deleteWeapon()
{
	Scene* activeScene = SceneManager::GetInstance()->getActiveScene();

	if (activeScene != nullptr && _gameObject != nullptr) {

		activeScene->removeGameobject(_gameObject->getName());

		// Notificar al spawner, que tiene un registro de todas las armas que hay en escena
		GameplayManager* gameplayManager = GameplayManager::GetInstance();
		if (gameplayManager != nullptr) {

			GameObject* gameplayManagerObj = gameplayManager->getGameObject();

			if (gameplayManagerObj != nullptr) {

				Spawner* spawner = gameplayManagerObj->getComponent<Spawner>();
				if (spawner != nullptr)
					spawner->deleteWeapon(_gameObject->getName());

				// Borrar la sombra si todavia no ha sido borrada
				deleteShadow();
			}
		}
	}
}
