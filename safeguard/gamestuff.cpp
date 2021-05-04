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

float calculateDistance(sf::Vector2f vec1, sf::Vector2f vec2)
{
	return abs(sqrtf(pow(vec2.x - vec1.x, 2) + pow(vec2.y - vec1.y, 2) * 1.0f));
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
	buttonShape->setShape<RectangleShape>(Vector2f(90.0f, 37.0f));
	buttonShape->getShape().setFillColor(Color::Magenta);

	//text
	auto upgradeButtonText = UpgradeButton->addComponent<TextComponent>("UPGRADE\n ($15)");
	upgradeButtonText->getText()->setOrigin(upgradeButtonText->getText()->getLocalBounds().width / 2 - 70, upgradeButtonText->getText()->getLocalBounds().height / 2 - 30);
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
	buttonShape->setShape<RectangleShape>(Vector2f(90.0f, 37.0f));
	buttonShape->getShape().setFillColor(Color::Magenta);

	//text
	auto upgradeButtonText = UpgradeButton->addComponent<TextComponent>("UPGRADE\n ($15)");
	upgradeButtonText->getText()->setOrigin(upgradeButtonText->getText()->getLocalBounds().width / 2 - 70, upgradeButtonText->getText()->getLocalBounds().height / 2 - 30.0f);
	upgradeButtonText->getText()->setColor(Color::Blue);
	upgradeButtonText->getText()->setCharacterSize(15.0f);
	upgradeButtonText->getText()->setStyle(sf::Text::Bold);

	//add button component
	UpgradeButton->addComponent<ButtonComponent>(buttonShape, upgradeButtonText);
	UpgradeButton->setPosition(Vector2f(InterfaceArea->getPosition().x - 45.0f, InterfaceArea->getPosition().y + 42.0f));
}





//SPAWN ENEMIES
std::shared_ptr<Entity> spawn_enemy(int level) {
	auto startingPos = LevelSystem::getTilePosition(LevelSystem::findTiles(LevelSystem::START)[0]);
	auto enemy = Engine::GetActiveScene()->makeEntity();
	enemy->addTag("enemy");

	auto spritec = enemy->addComponent<SpriteComponent>();
	auto tex = Resources::get<Texture>("enemies.png");
	spritec->setTexture(tex);
	spritec->getSprite().setTextureRect(sf::IntRect(975, 658, 35, 35));
	spritec->getSprite().setScale(sf::Vector2f(2.0f, 2.0f));

	//ADD ENEMY COMPONENT
	auto ec = enemy->addComponent<EnemyAIComponent>();
	ec->load(level);

	//ADD HP TEXT
	auto healthText = enemy->addComponent<TextComponent>("HP:" + to_string(ec->getHealth()));
	healthText->getText()->setOrigin(healthText->getText()->getLocalBounds().width / 2 - 65.0f, healthText->getText()->getLocalBounds().height / 2 + 10.0f);
	healthText->getText()->setColor(Color::Red);
	healthText->getText()->setCharacterSize(12.0f);
	healthText->getText()->setStyle(sf::Text::Regular);

	
	enemy->setPosition(startingPos);
	return enemy;
}

void executeWave(int wave)
{

}



void createBaseEntity()
{
	auto base = Engine::GetActiveScene()->makeEntity();
	base->addTag("base");
	auto spritec = base->addComponent<SpriteComponent>();
	auto tex = Resources::get<Texture>("base.png");
	spritec->setTexture(tex);
	spritec->getSprite().setTextureRect(sf::IntRect(15, 20, 487, 480));
	spritec->getSprite().setScale(sf::Vector2f(0.4f, 0.4f));


	//ADD HP TEXT
	auto healthText = base->addComponent<TextComponent>("HP:200");
	healthText->getText()->setOrigin(healthText->getText()->getLocalBounds().width / 2 - 110.0f, healthText->getText()->getLocalBounds().height / 2 + 10.0f);
	healthText->getText()->setColor(Color::Red);
	healthText->getText()->setCharacterSize(18.0f);
	healthText->getText()->setStyle(sf::Text::Bold);
	

	base->setPosition(LevelSystem::getTilePosition(LevelSystem::findTiles(LevelSystem::END)[0]) - sf::Vector2f(LevelSystem::getTileSize() / 2 + 80.0f, LevelSystem::getTileSize() / 2 + 135.0f));
}

void createMoneyEntity()
{
	auto money = Engine::GetActiveScene()->makeEntity();
	money->addTag("money");

	auto moneyBackground = money->addComponent<ShapeComponent>();
	moneyBackground->setShape<RectangleShape>(Vector2f(100.0f, 50.0f));
	moneyBackground->getShape().setOrigin(moneyBackground->getShape().getLocalBounds().width / 2, moneyBackground->getShape().getLocalBounds().height / 2);
	moneyBackground->getShape().setFillColor(Color::Color(100, 100, 100, 170));
	

	auto moneyText = money->addComponent<TextComponent>("$100");
	moneyText->getText()->setOrigin(moneyText->getText()->getLocalBounds().width / 2, moneyText->getText()->getLocalBounds().height / 2 + 5.0f);
	moneyText->getText()->setColor(Color::Green);
	moneyText->getText()->setCharacterSize(25.0f);
	moneyText->getText()->setStyle(sf::Text::Bold);

	money->setPosition(Vector2f(Engine::GetWindow().getSize().x / 10 - 80.0f, Engine::GetWindow().getSize().y / 2 + Engine::GetWindow().getSize().y / 4 + Engine::GetWindow().getSize().y / 8));
	
}

void createWaveEntity(int wave)
{
	auto waveEntity = Engine::GetActiveScene()->makeEntity();
	waveEntity->addTag("wave");

	auto waveBackground = waveEntity->addComponent<ShapeComponent>();
	waveBackground->setShape<RectangleShape>(Vector2f(180.0f, 50.0f));
	waveBackground->getShape().setOrigin(waveBackground->getShape().getLocalBounds().width / 2, waveBackground->getShape().getLocalBounds().height / 2);
	waveBackground->getShape().setFillColor(Color::Color(100, 100, 100, 170));

	auto waveText = waveEntity->addComponent<TextComponent>("Wave: " + to_string(wave) + "/3");
	waveText->getText()->setOrigin(waveText->getText()->getLocalBounds().width / 2 - 10.0f, waveText->getText()->getLocalBounds().height / 2 + 5.0f);
	waveText->getText()->setColor(Color::White);
	waveText->getText()->setCharacterSize(25.0f);
	waveText->getText()->setStyle(sf::Text::Bold);

	waveEntity->setPosition(Vector2f(Engine::GetWindow().getSize().x / 10 - 80.0f, Engine::GetWindow().getSize().y / 2 - Engine::GetWindow().getSize().y / 4 - Engine::GetWindow().getSize().y / 8 - 30.0f));
}

void createLevelEntity(int level)
{
	auto levelEntity = Engine::GetActiveScene()->makeEntity();
	levelEntity->addTag("level");

	auto levelBackground = levelEntity->addComponent<ShapeComponent>();
	levelBackground->setShape<RectangleShape>(Vector2f(180.0f, 50.0f));
	levelBackground->getShape().setOrigin(levelBackground->getShape().getLocalBounds().width / 2, levelBackground->getShape().getLocalBounds().height / 2);
	levelBackground->getShape().setFillColor(Color::Color(100, 100, 100, 170));

	auto levelText = levelEntity->addComponent<TextComponent>("Level: " + to_string(level) + "/3");
	levelText->getText()->setOrigin(levelText->getText()->getLocalBounds().width / 2 - 10.0f, levelText->getText()->getLocalBounds().height / 2 + 5.0f);
	levelText->getText()->setColor(Color::White);
	levelText->getText()->setCharacterSize(25.0f);
	levelText->getText()->setStyle(sf::Text::Bold);

	levelEntity->setPosition(Vector2f(Engine::GetWindow().getSize().x / 10 - 80.0f, Engine::GetWindow().getSize().y / 2 - Engine::GetWindow().getSize().y / 4 - Engine::GetWindow().getSize().y / 8 - 100.0f));

}






void createWayPointEntities(int level)
{
	if (level == 1) {
		int wpindex = 0;
		//Hardcoded. Level 1 has a total of 1 waypoints, so we create 1 waypoint entity accordingly

		//----------------------------------------------------WAYPOINT 1------------------------------------------------------
		auto waypoint1 = Engine::GetActiveScene()->makeEntity();
		waypoint1->addTag("waypoint1");
		//debug
		auto tc1 = waypoint1->addComponent<TextComponent>(to_string(wpindex));
		tc1->getText()->setColor(Color::Blue);
		tc1->getText()->setCharacterSize(15.0f);
		tc1->getText()->setStyle(sf::Text::Bold);
		tc1->getText()->setOrigin(tc1->getText()->getLocalBounds().width / 2, tc1->getText()->getLocalBounds().height / 2);
		auto shape1 = waypoint1->addComponent<ShapeComponent>();
		shape1->setShape<CircleShape>(20.0f);
		shape1->getShape().setFillColor(Color::Red);
		//set position of entity
		waypoint1->setPosition(LevelSystem::getTilePosition(LevelSystem::findTiles(LevelSystem::WAYPOINT1)[0]) - sf::Vector2f(LevelSystem::getTileSize() / 2, LevelSystem::getTileSize() / 2));
		wpindex++;
	}
	if (level == 2) {
		int wpindex = 0;
		//Hardcoded. Level 2 has a total of 5 waypoints, so we create 5 waypoint entities accordingly

		//----------------------------------------------------WAYPOINT 1------------------------------------------------------
		auto waypoint1 = Engine::GetActiveScene()->makeEntity();
		waypoint1->addTag("waypoint1");
		//debug
		auto tc1 = waypoint1->addComponent<TextComponent>(to_string(wpindex));
		tc1->getText()->setColor(Color::Blue);
		tc1->getText()->setCharacterSize(15.0f);
		tc1->getText()->setStyle(sf::Text::Bold);
		tc1->getText()->setOrigin(tc1->getText()->getLocalBounds().width / 2, tc1->getText()->getLocalBounds().height / 2);
		auto shape1 = waypoint1->addComponent<ShapeComponent>();
		shape1->setShape<CircleShape>(20.0f);
		shape1->getShape().setFillColor(Color::Red);
		//set position of entity
		waypoint1->setPosition(LevelSystem::getTilePosition(LevelSystem::findTiles(LevelSystem::WAYPOINT1)[0]) - sf::Vector2f(LevelSystem::getTileSize() / 2, LevelSystem::getTileSize() / 2));
		wpindex++;

		//----------------------------------------------------WAYPOINT 2------------------------------------------------------
		auto waypoint2 = Engine::GetActiveScene()->makeEntity();
		waypoint2->addTag("waypoint2");
		//debug
		auto tc2 = waypoint2->addComponent<TextComponent>(to_string(wpindex));
		tc2->getText()->setColor(Color::Blue);
		tc2->getText()->setCharacterSize(15.0f);
		tc2->getText()->setStyle(sf::Text::Bold);
		tc2->getText()->setOrigin(tc2->getText()->getLocalBounds().width / 2, tc2->getText()->getLocalBounds().height / 2);
		auto shape2 = waypoint2->addComponent<ShapeComponent>();
		shape2->setShape<CircleShape>(20.0f);
		shape2->getShape().setFillColor(Color::Red);
		//set position of entity
		waypoint2->setPosition(LevelSystem::getTilePosition(LevelSystem::findTiles(LevelSystem::WAYPOINT2)[0]) - sf::Vector2f(LevelSystem::getTileSize() / 2, LevelSystem::getTileSize() / 2));
		wpindex++;

		//----------------------------------------------------WAYPOINT 3------------------------------------------------------
		auto waypoint3 = Engine::GetActiveScene()->makeEntity();
		waypoint3->addTag("waypoint3");
		//debug
		auto tc3 = waypoint3->addComponent<TextComponent>(to_string(wpindex));
		tc3->getText()->setColor(Color::Blue);
		tc3->getText()->setCharacterSize(15.0f);
		tc3->getText()->setStyle(sf::Text::Bold);
		tc3->getText()->setOrigin(tc3->getText()->getLocalBounds().width / 2, tc3->getText()->getLocalBounds().height / 2);
		auto shape3 = waypoint3->addComponent<ShapeComponent>();
		shape3->setShape<CircleShape>(20.0f);
		shape3->getShape().setFillColor(Color::Red);
		//set position of entity
		waypoint3->setPosition(LevelSystem::getTilePosition(LevelSystem::findTiles(LevelSystem::WAYPOINT3)[0]) - sf::Vector2f(LevelSystem::getTileSize() / 2, LevelSystem::getTileSize() / 2));
		wpindex++;

		//----------------------------------------------------WAYPOINT 4------------------------------------------------------
		auto waypoint4 = Engine::GetActiveScene()->makeEntity();
		waypoint4->addTag("waypoint4");
		//debug
		auto tc4 = waypoint4->addComponent<TextComponent>(to_string(wpindex));
		tc4->getText()->setColor(Color::Blue);
		tc4->getText()->setCharacterSize(15.0f);
		tc4->getText()->setStyle(sf::Text::Bold);
		tc4->getText()->setOrigin(tc4->getText()->getLocalBounds().width / 2, tc4->getText()->getLocalBounds().height / 2);
		auto shape4 = waypoint4->addComponent<ShapeComponent>();
		shape4->setShape<CircleShape>(20.0f);
		shape4->getShape().setFillColor(Color::Red);
		//set position of entity
		waypoint4->setPosition(LevelSystem::getTilePosition(LevelSystem::findTiles(LevelSystem::WAYPOINT4)[0]) - sf::Vector2f(LevelSystem::getTileSize() / 2, LevelSystem::getTileSize() / 2));
		wpindex++;

		//----------------------------------------------------WAYPOINT 5------------------------------------------------------
		auto waypoint5 = Engine::GetActiveScene()->makeEntity();
		waypoint5->addTag("waypoint5");
		//debug
		auto tc5 = waypoint5->addComponent<TextComponent>(to_string(wpindex));
		tc5->getText()->setColor(Color::Blue);
		tc5->getText()->setCharacterSize(15.0f);
		tc5->getText()->setStyle(sf::Text::Bold);
		tc5->getText()->setOrigin(tc5->getText()->getLocalBounds().width / 2, tc5->getText()->getLocalBounds().height / 2);
		auto shape5 = waypoint5->addComponent<ShapeComponent>();
		shape5->setShape<CircleShape>(20.0f);
		shape5->getShape().setFillColor(Color::Red);
		//set position of entity
		waypoint5->setPosition(LevelSystem::getTilePosition(LevelSystem::findTiles(LevelSystem::WAYPOINT5)[0]) - sf::Vector2f(LevelSystem::getTileSize() / 2, LevelSystem::getTileSize() / 2));
		wpindex++;
	}

}
