#pragma once
#include "Component.h"
#include "InputManager.h"
#include "LMVector.h"

using namespace LocoMotor;

namespace JuegoDePistolas {

	class Bullet:public LocoMotor::Component
	{
	public:
		Bullet();
		~Bullet();

		void setDirection(LMVector3 _direction);
		void setVelocity(int vel);

		void setBulletActive(bool _bulletActive);
		
		void destroyBullet();
		void OnCollisionEnter(GameObject* other);
	protected:
		void start() override;
		void update(float dT) override;
		void setParameters(std::vector<std::pair<std::string, std::string>>& params) override;
		void awake()override;

	private:

		LMVector3 direction;

		//const int velocity = 200;
		int velocity = 100;

		bool bulletActive = false;

		float timeToLive=2000;
		float timeAlive = 0;
		bool timeset = false;
	};
}



