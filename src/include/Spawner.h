#pragma once
#include "LMVector.h"
#include "Component.h"
#include "Weapon.h"
#include <vector>
#include <unordered_map>
#include <string>

using namespace LocoMotor;

namespace JuegoDePistolas {

	class Spawner :public LocoMotor::Component
	{
	public:
		Spawner();
		~Spawner();

		void setSpawnerPosition(int id,LMVector3 pos);
		LMVector3 getSpawnerPosition(int id);
		void setSpawnerAvailable(int id,bool avail);
		bool getSpawnerAvailableState(int id);
		void addWeapon(int id, int spawnindex);

		// Eliminar un arma por su nombre del map de armas del spawner
		void deleteWeapon(const std::string weaponName);

		// Se llama al iniciar una nueva ronda
		void startRound();

	protected:
		void awake() override;
		void start() override;
		void update(float dT) override;
		bool setParameters(std::vector<std::pair<std::string, std::string>>& params) override;

	private:

		std::vector<std::pair<bool, LMVector3>> gunSpawners;

		std::vector<GameObject*> _spawnPoints;
		// Mantiene un registro de todas las armas que hay actualmente en el escenario
		std::unordered_map<std::string, Weapon*> allWeapons;
		
		float _currTimeTospawn = 0;
		int _weaponID = 0;
		float _timeToSpawn = 5000;
	};
}
