#include "tower.h"

class AttackTower : Tower {

public:

	AttackTower() = default;

	std::shared_ptr<Entity> create_tower();
	std::shared_ptr<Entity> create_tower_bullet(Entity* tower, sf::Vector2f direction);

protected:

	float _firetime;

};