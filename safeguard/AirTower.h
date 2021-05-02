#include "tower.h"

class AirTower : public Tower {

public:

	AirTower() = default;

	std::shared_ptr<Entity> create_tower();
	std::shared_ptr<Entity> create_tower_bullet(Entity* tower, sf::Vector2f direction);
	bool getShootsAirEnemies();
	int getMaxUpgradeLevel();
	void visualUpgrade(std::shared_ptr<Entity> entity);



protected:
	bool _shootsAirEnemies = true;
	int _maxUpgradeLevel = 3;

};