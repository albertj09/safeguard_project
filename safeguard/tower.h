#pragma once

#include "engine.h"
#include "components/cmp_sprite.h"
#include <system_resources.h>
#include "components/cmp_physics.h"
#include "components/cmp_bullet.h"


class Tower {

public:
	int getUpgradeLevel();
	void updateTime(double dt);
	float getFireRateStatus();
	void setBaseFireRate(float rate);
	float getBaseFireRate();
	void setCanFire(bool canFire);
	bool getCanFire();
	float getRange();
	void setRange(float range);

protected:
	int _upgradeLevel;
	bool _canFire;
	float _baseFireRate;
	float _firerate = _baseFireRate;
	float _range;
};