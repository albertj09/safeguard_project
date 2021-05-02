#pragma once

#include <string>
#include "ecm.h"
#include "tower.h"
#include "AttackTower.h"
#include "AirTower.h"

std::vector<std::shared_ptr<Entity>> spawn_enemies(int no_of_enemies);
void create_upgradeInterface_ATTACKTOWER(AttackTower* tower, sf::Vector2f loc);
void create_upgradeInterface_AIRTOWER(AirTower* tower, sf::Vector2f loc);
std::string boolToString(bool b);
std::string InterpretLevel(AttackTower* tower);
int attackSpeedInterpreter(AttackTower* tower);
std::string InterpretLevelAIR(AirTower* tower);
int attackSpeedInterpreterAIR(AirTower* tower);
void createBuyInterface();