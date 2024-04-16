#include "Weapon.h"
#include "LocalMultiplayerManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"

JuegoDePistolas::Weapon::Weapon()
{
}

JuegoDePistolas::Weapon::~Weapon()
{
}

void JuegoDePistolas::Weapon::shoot()
{
	ammo--;
}

void JuegoDePistolas::Weapon::start()
{
}

void JuegoDePistolas::Weapon::update(float dT)
{
	if (!isPicked) { // El arma esta en el mapa esperando a ser recogida
		std::array<LocalMultiplayerManager::PlayerData, 4> allPlayers = LocalMultiplayerManager::GetInstance()->getPlayers();
		allPlayers[0].gameObject;

		// Comprobar si algun player la recoge
		for (int i = 0; i < allPlayers.size(); i++) {
			GameObject* thisPlayerObj = allPlayers[i].gameObject;
			Transform* tr = _gameObject->getComponent<Transform>();
			Transform* playerTr = thisPlayerObj->getComponent<Transform>();
			PlayerController* playerContr = thisPlayerObj->getComponent<PlayerController>();
			if (tr == nullptr || playerTr == nullptr || playerContr == nullptr)return;
			if (!playerContr->getHasWeapon() && LMVector3::distance(playerTr->getPosition(), tr->getPosition()) < 30) {
				playerContr->pickWeapon(_gameObject->getName());
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
		tr->setPosition(playerTr->getPosition() + playerContr->getDirection() * 50);
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
