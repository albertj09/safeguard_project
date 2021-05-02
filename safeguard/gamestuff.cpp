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

std::string InterpretLevel(AttackTower* tower)
{
	if (tower->getUpgradeLevel() == tower->getMaxUpgradeLevel()) {
		return to_string(tower->getUpgradeLevel()) + "(MAX)";
	}
	else {
		return to_string(tower->getUpgradeLevel());
	}
}

int attackSpeedInterpreter(AttackTower* tower)
{
	return 1.0f * (11.0f - tower->getBaseFireRate());
}

std::string InterpretLevelAIR(AirTower* tower)
{
	if (tower->getUpgradeLevel() == tower->getMaxUpgradeLevel()) {
		return to_string(tower->getUpgradeLevel()) + "(MAX)";
	}
	else {
		return to_string(tower->getUpgradeLevel());
	}
}

int attackSpeedInterpreterAIR(AirTower* tower)
{
	return 1.0f * (11.0f - tower->getBaseFireRate());
}

void createBuyInterface()
{
	//------------------------------INTERFACE AREA-------------------------------

	auto InterfaceArea = Engine::GetActiveScene()->makeEntity();
	InterfaceArea->addTag("buyInterface");

	auto shape = InterfaceArea->addComponent<ShapeComponent>();
	shape->setShape<RectangleShape>(Vector2f(600.0f, 100.0f));
	shape->getShape().setOrigin(shape->getShape().getLocalBounds().width / 2, shape->getShape().getLocalBounds().height / 2);
	shape->getShape().setFillColor(Color::Color(255, 255, 255, 130));

	InterfaceArea->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, Engine::GetWindow().getSize().y / 2 + Engine::GetWindow().getSize().y / 4 + Engine::GetWindow().getSize().y / 8));
	//-----------------------------------------------------------------------------

	//-------------------------------PURCHASE ATTACK TOWER BUTTON-------------------------------
	auto p_at_button = Engine::GetActiveScene()->makeEntity();
	p_at_button->addTag("purchase_tower_button_ATTACK");

	auto buttonShape = p_at_button->addComponent<ShapeComponent>();

	buttonShape->setShape<RectangleShape>(Vector2f(68.0f, 64.0f));
	buttonShape->getShape().setOrigin(shape->getShape().getLocalBounds().width / 2, shape->getShape().getLocalBounds().height / 2);

	auto text1 = p_at_button->addComponent<TextComponent>("Attack\nTower\n $20");
	text1->getText()->setOrigin(text1->getText()->getLocalBounds().width / 2 + 240.0f, text1->getText()->getLocalBounds().height / 2);
	text1->getText()->setColor(Color::Red);
	text1->getText()->setCharacterSize(15.0f);
	text1->getText()->setStyle(sf::Text::Bold);

	p_at_button->addComponent<ButtonComponent>(buttonShape, text1);
	//adjust position of the button entity
	p_at_button->setPosition(Vector2f(InterfaceArea->getPosition().x + 40.0f, InterfaceArea->getPosition().y + 17.5f));
	//------------------------------------------------------------------------------------------

	//-------------------------------PURCHASE AIR TOWER BUTTON-------------------------------
	auto p_airt_button = Engine::GetActiveScene()->makeEntity();
	p_airt_button->addTag("purchase_tower_button_AIR");

	auto buttonShape2 = p_airt_button->addComponent<ShapeComponent>();
	buttonShape2->setShape<RectangleShape>(Vector2f(68.0f, 64.0f));
	buttonShape2->getShape().setOrigin(shape->getShape().getLocalBounds().width / 2, shape->getShape().getLocalBounds().height / 2);
	
	auto text2 = p_airt_button->addComponent<TextComponent>(" Air\nTower\n $25");
	text2->getText()->setOrigin(text2->getText()->getLocalBounds().width / 2 + 245.0f, text2->getText()->getLocalBounds().height / 2);
	text2->getText()->setColor(Color::Red);
	text2->getText()->setCharacterSize(15.0f);
	text2->getText()->setStyle(sf::Text::Bold);

	p_airt_button->addComponent<ButtonComponent>(buttonShape2, text2);
	//adjust position of the button entity
	p_airt_button->setPosition(Vector2f(InterfaceArea->getPosition().x + 150.0f, InterfaceArea->getPosition().y + 17.5f));
}




void create_upgradeInterface_ATTACKTOWER(AttackTower* tower, sf::Vector2f loc){

	//simply create a rectangular box area
	auto InterfaceArea = Engine::GetActiveScene()->makeEntity();
	InterfaceArea->addTag("upgradeInterface"); //tag it for potential reasons

	//define the size and shape
	auto shape = InterfaceArea->addComponent<ShapeComponent>();
	shape->setShape<RectangleShape>(Vector2f(210.0f, 170.0f));
	shape->getShape().setOrigin(shape->getShape().getLocalBounds().width / 2, shape->getShape().getLocalBounds().height / 2);
	shape->getShape().setFillColor(Color::Color(255,255,255,130));

	InterfaceArea->setPosition(Vector2f(loc.x + 180.0f, loc.y - 64.0f));

	//Display the statistics of the tower
	auto labelText = InterfaceArea->addComponent<TextComponent>("TOWER LEVEL: " + InterpretLevel(tower) + "\n\n" + "Attack speed: " + to_string(attackSpeedInterpreter(tower)) + "\n" + "Range: " + to_string((int)tower->getRange()) + "\n" + "Damage: " + to_string((int)tower->getDamage()) + "\n" + "Anti air?: " + boolToString(tower->getShootsAirEnemies()));
	labelText->getText()->setColor(Color::Black);
	labelText->getText()->setCharacterSize(15.0f);
	labelText->getText()->setStyle(sf::Text::Bold);
	labelText->getText()->setOrigin(labelText->getText()->getLocalBounds().width / 2 - 5.0f, labelText->getText()->getLocalBounds().height / 2 + 30.0f);

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
	UpgradeButton->setPosition(Vector2f(InterfaceArea->getPosition().x - 45.0f, InterfaceArea->getPosition().y + 42.0f));

	
}

void create_upgradeInterface_AIRTOWER(AirTower* tower, sf::Vector2f loc)
{
	//simply create a rectangular box area
	auto InterfaceArea = Engine::GetActiveScene()->makeEntity();
	InterfaceArea->addTag("upgradeInterface"); //tag it for potential reasons

	//define the size and shape
	auto shape = InterfaceArea->addComponent<ShapeComponent>();
	shape->setShape<RectangleShape>(Vector2f(210.0f, 170.0f));
	shape->getShape().setOrigin(shape->getShape().getLocalBounds().width / 2, shape->getShape().getLocalBounds().height / 2);
	shape->getShape().setFillColor(Color::Color(255, 255, 255, 130));

	InterfaceArea->setPosition(Vector2f(loc.x + 180.0f, loc.y - 64.0f));

	//Display the statistics of the tower
	auto labelText = InterfaceArea->addComponent<TextComponent>("TOWER LEVEL: " + InterpretLevelAIR(tower) + "\n\n" + "Attack speed: " + to_string(attackSpeedInterpreterAIR(tower)) + "\n" + "Range: " + to_string((int)tower->getRange()) + "\n" + "Damage: " + to_string((int)tower->getDamage()) + "\n" + "Anti air?: " + boolToString(tower->getShootsAirEnemies()));
	labelText->getText()->setColor(Color::Black);
	labelText->getText()->setCharacterSize(15.0f);
	labelText->getText()->setStyle(sf::Text::Bold);
	labelText->getText()->setOrigin(labelText->getText()->getLocalBounds().width / 2 - 5.0f, labelText->getText()->getLocalBounds().height / 2 + 30.0f);

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
	UpgradeButton->setPosition(Vector2f(InterfaceArea->getPosition().x - 45.0f, InterfaceArea->getPosition().y + 42.0f));
}










//std::vector<std::shared_ptr<Entity>> spawn_enemies(int no_of_enemies) {
//
//}