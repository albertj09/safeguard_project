#include "tower.h"

class AttackTower : public Tower {

public:

	AttackTower() = default;

	std::shared_ptr<Entity> create_tower();
	std::shared_ptr<Entity> create_tower_bullet(Entity* tower, sf::Vector2f direction);
	bool getShootsAirEnemies();
		
	

protected:
	bool _shootsAirEnemies = false;

};