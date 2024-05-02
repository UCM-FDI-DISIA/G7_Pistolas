
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

	// Posiciones de los spawnpoints
	gunSpawners = {
		{true, scene->getObjectByName("GunSpawnpoint_1")->getComponent<Transform>()->getPosition()},
		{true, scene->getObjectByName("GunSpawnpoint_2")->getComponent<Transform>()->getPosition()},
		{true, scene->getObjectByName("GunSpawnpoint_3")->getComponent<Transform>()->getPosition()},
		{true, scene->getObjectByName("GunSpawnpoint_4")->getComponent<Transform>()->getPosition()},
		{true, scene->getObjectByName("GunSpawnpoint_5")->getComponent<Transform>()->getPosition()},
		{true, scene->getObjectByName("GunSpawnpoint_6")->getComponent<Transform>()->getPosition()},
		{true, scene->getObjectByName("GunSpawnpoint_7")->getComponent<Transform>()->getPosition()},
		{true, scene->getObjectByName("GunSpawnpoint_8")->getComponent<Transform>()->getPosition()}
	};
}

void JuegoDePistolas::Spawner::start()
{
}

void JuegoDePistolas::Spawner::update(float dT)
{
	_currTimeTospawn += dT;


	// Numero maximo de armas que pueden existir en la escena simultaneamente segun el numero de jugadores actuales
	int maxWeapons = LocalMultiplayerManager::GetInstance()->getNumPlayersConnected() + 1;

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

void JuegoDePistolas::Spawner::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
	//for (const auto& pair : params) {
	//	if (pair.first == "spawnpoint1") {
	//		gunSpawners.push_back({ true,LMVector3::stringToVector(pair.second) });
	//	}
	//	else if (pair.first == "spawnpoint2") {
	//		gunSpawners.push_back({ true,LMVector3::stringToVector(pair.second) });
	//	}
	//	else if (pair.first == "spawnpoint3") {
	//		gunSpawners.push_back({ true,LMVector3::stringToVector(pair.second) });
	//	}
	//	else if (pair.first == "spawnpoint4") {
	//		gunSpawners.push_back({ true,LMVector3::stringToVector(pair.second) });
	//	}
	//}
}


void JuegoDePistolas::Spawner::setSpawnerPosition(int id, LMVector3 pos)
{
	gunSpawners[id].second = pos;
}

LMVector3 JuegoDePistolas::Spawner::getSpawnerPosition(int id)
{
	return gunSpawners[id].second;
}

void JuegoDePistolas::Spawner::setSpawnerAvailable(int id, bool avail)
{
	gunSpawners[id].first = avail;
}

bool JuegoDePistolas::Spawner::getSpawnerAvailableState(int id)
{
	return gunSpawners[id].first;
}

void JuegoDePistolas::Spawner::addWeapon(int id, int spawnindex)
{
	std::string weaponName = "Weapon" + std::to_string(id);

	GameObject* nWeapon = SceneManager::GetInstance()->getActiveScene()->addGameobjectRuntime(weaponName);

	Transform* transfComp = (Transform*)nWeapon->addComponent("Transform");
	MeshRenderer* meshComp = (MeshRenderer*)nWeapon->addComponent("MeshRenderer");
	Weapon* weaponComp = (Weapon*)nWeapon->addComponent("Weapon");

	meshComp->setMesh("Revolver.mesh");
	meshComp->setMaterial("Revolver");
	meshComp->setVisible(true);
	meshComp->setEnabled(true);


	transfComp->setPosition(gunSpawners[spawnindex].second);
	transfComp->setSize({ 1, 1, 1 });
	weaponComp->setSpawnPoint(spawnindex);

	setSpawnerAvailable(spawnindex, false);

	// Añadir arma al registro de armas de la partida
	allWeapons[weaponName] = weaponComp;
	std::cout << "Currrent weapons = " << allWeapons.size() << std::endl;
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
		//deleteWeapon(it->first); // Llama a deleteWeapon con la clave actual
		it->second->deleteWeapon();
		it = allWeapons.begin(); // Reinicia el iterador después de borrar
	}
}