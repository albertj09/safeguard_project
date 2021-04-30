#include "tower.h"

int Tower::getUpgradeLevel()
{
	return this->_upgradeLevel;
}

void Tower::updateTime(double dt) {
	if (this->_firerate >= 0.0f) this->_firerate -= (float)dt;
}

float Tower::getFireRateStatus() {
	return this->_firerate;
}

void Tower::setBaseFireRate(float rate) {
	this->_baseFireRate = rate;
}

float Tower::getBaseFireRate()
{
	return this->_baseFireRate;
}

void Tower::setCanFire(bool canFire)
{
	this->_canFire = canFire;
}

bool Tower::getCanFire()
{
	return this->_canFire;
}

float Tower::getRange()
{
	return this->_range;
}

void Tower::setRange(float range)
{
	this->_range = range;
}
