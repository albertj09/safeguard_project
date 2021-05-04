#include "cmp_enemy_ai.h"
#include "../gamestuff.h"
#include <cmath>

using namespace std;
using namespace sf;

void EnemyAIComponent::update(double dt) {


  _direction = normalize(_waypointsEntities[_waypointIndex]->getPosition() - _parent->getPosition());


  if (calculateDistance(_parent->getPosition(), _waypointsEntities[_waypointIndex]->getPosition()) < 0.1f) {
      _waypointIndex++;
  }


 /* auto mov = _direction * (float)(dt * _speed);
  mov.x += _direction.x * 16.f;*/
 /* if (!validMove(_parent->getPosition() + mov)) {
    _direction *= -1.f;
  }*/

  move(_direction * (float)(dt * _speed));
  ActorMovementComponent::update(dt);
}

void EnemyAIComponent::load(int level)
{
    if (level == 1) {
        _waypointsEntities.push_back(Engine::GetActiveScene()->ents.find("waypoint1")[0]);
        _waypointIndex = 0;
    }
    if (level == 2) {
        _waypointsEntities.push_back(Engine::GetActiveScene()->ents.find("waypoint1")[0]);
        _waypointsEntities.push_back(Engine::GetActiveScene()->ents.find("waypoint2")[0]);
        _waypointsEntities.push_back(Engine::GetActiveScene()->ents.find("waypoint3")[0]);
        _waypointsEntities.push_back(Engine::GetActiveScene()->ents.find("waypoint4")[0]);
        _waypointsEntities.push_back(Engine::GetActiveScene()->ents.find("waypoint5")[0]);
        _waypointIndex = 0;
    }
    if (level == 3) {
        _waypointsEntities.push_back(Engine::GetActiveScene()->ents.find("waypoint1")[0]);
        _waypointsEntities.push_back(Engine::GetActiveScene()->ents.find("waypoint2")[0]);
        _waypointsEntities.push_back(Engine::GetActiveScene()->ents.find("waypoint3")[0]);
        _waypointsEntities.push_back(Engine::GetActiveScene()->ents.find("waypoint4")[0]);
        _waypointIndex = 0;
    }
    _health = 100;
}

void EnemyAIComponent::setGotShot(bool hit)
{
    this->_gotShot = hit;
}

bool EnemyAIComponent::getGotShot()
{
    return this->_gotShot;
}

int EnemyAIComponent::getHealth()
{
    return this->_health;
}

void EnemyAIComponent::setHealth(int hp)
{
    if (hp <= 0) {
        this->_health = 0;
    }
    else {
        this->_health = hp;
    }
}


EnemyAIComponent::EnemyAIComponent(Entity* p) : ActorMovementComponent(p) {
  _direction = Vector2f(1.0f, 0);
  _speed = 40.0f;
}
