#include "Weapon.h"
#include "LocalMultiplayerManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Bullet.h"
#include "MeshRenderer.h"
#include <string>

JuegoDePistolas::Weapon::Weapon()
{
}

JuegoDePistolas::Weapon::~Weapon()
{
}

void JuegoDePistolas::Weapon::shoot(int playerId, int bulletId)
{
	Transform* weaponTr = _gameObject->getComponent<Transform>();
	if (weaponTr == nullptr|| SceneManager::GetInstance()->getActiveScene()->getObjectByName(holderPlayerName)->getComponent<Transform>()==nullptr)return;

	std::string bulletName = "Bullet" + std::to_string(playerId) + std::to_string(bulletId);

	GameObject* nBullet = SceneManager::GetInstance()->getActiveScene()->addGameobjectRuntime(bulletName);

	Bullet* bullComp = (Bullet*)nBullet->addComponent("Bullet");
	Transform* transfComp = (Transform*)nBullet->addComponent("Transform");
	MeshRenderer* meshComp = (MeshRenderer*)nBullet->addComponent("MeshRenderer");

	meshComp->setMesh("Bullet.mesh");
	meshComp->setMaterial("Bullet");
	meshComp->setVisible(true);
	meshComp->setEnabled(true);
	bullComp->setBulletActive(true);
	bullComp->setVelocity(100);

	transfComp->setPosition({ weaponTr->getPosition().getX(),weaponTr->getPosition().getY(),weaponTr->getPosition().getZ() });
	transfComp->setSize({ 1, 1, 1 });
	
	
	bullComp->setDirection(SceneManager::GetInstance()->getActiveScene()->getObjectByName(holderPlayerName)->getComponent<Transform>()->getRotation().forward() * -1);
		
	transfComp->setRotation(SceneManager::GetInstance()->getActiveScene()->getObjectByName(holderPlayerName)->getComponent<Transform>()->getRotation());
	

	ammo--;
}

void JuegoDePistolas::Weapon::start()
{
}

void JuegoDePistolas::Weapon::update(float dT)
{
	if (!isPicked) { // El arma esta en el mapa esperando a ser recogida
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
				playerContr->pickWeapon(_gameObject->getName(),spawnInd);
				isPicked = true;
				holderPlayerName = thisPlayerObj->getName();
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

		if (ammo <= 0) { // Si no queda municion se destruye el arma
			playerContr->dropWeapon();
			SceneManager::GetInstance()->getActiveScene()->removeGameobject(_gameObject->getName());
		}
	}
	
}

void JuegoDePistolas::Weapon::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}


int JuegoDePistolas::Weapon::getSpawnPoint() {
	return spawnInd;
}

void JuegoDePistolas::Weapon::setSpawnPoint(int spawnpoint) {
	spawnInd = spawnpoint;
}