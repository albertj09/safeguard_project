#pragma once

#include <string>
#include "ecm.h"
#include "tower.h"
#include "AttackTower.h"

std::vector<std::shared_ptr<Entity>> spawn_enemies(int no_of_enemies);
std::shared_ptr<Entity> create_purchase_tower_button_ATTACK(std::string);
void create_upgradeInterface_ATTACKTOWER(AttackTower* tower, sf::Vector2f loc);
std::string boolToString(bool b);