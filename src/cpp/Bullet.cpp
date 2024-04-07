#include "Bullet.h"
#include "GameObject.h"
#include "Transform.h"
#include "LMVector.h"
#include "ParticleSystem.h"

#include <iostream>
#include <cmath>

#include "InputManager.h"
#include "MeshRenderer.h"
#include "LMInputs.h"
#include "SceneManager.h"
#include "Scene.h"

using namespace JuegoDePistolas;
using namespace LocoMotor;

Bullet::Bullet()
{

}

Bullet::~Bullet()
{
}

void JuegoDePistolas::Bullet::setDirection(LMVector3 _direction)
{
	direction = _direction;
}


void Bullet::start()
{
}

void Bullet::update(float dT)
{
	Transform* tr = _gameObject->getComponent<Transform>();

	if (direction.Magnitude() > .1f)
		tr->SetPosition(tr->GetPosition() + direction * velocity * dT/1000);
}

void JuegoDePistolas::Bullet::setParameters(std::vector<std::pair<std::string, std::string>>& params)
{
}

