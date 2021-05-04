#include "tower.h"


class AttackTower : public Tower {

public:

	AttackTower() = default;

	std::shared_ptr<Entity> create_tower();
	std::shared_ptr<Entity> create_tower_bullet(Entity* tower, sf::Vector2f direction);
	bool getShootsAirEnemies();
	int getMaxUpgradeLevel();
	void visualUpgrade(std::shared_ptr<Entity> entity);
	
	

protected:
	bool _shootsAirEnemies = false;
	int _maxUpgradeLevel = 4;

};