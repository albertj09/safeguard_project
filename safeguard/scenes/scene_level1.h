#pragma once

#include "engine.h"
#include <LevelSystem.h>
#include <algorithm>
#include "../gamestuff.h"


struct locationTypes {
	sf::Vector2f location;
	std::string type;
};



//-----------------ATTACK TOWER STRUCTURES------------------
struct attackTowerSets {
	AttackTower* towerobj;
	std::shared_ptr<Entity> entityobj;
};

struct mappingAttackTowerSets {
	sf::Vector2f position;
	attackTowerSets sets;
};
//----------------------------------------------------------

//----------------AIR TOWER STRUCTURES----------------------
struct airTowerSets {
	AirTower* towerobj;
	std::shared_ptr<Entity> entityobj;
};
struct mappingAirTowerSets {
	sf::Vector2f position;
	airTowerSets sets;
};
//-----------------------------------------------------------



class Level1Scene : public Scene {
public:
	
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

private:

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
	bool _towerBeingPlaced;
	int _indexAttack;
	int _indexAir;
	int _indexBomb;
	bool _upgradeInterfaceOpen;
	bool _buyInterfaceOpen;
	std::string _selectedTowerTypeWhenPlacing;
	std::string _selectedTowerTypeWhenClicked;
	
		
	
};
