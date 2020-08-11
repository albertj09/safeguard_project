#include "gamestuff.h"
#include "engine.h"
#include <LevelSystem.h>
#include "components/cmp_sprite.h"
#include "components/cmp_text.h"
#include "components/cmp_button.h"
#include "components/cmp_bullet.h"
#include "components/cmp_physics.h"
#include "system_resources.h"
#include "SFML/Graphics.hpp"
#include "gamestuff.h"

using namespace std;
using namespace sf;



//button to purchase the attack tower
shared_ptr<Entity> create_purchase_tower_button(string text) {
	auto p_t_button = Engine::GetActiveScene()->makeEntity();
	p_t_button->addTag("purchase_tower_button");

	auto s = p_t_button->addComponent<ShapeComponent>();
	
	s->setShape<RectangleShape>(Vector2f(64.0f, 64.0f));
	s->getShape().setOrigin(s->getShape().getLocalBounds().width / 2, s->getShape().getLocalBounds().height / 2);
	

	auto t = p_t_button->addComponent<TextComponent>(text);
	t->getText()->setOrigin(64.0f / 2 - 5, t->getText()->getLocalBounds().height / 2 - 15);
	t->getText()->setColor(Color::Red);
	t->getText()->setCharacterSize(15.0f);
	t->getText()->setStyle(sf::Text::Bold);
	

	p_t_button->addComponent<ButtonComponent>(s, t);
	p_t_button->setPosition(sf::Vector2f(64.0f, 665.0f));
	return p_t_button;
}

shared_ptr<Entity> create_tower() {
	auto tower = Engine::GetActiveScene()->makeEntity();
	tower->addTag("attack_tower");


	auto spritec = tower->addComponent<SpriteComponent>();
	auto tex = Resources::get<Texture>("towers.png");
	spritec->setTexture(tex);
	spritec->getSprite().setTextureRect(sf::IntRect(13, 180, 50, 50));
	spritec->getSprite().setScale(sf::Vector2f(2.0f, 2.0f));
	tower->setPosition(sf::Vector2f(128, 128));
	return tower;
	
}



//std::shared_ptr<Entity> create_tower_bullet(Entity* tower, sf::Vector2f direction) {
//	auto e = Engine::GetActiveScene()->makeEntity();
//	e->addTag("bullet");
//
//	//The bullet appears from the tower
//	Vector2f pos = tower->getPosition() + (32.0f * direction);
//	e->setPosition(pos);
//
//	auto s = e->addComponent<SpriteComponent>(); //create the bullet sprite component
//
//	auto tex1 = sf::Texture();
//	tex1.loadFromFile("res/img/enemies.png");	//load texture sheet 1
//	
//	s->getSprite().setTexture(tex1);
//	s->getSprite().setTextureRect(sf::IntRect(32 * 38, 32 * 22, 32, 32));
//	s->getSprite().setOrigin(s->getSprite().getLocalBounds().width / 2, s->getSprite().getLocalBounds().height / 2);
//
//	auto p = e->addComponent<PhysicsComponent>(true, Vector2f(1.0f, 1.0f));
//	p->getBody()->SetBullet(true);
//
//	direction.y *= -1;
//	auto b  = e->addComponent<BulletComponent>(tower, direction, 600.0f);
//
//	return e;
//
//}

//std::vector<std::shared_ptr<Entity>> spawn_enemies(int no_of_enemies) {
//
//}