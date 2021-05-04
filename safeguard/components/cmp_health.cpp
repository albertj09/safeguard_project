#include "cmp_health.h"
#include "system_resources.h"
#include "system_renderer.h"
#include "engine.h"
#include "cmp_sprite.h"

using namespace std;
using namespace sf;

HealthComponent::HealthComponent(Entity* p) : Component(p) {}

int HealthComponent::getHealth()
{
    return this->_health;
}

void HealthComponent::setHealth(int hp)
{
    if (hp <= 0) {
        this->_health = 0;
    }
    else {
        this->_health = hp;
    }
}

void HealthComponent::initialize(int hp)
{
    this->_health = hp;
}

