#pragma once
#include "LMVector.h"
#include "Component.h"
#include <vector>

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
		void addSpawnpoints(LMVector3 pos);

		void addWeapon(int id, int spawnindex);

	protected:
		void start() override;
		void update(float dT) override;
		void setParameters(std::vector<std::pair<std::string, std::string>>& params) override;

	private:

		std::vector<std::pair<bool, LMVector3>> _spawners;
		
		float _currTimeTospawn = 0;
		int _weaponID = 0;
		float _timeToSpawn = 5000;

	};
}
