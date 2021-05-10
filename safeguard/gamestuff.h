#pragma once

#include <string>
#include "ecm.h"
#include "tower.h"
#include "AttackTower.h"
#include "AirTower.h"
#include "components/cmp_enemy_ai.h"
#include "components/cmp_health.h"

struct locationTypes {
	sf::Vector2f location;
	std::string type;
};

//-----------------ATTACK TOWER STRUCTURES------------------
struct attackTowerSets {
	AttackTower* towerobj;	
	std::shared_ptr<Entity> entityobj;
};

struct mappingAttackTowerSets {
	sf::Vector2f position;
	attackTowerSets sets;
};
//----------------------------------------------------------

//----------------AIR TOWER STRUCTURES----------------------
struct airTowerSets {
	AirTower* towerobj;	
	std::shared_ptr<Entity> entityobj;
};
struct mappingAirTowerSets {
	sf::Vector2f position;
	airTowerSets sets;
};
//-----------------------------------------------------------

//std::vector<std::shared_ptr<Entity>> spawn_enemies(int no_of_enemies);
std::shared_ptr<Entity> spawn_enemy(int level);
std::shared_ptr<Entity> spawn_enemy_FLYING(int level);
std::shared_ptr<Entity> spawn_enemy_BOSS(int level);
std::shared_ptr<Entity> spawn_enemy_FLYING_BOSS(int level);
std::shared_ptr<Entity> spawn_enemy_BOSS_FINAL(int level);
std::shared_ptr<Entity> spawn_enemy_FLYING_BOSS_FINAL(int level);
void createWayPointEntities(int level);
void create_upgradeInterface_ATTACKTOWER(AttackTower* tower, sf::Vector2f loc);
void create_upgradeInterface_AIRTOWER(AirTower*	tower, sf::Vector2f loc);
std::string boolToString(bool b);
std::string InterpretLevel(AttackTower* tower);
int attackSpeedInterpreter(AttackTower* tower);
std::string InterpretLevelAIR(AirTower* tower);
int attackSpeedInterpreterAIR(AirTower* tower);
void createBuyInterface();
float calculateDistance(sf::Vector2f vec1, sf::Vector2f vec2);
void createBaseEntity();
void createMoneyEntity();
void createWaveEntity(int wave);
void createLevelEntity(int level);
void executeWave(int wave);
std::string vsyncInterpreter(bool b);
std::string resolutionInterpreter(int i);
void createPauseMenu();

