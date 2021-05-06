#pragma once

#include "engine.h"
#include <LevelSystem.h>
#include <algorithm>
#include "../gamestuff.h"
#include <SFML/Audio.hpp>









//struct locationTypes {
//	sf::Vector2f location;
//	std::string type;
//};
//
////-----------------ATTACK TOWER STRUCTURES------------------
//struct attackTowerSets {
//	AttackTower* towerobj;
//	std::shared_ptr<Entity> entityobj;
//};
//
//struct mappingAttackTowerSets {
//	sf::Vector2f position;
//	attackTowerSets sets;
//};
////----------------------------------------------------------
//
////----------------AIR TOWER STRUCTURES----------------------
//struct airTowerSets {
//	AirTower* towerobj;
//	std::shared_ptr<Entity> entityobj;
//};
//struct mappingAirTowerSets {
//	sf::Vector2f position;
//	airTowerSets sets;
//};
//-----------------------------------------------------------



class Level1Scene : public Scene {
public:
	
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

private:

	//SOUNDS
	//purchase
	sf::SoundBuffer _bufferSoundPurchase;
	sf::Sound _soundPurchase;
	//enemy death
	sf::SoundBuffer _bufferSoundDeath;
	sf::Sound _soundDeath;
	//base hit
	sf::SoundBuffer _bufferSoundHit;
	sf::Sound _soundBaseHit;

	


	//WAYPOINTS
	std::vector<sf::Vector2f> _waypoints;
	std::shared_ptr<std::vector<Entity>> _wayPointsEntities;

	//ENEMIES
	std::vector<std::shared_ptr<Entity>> _enemies;
	

	//TOWER COORDINATES
	std::vector<sf::Vector2f> _towerCoords;
	std::vector<locationTypes> _locationTypes;
	sf::Vector2f _currentSelectedTower;

	
	//ATTACK TOWER MAPPING SETS 
	std::vector<attackTowerSets> _attackTowerSets;
	std::vector<mappingAttackTowerSets> _attackTowerMappingSets;

	//AIR TOWER MAPPING SETS 
	std::vector<airTowerSets> _airTowerSets;
	std::vector<mappingAirTowerSets> _airTowerMappingSets;

	//BOMB TOWER MAPPING SETS 


	//OTHER VARIABLES
	float _clickTimeout;
	float _spawnTimeout;
	bool _towerBeingPlaced;
	int _indexAttack;
	int _indexAir;
	int _indexBomb;
	bool _upgradeInterfaceOpen;
	bool _buyInterfaceOpen;
	int _level = 1;
	int _wave;
	std::string _selectedTowerTypeWhenPlacing;
	std::string _selectedTowerTypeWhenClicked;

	//BASE HEALTH
	int _baseHealth = 200;
	int _money;

	//MISC
	int _wave_1_amount = 2;
	int _wave_2_amount = 15;
	int _wave_3_amount = 20;

	int _wave_1_enemiesSpawned;
	int _wave_2_enemiesSpawned;
	int _wave_3_enemiesSpawned;
	bool _nextScene;
		
	
};
