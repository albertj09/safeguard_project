#include "tower.h"

int Tower::getUpgradeLevel()
{
	return this->_upgradeLevel;
}

void Tower::setUpgradeLevel(int lvl)
{
	this->_upgradeLevel = lvl;
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

float Tower::getDamage()
{
	return this->_damage;
}

void Tower::setDamage(float dmg)
{
	this->_damage = dmg;
}

void Tower::setTowerType(std::string type)
{
	this->_towerType = type;
}

std::string Tower::getTowerType()
{
	return this->_towerType;
}
