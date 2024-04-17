#include "MainMenuManager.h"
#include "InputManager.h"
#include "LMInputs.h"
#include "SceneManager.h"

using namespace LocoMotor;

JuegoDePistolas::MainMenuManager::MainMenuManager()
{
}

JuegoDePistolas::MainMenuManager::~MainMenuManager()
{
}

void JuegoDePistolas::MainMenuManager::start()
{
}

void JuegoDePistolas::MainMenuManager::update(float dT)
{
	if (Input::InputManager::GetInstance()->GetKeyDown(Input::LMKS_SPACE)) {
		SceneManager::GetInstance()->loadScene("Assets/Scenes/Scene.lua", "Scene");
		SceneManager::GetInstance()->changeScene("Scene");
	}
}

void JuegoDePistolas::MainMenuManager::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}
