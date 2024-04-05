#pragma once
#include "Component.h"

namespace JuegoDePistolas {
	class LocalMultiplayerManager : public LocoMotor::Component
	{
	public:
		LocalMultiplayerManager();
		~LocalMultiplayerManager();
	protected:
		void start() override;
		void update(float dT) override;
		void setParameters(std::vector<std::pair<std::string, std::string>>& params) override;
	};
}



