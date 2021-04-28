#pragma once

#include "engine.h"
#include <LevelSystem.h>
#include <algorithm>
#include "../tower.h"
#include "../AttackTower.h"
#include <map>


struct towerSets {
	AttackTower* towerobj;
	std::shared_ptr<Entity> entityobj;
};

struct mappingAttackTowerSets {
	sf::Vector2f position;
	towerSets sets;
};



class Level1Scene : public Scene {
public:
	
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

private:

	//BUTTONS
	std::shared_ptr<Entity> _purchase_attacktower_btn;


	//ENEMIES
	std::vector<std::shared_ptr<Entity>> _enemies;
	

	//TOWER COORDINATES
	std::vector<sf::Vector2f> _towerCoords;

	
	//TOWER MAPPING SETS
	std::vector<towerSets> _towerSets;
	std::vector<mappingAttackTowerSets> _attackTowerMappingSets;


	//OTHER VARIABLES
	float _clickTimeout;
	bool _towerBeingPlaced;
	int _index;
		
	
};
