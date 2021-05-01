#include "AttackTower.h"

using namespace sf;



std::shared_ptr<Entity> AttackTower::create_tower() {
	auto tower = Engine::GetActiveScene()->makeEntity();
	tower->addTag("attack_tower");
	auto spritec = tower->addComponent<SpriteComponent>();
	auto tex = Resources::get<Texture>("towers.png");
	spritec->setTexture(tex);
	spritec->getSprite().setTextureRect(sf::IntRect(13, 180, 50, 50));
	spritec->getSprite().setScale(sf::Vector2f(2.0f, 2.0f));

	
	return tower;	
}



std::shared_ptr<Entity> AttackTower::create_tower_bullet(Entity* tower, sf::Vector2f direction)
{
	auto e = Engine::GetActiveScene()->makeEntity();
	e->addTag("bullet");

	//The bullet appears from the tower
	Vector2f pos = tower->getPosition() + (33.0f * direction);
	e->setPosition(pos);

	auto s = e->addComponent<SpriteComponent>(); //create the bullet sprite component

	auto tex1 = Resources::get<Texture>("enemies.png");
	//tex1.loadFromFile("res/img/enemies.png");	//load texture sheet 1

	s->setTexture(tex1);
	s->getSprite().setTextureRect(sf::IntRect(32 * 38, 32 * 22, 32, 32));
	s->getSprite().setOrigin(s->getSprite().getLocalBounds().width / 2, s->getSprite().getLocalBounds().height / 2);

	auto p = e->addComponent<PhysicsComponent>(true, Vector2f(1.0f, 1.0f));
	p->getBody()->SetBullet(true);


	direction.y *= -1;
	auto b = e->addComponent<BulletComponent>(tower, direction, 600.0f);

	//reset the firerate back to the default fire rate
	_firerate = _baseFireRate;

	return e;

}

bool AttackTower::getShootsAirEnemies()
{
	return this->_shootsAirEnemies;
}

int AttackTower::getMaxUpgradeLevel()
{
	return this->_maxUpgradeLevel;
}
