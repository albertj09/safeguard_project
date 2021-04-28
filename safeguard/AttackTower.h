#include "tower.h"

class AttackTower : Tower {

public:

	AttackTower() = default;

	std::shared_ptr<Entity> create_tower();
	std::shared_ptr<Entity> create_tower_bullet(Entity* tower, sf::Vector2f direction);
	void updateTime(double dt);
	float getFireRateStatus();
	void setBaseFireRate(float rate);
	float getBaseFireRate();
	void setCanFire(bool canFire);
	bool getCanFire();
	

protected:

	bool _canFire;
	float _baseFireRate;
	float _firerate = _baseFireRate;

};