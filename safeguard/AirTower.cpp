#include "AirTower.h"

using namespace sf;



std::shared_ptr<Entity> AirTower::create_tower() {
	auto tower = Engine::GetActiveScene()->makeEntity();
	tower->addTag("air_tower");
	auto spritec = tower->addComponent<SpriteComponent>();
	auto tex = Resources::get<Texture>("towers.png");
	spritec->setTexture(tex);
	spritec->getSprite().setTextureRect(sf::IntRect(47, 55, 50, 50));
	spritec->getSprite().setScale(sf::Vector2f(2.0f, 2.0f));


	return tower;
}



std::shared_ptr<Entity> AirTower::create_tower_bullet(Entity* tower, sf::Vector2f direction)
{
	auto e = Engine::GetActiveScene()->makeEntity();
	e->addTag("bullet");

	//The bullet appears from the tower
	Vector2f pos = tower->getPosition() + (33.0f * direction);
	e->setPosition(pos);

	auto s = e->addComponent<SpriteComponent>(); //create the bullet sprite component

	auto tex1 = Resources::get<Texture>("enemies.png");

	s->setTexture(tex1);
	s->getSprite().setTextureRect(sf::IntRect(1427, 720, 32, 32));
	s->getSprite().setOrigin(s->getSprite().getLocalBounds().width / 2, s->getSprite().getLocalBounds().height / 2);
	s->getSprite().setScale(Vector2f(0.5f, 0.5f));

	auto p = e->addComponent<PhysicsComponent>(true, Vector2f(1.0f, 1.0f));
	p->getBody()->SetBullet(true);


	direction.y *= -1;
	auto b = e->addComponent<BulletComponent>(tower, direction, 600.0f);

	//reset the firerate back to the default fire rate
	_firerate = _baseFireRate;

	return e;

}

bool AirTower::getShootsAirEnemies()
{
	return this->_shootsAirEnemies;
}

int AirTower::getMaxUpgradeLevel()
{
	return this->_maxUpgradeLevel;
}

void AirTower::visualUpgrade(std::shared_ptr<Entity> entity)
{
	if (this->_upgradeLevel == 2) {
		entity->get_components<SpriteComponent>()[0]->getSprite().setTextureRect(sf::IntRect(7, 125, 42, 60));
	}
	else if (this->_upgradeLevel == 3) {
		entity->get_components<SpriteComponent>()[0]->getSprite().setTextureRect(sf::IntRect(199, 123, 45, 50));
	}
}