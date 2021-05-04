#pragma once

#include <ecm.h>
//#include <SFML/Graphics/Sprite.hpp>
//#include <SFML/Graphics/Texture.hpp>

class HealthComponent : public Component
{	
protected:
	int _health;

public:

	explicit HealthComponent(Entity* p);
	HealthComponent() = delete;
	int getHealth();
	void setHealth(int hp);
	void initialize(int hp);
	
};