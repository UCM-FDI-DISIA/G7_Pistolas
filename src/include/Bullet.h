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

	protected:
		void start() override;
		void update(float dT) override;
		void setParameters(std::vector<std::pair<std::string, std::string>>& params) override;

	private:

		LMVector3 direction;

		const int velocity = 200;
	};
}



