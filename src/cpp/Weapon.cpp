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
	Transform* weaponTr = _gameObject->getComponent<Transform>();
	if (weaponTr == nullptr || SceneManager::GetInstance()->getActiveScene()->getObjectByName(holderPlayerName)->getComponent<Transform>() == nullptr)return;

	std::string bulletName = "Bullet" + std::to_string(playerId) + std::to_string(bulletId);

	GameObject* nBullet = SceneManager::GetInstance()->getActiveScene()->addGameobjectRuntime(bulletName);

	Bullet* bullComp = (Bullet*)nBullet->addComponent("Bullet");
	Transform* transfComp = (Transform*)nBullet->addComponent("Transform");
	MeshRenderer* meshComp = (MeshRenderer*)nBullet->addComponent("MeshRenderer");
	EventEmitter* emitComp = (EventEmitter*)nBullet->addComponent("EventEmitter");

	meshComp->setMesh("Bullet.mesh");
	meshComp->setMaterial("Bullet");
	meshComp->setVisible(true);
	meshComp->setEnabled(true);
	bullComp->setBulletActive(true);
	bullComp->setVelocity(40);

	emitComp->setEvent("event:/Shot");
	emitComp->play();


	transfComp->setPosition({ weaponTr->getPosition().getX(),weaponTr->getPosition().getY(),weaponTr->getPosition().getZ() });
	transfComp->setSize({ 2, 2, 2 });


	bullComp->setDirection(SceneManager::GetInstance()->getActiveScene()->getObjectByName(holderPlayerName)->getComponent<Transform>()->getRotation().forward() * -1);

	transfComp->setRotation(SceneManager::GetInstance()->getActiveScene()->getObjectByName(holderPlayerName)->getComponent<Transform>()->getRotation());


	ammo--;
}

//void JuegoDePistolas::Weapon::start()
//{
//	initialPosY = _gameObject->getComponent<Transform>()->getPosition().getY();
//}

void JuegoDePistolas::Weapon::start()
{
	tr = _gameObject->getComponent<Transform>();

	initialPosY = tr->getPosition().getY();

	GameObject* shadowLine = SceneManager::GetInstance()->getActiveScene()->addGameobjectRuntime("shadowLine_" + _gameObject->getName());

	// Guardarse una referencia al transform de la sombra
	shadowLineTr = (Transform*)shadowLine->addComponent("Transform");
	MeshRenderer* shadowLineMesh = (MeshRenderer*)shadowLine->addComponent("MeshRenderer");

	shadowLineMesh->setMesh("ShadowLine.mesh");
	shadowLineMesh->setMaterial("ShadowLineWhite");

	rotationSpeed = rotationSpeedSpawning;
}

void JuegoDePistolas::Weapon::update(float dT)
{

	if (!isPicked) { // El arma esta en el mapa esperando a ser recogida

		// Animacion de spawn
		if (spawnAnimationTimer < spawnAnimationDuration)
		{
			spawnAnimationTimer += dT / 1000;

			// Actualizar posicion de sombra
			if (shadowLineTr != nullptr)
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

			Transform* tr = _gameObject->getComponent<Transform>();
			if (tr != nullptr)
				tr->setPosition(LMVector3(tr->getPosition().getX(), currentValue, tr->getPosition().getZ()));
		}


		currentRotation += rotationSpeed * dT / 1000;

		// Animacion rotacion
		tr->setRotationWithVector(LMVector3(0, currentRotation, 0));

		if (LocalMultiplayerManager::GetInstance() == nullptr)return;
		std::array<LocalMultiplayerManager::PlayerData, 4> allPlayers = LocalMultiplayerManager::GetInstance()->getPlayers();

		// Comprobar si algun player la recoge
		for (int i = 0; i < allPlayers.size(); i++) {
			LocalMultiplayerManager::PlayerData thisPlayer = allPlayers[i];
			// Realizar cambios solo si es valido el controllerId
			if (thisPlayer.controllerId == Input::InputManager::invalidControllerId())
				continue;
			GameObject* thisPlayerObj = thisPlayer.gameObject;
			Transform* tr = _gameObject->getComponent<Transform>();
			Transform* playerTr = thisPlayerObj->getComponent<Transform>();
			PlayerController* playerContr = thisPlayerObj->getComponent<PlayerController>();
			if (tr == nullptr || playerTr == nullptr || playerContr == nullptr)return;
			if (!playerContr->getHasWeapon() && LMVector3::distance(playerTr->getPosition(), tr->getPosition()) < 10) {
				playerContr->pickWeapon(_gameObject->getName(), spawnInd);
				isPicked = true;
				holderPlayerName = thisPlayerObj->getName();
				deleteShadow();
			}
		}
	}
	else { // El arma la lleva un jugador
		// Mover revolver
		GameObject* holderPlayerObj = SceneManager::GetInstance()->getActiveScene()->getObjectByName(holderPlayerName);
		if (holderPlayerObj == nullptr)return;
		PlayerController* playerContr = holderPlayerObj->getComponent<PlayerController>();
		Transform* playerTr = holderPlayerObj->getComponent<Transform>();
		Transform* tr = _gameObject->getComponent<Transform>();
		if (tr == nullptr || playerContr == nullptr)return;
		tr->setPosition(playerTr->getPosition() + playerContr->getDirection() * 5);
		LMQuaternion newRotation = playerTr->getRotation().rotate(LMVector3(0, 1, 0), -90);
		tr->setRotation(newRotation);

		// Si no queda municion se destruye el arma
		if (ammo <= 0)
			playerContr->dropWeapon();
	}
}

void JuegoDePistolas::Weapon::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

float JuegoDePistolas::Weapon::lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void JuegoDePistolas::Weapon::deleteShadow()
{
	// Borrar sombra
	if (shadowLineTr != nullptr) {
		GameObject* shadowLineObj = shadowLineTr->getGameObject();
		if (shadowLineObj != nullptr) {

			shadowLineTr = nullptr;
			SceneManager::GetInstance()->getActiveScene()->removeGameobject(shadowLineObj->getName());
		}
		SceneManager::GetInstance()->getActiveScene()->removeGameobject(shadowLineObj->getName());
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
	if (_gameObject != nullptr)
		SceneManager::GetInstance()->getActiveScene()->removeGameobject(_gameObject->getName());

	// Notificar al spawner, que tiene un registro de todas las armas que hay en escena
	Spawner* spawner = GameplayManager::GetInstance()->getGameObject()->getComponent<Spawner>();
	if (spawner != nullptr)
		spawner->deleteWeapon(_gameObject->getName());

	// Borrar la sombra si todavia no ha sido borrada
	deleteShadow();
}
