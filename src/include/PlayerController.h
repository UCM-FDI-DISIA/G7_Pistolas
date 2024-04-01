#pragma once
#include "Component.h"

namespace JuegoDePistolas {
	class PlayerController:public LocoMotor::Component
	{
	public:
		PlayerController();
		~PlayerController();
	protected:
		void start() override;
		void update(float dT) override;
		void setParameters(std::vector<std::pair<std::string, std::string>>& params) override;
	};
}



