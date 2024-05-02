#pragma once
#include "Component.h"
#include "Transform.h"

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

		float lerp(float a, float b, float t);
		void deleteShadow();

		// El player que ha recogido el arma 
		std::string holderPlayerName = "";
		bool isPicked = false;
		int ammo = 5;

		int spawnInd=0;

		float spawnAnimationTimer = 0;
		float spawnAnimationDuration = 3;
		float spawnHeight = 25;

		// Guarda la posicion Y inicial del arma para ejecutar la animacion correctamente
		float initialPosY;

		Transform* tr;
		Transform* shadowLineTr;

		float rotationSpeed;
		float rotationSpeedSpawning = 400;
		float rotationSpeedIdle = 100;
		float currentRotation;
	};
}


