#pragma once

#include "cmp_actor_movement.h"
#include <ecm.h>
#include <engine.h>
#include <LevelSystem.h>
//#include "cmp_sprite.h"

class EnemyAIComponent : public ActorMovementComponent {
protected:
	sf::Vector2f _direction;
	std::vector<std::shared_ptr<Entity>> _waypointsEntities;
	int _waypointIndex;
	int _health;
	bool _gotShot;
	bool _airtype;
	bool _bossType;
	

public:
  void update(double dt) override;

  void load(int level);

  void setGotShot(bool hit);

  bool getGotShot();

  int getHealth();

  void setHealth(int hp);

  void setAirType(bool b);

  bool isAirType();

  void setBossType(bool b);

  bool isBossType();

  explicit EnemyAIComponent(Entity* p);

  EnemyAIComponent() = delete;
};
