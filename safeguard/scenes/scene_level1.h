#pragma once

#include "engine.h"
#include <map>
#include <LevelSystem.h>
#include <unordered_map>
#include <algorithm>





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
	bool _towerBeingPlaced;
	int _index;
	std::vector<sf::Vector2f> _towerCoords;
	std::vector<sf::Vector2ul> _nonConvertedTiles;
	
};
