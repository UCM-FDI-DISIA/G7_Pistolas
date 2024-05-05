
#include "Spawner.h"
#include "Transform.h"
#include "LMVector.h"
#include "GameObject.h"

#include <iostream>
#include <cmath>

#include "MeshRenderer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameplayManager.h"
#include "Weapon.h"
#include <LocalMultiplayerManager.h>

JuegoDePistolas::Spawner::Spawner()
{

}

JuegoDePistolas::Spawner::~Spawner()
{
}


void JuegoDePistolas::Spawner::awake()
{
	Scene* scene = SceneManager::GetInstance()->getActiveScene();

	if (scene != nullptr) {

		GameObject* GunSpawnpoint_1 = scene->getObjectByName("GunSpawnpoint_1");
		GameObject* GunSpawnpoint_2 = scene->getObjectByName("GunSpawnpoint_2");
		GameObject* GunSpawnpoint_3 = scene->getObjectByName("GunSpawnpoint_3");
		GameObject* GunSpawnpoint_4 = scene->getObjectByName("GunSpawnpoint_4");
		GameObject* GunSpawnpoint_5 = scene->getObjectByName("GunSpawnpoint_5");
		GameObject* GunSpawnpoint_6 = scene->getObjectByName("GunSpawnpoint_6");
		GameObject* GunSpawnpoint_7 = scene->getObjectByName("GunSpawnpoint_7");
		GameObject* GunSpawnpoint_8 = scene->getObjectByName("GunSpawnpoint_8");

		if (GunSpawnpoint_1 != nullptr && GunSpawnpoint_2 != nullptr && GunSpawnpoint_3 != nullptr && GunSpawnpoint_4 != nullptr
			&& GunSpawnpoint_5 != nullptr && GunSpawnpoint_6 != nullptr && GunSpawnpoint_7 != nullptr && GunSpawnpoint_8 != nullptr) {

			Transform* GunSpawnpoint_1_transform = GunSpawnpoint_1->getComponent<Transform>();
			Transform* GunSpawnpoint_2_transform = GunSpawnpoint_2->getComponent<Transform>();
			Transform* GunSpawnpoint_3_transform = GunSpawnpoint_3->getComponent<Transform>();
			Transform* GunSpawnpoint_4_transform = GunSpawnpoint_4->getComponent<Transform>();
			Transform* GunSpawnpoint_5_transform = GunSpawnpoint_5->getComponent<Transform>();
			Transform* GunSpawnpoint_6_transform = GunSpawnpoint_6->getComponent<Transform>();
			Transform* GunSpawnpoint_7_transform = GunSpawnpoint_7->getComponent<Transform>();
			Transform* GunSpawnpoint_8_transform = GunSpawnpoint_8->getComponent<Transform>();

			if (GunSpawnpoint_1_transform != nullptr && GunSpawnpoint_2_transform != nullptr && GunSpawnpoint_3_transform != nullptr && GunSpawnpoint_4_transform != nullptr
				&& GunSpawnpoint_5_transform != nullptr && GunSpawnpoint_6_transform != nullptr && GunSpawnpoint_7_transform != nullptr && GunSpawnpoint_8_transform != nullptr) {

				// Posiciones de los spawnpoints
				gunSpawners = {
					{true, GunSpawnpoint_1_transform->getPosition()},
					{true, GunSpawnpoint_2_transform->getPosition()},
					{true, GunSpawnpoint_3_transform->getPosition()},
					{true, GunSpawnpoint_4_transform->getPosition()},
					{true, GunSpawnpoint_5_transform->getPosition()},
					{true, GunSpawnpoint_6_transform->getPosition()},
					{true, GunSpawnpoint_7_transform->getPosition()},
					{true, GunSpawnpoint_8_transform->getPosition()}
				};
			}
		}
	}
}

void JuegoDePistolas::Spawner::start()
{
}

void JuegoDePistolas::Spawner::update(float dT)
{
	_currTimeTospawn += dT;

	LocalMultiplayerManager* localMultiplayerManager = LocalMultiplayerManager::GetInstance();

	if (localMultiplayerManager != nullptr) {

		// Numero maximo de armas que pueden existir en la escena simultaneamente segun el numero de jugadores actuales
		int maxWeapons = localMultiplayerManager->getNumPlayersConnected() + 1;

		// Spawnear un arma
		if (allWeapons.size() < maxWeapons
			&& _currTimeTospawn > _timeToSpawn) {

			int pos = std::rand() % gunSpawners.size();

			int i = 0;
			int maxTries = 50;
			// Cambiarlo hasta encontrar un spawnpoint disponible
			while (!getSpawnerAvailableState(pos) && i < 50)
			{
				pos = std::rand() % gunSpawners.size();
				i++;
			}

			// Si se ha encontrado un spawnpoint disponible
			if (i < maxTries) {
				addWeapon(_weaponID, pos);
				_weaponID++;
			}
			_currTimeTospawn = 0;
		}
	}
}

void JuegoDePistolas::Spawner::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
	return true;
}


void JuegoDePistolas::Spawner::setSpawnerPosition(int id, LMVector3 pos)
{
	if (gunSpawners.size() > id)
		gunSpawners[id].second = pos;
}

LMVector3 JuegoDePistolas::Spawner::getSpawnerPosition(int id)
{
	if (gunSpawners.size() > id)
		return gunSpawners[id].second;
}

void JuegoDePistolas::Spawner::setSpawnerAvailable(int id, bool avail)
{
	if (gunSpawners.size() > id)
		gunSpawners[id].first = avail;
}

bool JuegoDePistolas::Spawner::getSpawnerAvailableState(int id)
{
	if (gunSpawners.size() > id)
		return gunSpawners[id].first;
}

void JuegoDePistolas::Spawner::addWeapon(int id, int spawnindex)
{
	Scene* activeScene = SceneManager::GetInstance()->getActiveScene();

	if (activeScene != nullptr) {

		std::string weaponName = "Weapon" + std::to_string(id);

		GameObject* nWeapon = activeScene->addGameobjectRuntime(weaponName);

		if (nWeapon != nullptr) {
			MeshRenderer* meshComp = (MeshRenderer*)nWeapon->addComponent("MeshRenderer");
			Transform* transfComp = (Transform*)nWeapon->addComponent("Transform");
			Weapon* weaponComp = (Weapon*)nWeapon->addComponent("Weapon");

			if (meshComp != nullptr) {
				meshComp->setMesh("Revolver.mesh");
				meshComp->setMaterial("Revolver");
				meshComp->setVisible(true);
				meshComp->setEnabled(true);
			}

			if (transfComp != nullptr) {
				if (gunSpawners.size() > spawnindex)
					transfComp->setPosition(gunSpawners[spawnindex].second);
				transfComp->setSize({ 1, 1, 1 });
			}

			if (weaponComp != nullptr)
				weaponComp->setSpawnPoint(spawnindex);

			setSpawnerAvailable(spawnindex, false);

			// Añadir arma al registro de armas de la partida
			if (allWeapons.find(weaponName) != allWeapons.end())
				allWeapons[weaponName] = weaponComp;
			std::cout << "Currrent weapons = " << allWeapons.size() << std::endl;
		}
	}
}

void JuegoDePistolas::Spawner::deleteWeapon(const std::string key)
{
	// Busca el elemento en el diccionario
	auto it = allWeapons.find(key);

	// Verifica si se encontró el elemento
	if (it != allWeapons.end()) {
		allWeapons.erase(it); // Elimina el elemento del diccionario
	}
	std::cout << "Currrent weapons = " << allWeapons.size() << std::endl;
}

void JuegoDePistolas::Spawner::startRound()
{
	// Resetear tiempo de spawn
	_currTimeTospawn = 0;

	// Eliminar todas las armas
	for (auto it = allWeapons.begin(); it != allWeapons.end(); ) {

		if (it->second != nullptr)
			it->second->deleteWeapon();
		it = allWeapons.begin(); // Reinicia el iterador después de borrar un elemento
	}
}