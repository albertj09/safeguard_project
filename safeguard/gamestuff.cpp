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

//bool helper function
std::string boolToString(bool b)
{
	if (b) {
		return "yes";
	}
	else {
		return "no";
	}
}


void create_upgradeInterface_ATTACKTOWER(AttackTower* tower, sf::Vector2f loc){

	//simply create a rectangular box area
	auto InterfaceArea = Engine::GetActiveScene()->makeEntity();
	InterfaceArea->addTag("upgradeInterface"); //tag it for potential reasons

	//define the size and shape
	auto shape = InterfaceArea->addComponent<ShapeComponent>();
	shape->setShape<RectangleShape>(Vector2f(200.0f, 150.0f));
	shape->getShape().setOrigin(shape->getShape().getLocalBounds().width / 2, shape->getShape().getLocalBounds().height / 2);
	shape->getShape().setFillColor(Color::Color(255,255,255,130));

	InterfaceArea->setPosition(Vector2f(loc.x + 150.0f, loc.y - 64.0f));

	//Display the statistics of the tower
	auto labelText = InterfaceArea->addComponent<TextComponent>("Attack speed: " + std::to_string((int)tower->getBaseFireRate()) + "\n" + "Range: " + std::to_string((int)tower->getRange()) + "\n" + "Anti air?: " + boolToString(tower->getShootsAirEnemies()));
	labelText->getText()->setColor(Color::Black);
	labelText->getText()->setCharacterSize(15.0f);
	labelText->getText()->setStyle(sf::Text::Bold);
	labelText->getText()->setOrigin(labelText->getText()->getLocalBounds().width / 2 - 5.0f, labelText->getText()->getLocalBounds().height / 2 + 45.0f);

	//-----------------------------------------------------------------UPGRADE BUTTON-----------------------------------------------------------------

	//button entity
	auto UpgradeButton = Engine::GetActiveScene()->makeEntity();
	UpgradeButton->addTag("upgradeButton"); //tag it for potential reasons

	//define the size and shape of the button
	auto buttonShape = UpgradeButton->addComponent<ShapeComponent>();
	buttonShape->setShape<RectangleShape>(Vector2f(90.0f, 30.0f));
	buttonShape->getShape().setFillColor(Color::Magenta);

	//text
	auto upgradeButtonText = UpgradeButton->addComponent<TextComponent>("UPGRADE");
	upgradeButtonText->getText()->setOrigin(upgradeButtonText->getText()->getLocalBounds().width / 2 - 70, upgradeButtonText->getText()->getLocalBounds().height / 2 - 15);
	upgradeButtonText->getText()->setColor(Color::Blue);
	upgradeButtonText->getText()->setCharacterSize(15.0f);
	upgradeButtonText->getText()->setStyle(sf::Text::Bold);

	//add button component
	UpgradeButton->addComponent<ButtonComponent>(buttonShape, upgradeButtonText);
	UpgradeButton->setPosition(Vector2f(InterfaceArea->getPosition().x - 45.0f, InterfaceArea->getPosition().y + 32.0f));
}










//std::vector<std::shared_ptr<Entity>> spawn_enemies(int no_of_enemies) {
//
//}