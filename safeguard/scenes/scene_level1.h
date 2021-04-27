#pragma once

#include "engine.h"
#include <LevelSystem.h>
#include <algorithm>
#include "../tower.h"
#include "../AttackTower.h"


struct towerSets {
	std::shared_ptr<AttackTower> towerobj;
	std::shared_ptr<Entity> entityobj;
};



class Level1Scene : public Scene {
public:
	
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

private:

	std::vector<std::shared_ptr<Entity>> _towers;
	std::vector<std::shared_ptr<Entity>> _enemies;
	std::shared_ptr<Entity> _purchase_attacktower_btn;
	std::shared_ptr<Entity> _attack_tower;
	float _clickTimeout;
	float _shootingDelay;
	bool _towerBeingPlaced;
	int _index;
	std::vector<sf::Vector2f> _towerCoords;
	std::vector<std::shared_ptr<AttackTower>> _attackTowers;
	std::map<std::shared_ptr<AttackTower>, std::shared_ptr<Entity>> entity_map;

	std::vector<towerSets> _towerSets;
	

	
	
	
};
