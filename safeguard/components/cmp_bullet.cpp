#include "cmp_bullet.h"
#include "cmp_physics.h"
#include "cmp_sprite.h"
#include "system_physics.h"

using namespace std;
using namespace sf;

void BulletComponent::update(double dt) {

	// Move it
	_parent->get_components<PhysicsComponent>()[0]->setVelocity(_direction * _speed);

	// Delete if hits something (but not the owner)
	auto touching = _parent->get_components<PhysicsComponent>()[0]->getTouching();
	if (touching.size() > 0)
	{
		for (auto t : touching)
		{
			if (t->GetFixtureA() != _tower->get_components<PhysicsComponent>()[0]->getFixture())
			{
				_parent->setForDelete();
			}
		}
	}

  _lifetime -= dt;
  if (_lifetime <= 0.f) {
    _parent->setForDelete();
  }
  if (_tower->is_fordeletion()) _parent->setForDelete();
}

BulletComponent::BulletComponent(Entity* p, Entity* tower, Vector2f direction, float speed)
	: Component(p), _tower(tower), _direction(direction), _lifetime(5.0f), _speed(speed) {}

void BulletComponent::setBulletDamage(float damage)
{
	this->_dmg = damage;
}

float BulletComponent::getBulletDamage()
{
	return this->_dmg;
}


