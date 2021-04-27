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
shared_ptr<Entity> create_purchase_tower_button_ATTACK(string text) {
	auto p_t_button = Engine::GetActiveScene()->makeEntity();
	p_t_button->addTag("purchase_tower_button");

	auto shape = p_t_button->addComponent<ShapeComponent>();
	
	shape->setShape<RectangleShape>(Vector2f(64.0f, 64.0f));
	shape->getShape().setOrigin(shape->getShape().getLocalBounds().width / 2, shape->getShape().getLocalBounds().height / 2);
	

	auto text1 = p_t_button->addComponent<TextComponent>(text);
	text1->getText()->setOrigin(64.0f / 2 - 5, text1->getText()->getLocalBounds().height / 2 - 15);
	text1->getText()->setColor(Color::Red);
	text1->getText()->setCharacterSize(15.0f);
	text1->getText()->setStyle(sf::Text::Bold);


	p_t_button->addComponent<ButtonComponent>(shape, text1);
	p_t_button->setPosition(sf::Vector2f(64.0f, 665.0f));
	return p_t_button;
}

shared_ptr<Entity> create_tower_ATTACK() {
	auto tower = Engine::GetActiveScene()->makeEntity();
	tower->addTag("attack_tower");
	auto spritec = tower->addComponent<SpriteComponent>();
	auto tex = Resources::get<Texture>("towers.png");
	spritec->setTexture(tex);
	spritec->getSprite().setTextureRect(sf::IntRect(13, 180, 50, 50));
	spritec->getSprite().setScale(sf::Vector2f(2.0f, 2.0f));
	//tower->setPosition(sf::Vector2f(128, 128));
	return tower;
	
}



std::shared_ptr<Entity> create_tower_bullet(Entity* tower, Vector2f direction) {
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
	auto b  = e->addComponent<BulletComponent>(tower, direction, 600.0f);

	return e;

}

//std::vector<std::shared_ptr<Entity>> spawn_enemies(int no_of_enemies) {
//
//}