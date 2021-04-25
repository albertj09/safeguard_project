#pragma once

#include "engine.h"

class Level1Scene : public Scene {
public:
	
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

private:

	std::vector<std::shared_ptr<Entity>> towers;
	std::vector<std::shared_ptr<Entity>> _enemies;
	std::shared_ptr<Entity> _purchase_attacktower_btn;
	std::shared_ptr<Entity> _attack_tower;
	float _clickTimeout;
};
