#pragma once
#include "Component.h"

using namespace LocoMotor;

namespace JuegoDePistolas {
	class Weapon:public LocoMotor::Component
	{
	public:
		Weapon();
		~Weapon();
		
		void shoot(int playerId, int bulletId);
		int getSpawnPoint();
		void setSpawnPoint(int spawnpoint);
		void deleteWeapon();
	protected:
		void start() override;
		void update(float dT) override;
		void setParameters(std::vector<std::pair<std::string, std::string>>& params) override;
	private:
		// El player que ha recogido el arma 
		std::string holderPlayerName = "";
		bool isPicked = false;
		int ammo = 5;

		int spawnInd=0;
	};
}


