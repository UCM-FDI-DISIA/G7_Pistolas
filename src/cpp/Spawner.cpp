
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


void JuegoDePistolas::Spawner::addSpawnpoints(LMVector3 pos)
{
	_spawners.push_back({ true,pos });
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

		int pos = std::rand() % _spawners.size();

		if (getSpawnerAvailableState(pos)) {
			addWeapon(_weaponID, pos);
			_weaponID++;
		}
		_currTimeTospawn = 0;
	}
}

void JuegoDePistolas::Spawner::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{

	for (const auto& pair : params) {
		if (pair.first == "spawnpoint1") {

			_spawners.push_back({ true,LMVector3::stringToVector(pair.second) });
		}
		else if (pair.first == "spawnpoint2") {
			_spawners.push_back({ true,LMVector3::stringToVector(pair.second) });
		}
		else if (pair.first == "spawnpoint3") {
			_spawners.push_back({ true,LMVector3::stringToVector(pair.second) });
		}
		else if (pair.first == "spawnpoint4") {
			_spawners.push_back({ true,LMVector3::stringToVector(pair.second) });
		}

	}

}


void JuegoDePistolas::Spawner::setSpawnerPosition(int id, LMVector3 pos)
{
	_spawners[id].second = pos;
}

LMVector3 JuegoDePistolas::Spawner::getSpawnerPosition(int id)
{
	return _spawners[id].second;
}

void JuegoDePistolas::Spawner::setSpawnerAvailable(int id, bool avail)
{
	_spawners[id].first = avail;
}

bool JuegoDePistolas::Spawner::getSpawnerAvailableState(int id)
{
	return _spawners[id].first;
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


	transfComp->setPosition(_spawners[spawnindex].second);
	transfComp->setSize({ 1, 1, 1 });
	weaponComp->setSpawnPoint(spawnindex);

	setSpawnerAvailable(spawnindex, false);

	// Añadir arma al registro de armas de la partida
	allWeapons[weaponName] = weaponComp;
	std::cout << "Currrent weapons = " << allWeapons.size() << std::endl;
}

void JuegoDePistolas::Spawner::deleteWeapon(const std::string& key)
{
	// Busca el elemento en el diccionario
	auto it = allWeapons.find(key);

	// Verifica si se encontró el elemento
	if (it != allWeapons.end()) {
		allWeapons.erase(it); // Elimina el elemento del diccionario
	}
	std::cout << "Currrent weapons = " << allWeapons.size() << std::endl;
}

void JuegoDePistolas::Spawner::resetRound()
{
	for (const auto& spawner : _spawners) {
		// spawner es de tipo std::pair<bool, LMVector3>
		bool flag = spawner.first;
		LMVector3 vector = spawner.second;

	}
}