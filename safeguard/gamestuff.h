#pragma once

#include <string>
#include "ecm.h"

std::vector<std::shared_ptr<Entity>> spawn_enemies(int no_of_enemies);
std::shared_ptr<Entity> create_purchase_tower_button(std::string);
std::shared_ptr<Entity> create_tower_bullet(Entity* tower, sf::Vector2f direction);
std::shared_ptr<Entity> create_tower();