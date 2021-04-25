#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class BulletComponent : public Component {
protected:
  float _lifetime;
  sf::Vector2f _direction;
  float _speed;
  Entity* _tower;

public:
  void update(double dt) override;
  void render() override {}
  explicit BulletComponent(Entity* p, Entity* tower, sf::Vector2f direction, float speed);
  BulletComponent() = delete;
};
