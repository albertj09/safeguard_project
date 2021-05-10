#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class BulletComponent : public Component {
protected:
  float _lifetime;
  sf::Vector2f _direction;
  float _speed;
  Entity* _tower;
  float _dmg;
  bool _air;

public:
  void update(double dt) override;
  void setBulletDamage(float damage);
  float getBulletDamage();
  void setBulletAir(bool b);
  bool isBulletAir();
  void render() override {}
  explicit BulletComponent(Entity* p, Entity* tower, sf::Vector2f direction, float speed);
  BulletComponent() = delete;
};
