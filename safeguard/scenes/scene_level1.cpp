#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_button.h"
#include "../game.h"
#include <iostream>
#include <thread>


using namespace std;
using namespace sf;


int money = 1000;



void Level1Scene::Load() {
  cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/level1TEST.txt", 32.0f);

  
 

  // Add physics colliders to level tiles.
  //{
  //  auto walls = ls::findTiles(ls::WALL);
  //  for (auto w : walls) {
  //    auto pos = ls::getTilePosition(w);
  //    pos += Vector2f(20.f, 20.f); //offset to center
  //    auto e = makeEntity();
  //    e->setPosition(pos);
  //    e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
  //  }
  //}


  //PURCHASE BUTTONS
  //_purchase_attacktower_btn = create_purchase_tower_button_ATTACK("Attack\nTower\n  $5");


  //ADDITIONAL VARIABLES
  _clickTimeout = 0.5f; //sensible timeout for the buttons
  _towerBeingPlaced = false;
  _indexAttack = 0;
  _indexAir = 0;
  _indexBomb = 0;
  _upgradeInterfaceOpen = false;
  _buyInterfaceOpen = false;
  

  setLoaded(true);

}





void Level1Scene::UnLoad() {
  cout << "Scene 1 Unload" << endl;
  ls::unload();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {

    Vector2f cursorPos = Engine::GetWindow().mapPixelToCoords(Mouse::getPosition(Engine::GetWindow()));
    
    //Clicking timeout
    if (_clickTimeout >= 0.0f) _clickTimeout -= dt;


    //Match the tower to the mouse cursor while the player is placing it
    if (_selectedTowerTypeWhenPlacing == "attack" && _towerBeingPlaced == true && !_attackTowerSets.empty()) {
        _attackTowerSets[_indexAttack].entityobj->setPosition(cursorPos);       
    }

    if (_selectedTowerTypeWhenPlacing == "air" && _towerBeingPlaced == true && !_airTowerSets.empty()) { 
        _airTowerSets[_indexAir].entityobj->setPosition(cursorPos);       
    }
    

   

    //ATTACK tower update firerates
    if (!_attackTowerSets.empty()) {
        for (auto set : _attackTowerSets) {
            set.towerobj->updateTime(dt);
        }
    }

    //AIR tower update firerates
    if (!_airTowerSets.empty()) {
        for (auto set : _airTowerSets) {
            set.towerobj->updateTime(dt);
        }
    }
    
    

    //ATTACK towers shoot based on their respective attributes
    if (!_attackTowerSets.empty()) {
        for (auto set : _attackTowerSets) {
            if (set.towerobj->getFireRateStatus() < 0.0f && set.towerobj->getCanFire()) {
                auto bullet = set.towerobj->create_tower_bullet(set.entityobj.get(), normalize(sf::Vector2f(1.f, 1.f)));
                bullet->get_components<BulletComponent>()[0]->setBulletDamage(set.towerobj->getDamage());
            }
        }
    }

    //AIR towers shoot based on their respective attributes
    if (!_airTowerSets.empty()) {
        for (auto set : _airTowerSets) {
            if (set.towerobj->getFireRateStatus() < 0.0f && set.towerobj->getCanFire()) {
                auto bulletAir = set.towerobj->create_tower_bullet(set.entityobj.get(), normalize(sf::Vector2f(1.f, 1.f)));
                bulletAir->get_components<BulletComponent>()[0]->setBulletDamage(set.towerobj->getDamage());
            }
        }
    }
    
    

    if (_clickTimeout < 0.0f) {

        //Toggle open/close the buying interface for the towers
        if (Keyboard::isKeyPressed(Keyboard::B)) {
            if (!_buyInterfaceOpen) {
                createBuyInterface();
                _buyInterfaceOpen = true;
                cout << "buying interface opened!" << endl;
                _clickTimeout = 0.5f;
            }
            else if (_buyInterfaceOpen) {
                for (auto e : Engine::GetActiveScene()->ents.find("buyInterface")) {
                    e->setForDelete();
                }
                for (auto e : Engine::GetActiveScene()->ents.find("purchase_tower_button_ATTACK")) {
                    e->setForDelete();
                }
                for (auto e : Engine::GetActiveScene()->ents.find("purchase_tower_button_AIR")) {
                    e->setForDelete();
                }

                _buyInterfaceOpen = false;
                cout << "buying interface closed!" << endl;
                _clickTimeout = 0.5f;
            }          
        }

        //When the buying interface is opened it is possible to purchase towers

        //ATTACK TOWER PURCHASING
        if (!Engine::GetActiveScene()->ents.find("purchase_tower_button_ATTACK").empty()) {
            if (Engine::GetActiveScene()->ents.find("purchase_tower_button_ATTACK")[0]->get_components<ButtonComponent>()[0]->isSelected() && Mouse::isButtonPressed(Mouse::Left) && _towerBeingPlaced == false) {
                AttackTower* new_attack_tower = new AttackTower();  //initialize the memory of the object. These objects must be deleted between each scene switch to avoid memory leaks.


                //----------------------------------------------DEFAULT STARTING VALUES FOR ATTACK TOWERS----------------------------------------------
                new_attack_tower->setBaseFireRate(4.0f);
                new_attack_tower->setCanFire(false);    //make sure that the tower doesn't shoot while it's being placed
                new_attack_tower->setRange(200.0f);
                new_attack_tower->setUpgradeLevel(1);
                new_attack_tower->setDamage(1.0f);
                new_attack_tower->setTowerType("attack");
                //-------------------------------------------------------------------------------------------------------------------------------------

                //entities
                auto newtower = new_attack_tower->create_tower();


                //create and add a new tower set (tower obj + its' entity)
                attackTowerSets newset;
                newset.towerobj = new_attack_tower;
                newset.entityobj = newtower;
                _attackTowerSets.push_back(newset);
                cout << "towerSets size: " << +_attackTowerSets.size() << endl;

                //misc
                _towerBeingPlaced = true;
                _selectedTowerTypeWhenPlacing = "attack";
                _clickTimeout = 0.5f; //reset the timer after every button click     
            }
        }

        //AIR TOWER PURCHASING
        if (!Engine::GetActiveScene()->ents.find("purchase_tower_button_AIR").empty()) {
            if (Engine::GetActiveScene()->ents.find("purchase_tower_button_AIR")[0]->get_components<ButtonComponent>()[0]->isSelected() && Mouse::isButtonPressed(Mouse::Left) && _towerBeingPlaced == false) {
                AirTower* new_air_tower = new AirTower();  //initialize the memory of the object. These objects must be deleted between each scene switch to avoid memory leaks.


                //----------------------------------------------DEFAULT STARTING VALUES FOR AIR TOWERS----------------------------------------------
                new_air_tower->setBaseFireRate(3.0f);
                new_air_tower->setCanFire(false);    //make sure that the tower doesn't shoot while it's being placed
                new_air_tower->setRange(300.0f);
                new_air_tower->setUpgradeLevel(1);
                new_air_tower->setDamage(1.0f);
                new_air_tower->setTowerType("air");
                //-------------------------------------------------------------------------------------------------------------------------------------

                //entities
                auto newtower = new_air_tower->create_tower();


                //create and add a new tower set (tower obj + its' entity)
                airTowerSets newset;
                newset.towerobj = new_air_tower;
                newset.entityobj = newtower;
                _airTowerSets.push_back(newset);

                //misc
                _towerBeingPlaced = true;
                _selectedTowerTypeWhenPlacing = "air";
                _clickTimeout = 0.5f; //reset the timer after every button click     
            }
        }

        


        //clicking on upgrade button
        if (!Engine::GetActiveScene()->ents.find("upgradeButton").empty()) {
            if (Engine::GetActiveScene()->ents.find("upgradeButton")[0]->get_components<ButtonComponent>()[0]->isSelected() && money >= 15 && _upgradeInterfaceOpen && Mouse::isButtonPressed(Mouse::Left)) {
                if (_selectedTowerTypeWhenClicked == "attack") {
                    for (auto s : _attackTowerMappingSets) {
                        if (s.position == _currentSelectedTower) {
                            if (s.sets.towerobj->getUpgradeLevel() < s.sets.towerobj->getMaxUpgradeLevel()) {
                                //increment the range
                                s.sets.towerobj->setRange(s.sets.towerobj->getRange() + 20.0f);
                                //making sure that the fastest firerate can be is 1.0f
                                if (s.sets.towerobj->getBaseFireRate() >= 2.0f) {
                                    //inrement the attack speed (by reducing the interval between shots)
                                    s.sets.towerobj->setBaseFireRate(s.sets.towerobj->getBaseFireRate() - 1.0f);
                                }
                                //increment the upgrade level
                                s.sets.towerobj->setUpgradeLevel(s.sets.towerobj->getUpgradeLevel() + 1);

                                //increment the damage
                                s.sets.towerobj->setDamage(s.sets.towerobj->getDamage() + 1.0f);

                                //visual upgrade
                                s.sets.towerobj->visualUpgrade(s.sets.entityobj);

                                //--------------------------SIMULATE THE REFRESHING OF THE VALUES BY RELOADING THE INTERFACE------------------------

                                for (auto e : Engine::GetActiveScene()->ents.find("upgradeInterface")) {
                                    e->setForDelete();
                                }
                                for (auto e : Engine::GetActiveScene()->ents.find("upgradeButton")) {
                                    e->setForDelete();
                                }

                                cout << "deleted previous interface" << endl;

                                create_upgradeInterface_ATTACKTOWER(s.sets.towerobj, s.sets.entityobj->getPosition());
                                //------------------------------------------------------------------------------------------------------------------

                                cout << "tower upgraded!" << endl;
                                money -= 15;
                                _clickTimeout = 0.5f;
                            }
                        }
                    }
                }
                else if (_selectedTowerTypeWhenClicked == "air") {
                    for (auto s : _airTowerMappingSets) {
                        if (s.position == _currentSelectedTower) {
                            if (s.sets.towerobj->getUpgradeLevel() < s.sets.towerobj->getMaxUpgradeLevel()) {
                                //increment the range
                                s.sets.towerobj->setRange(s.sets.towerobj->getRange() + 20.0f);
                                //making sure that the fastest firerate can be is 1.0f
                                if (s.sets.towerobj->getBaseFireRate() >= 2.0f) {
                                    //inrement the attack speed (by reducing the interval between shots)
                                    s.sets.towerobj->setBaseFireRate(s.sets.towerobj->getBaseFireRate() - 1.0f);
                                }
                                //increment the upgrade level
                                s.sets.towerobj->setUpgradeLevel(s.sets.towerobj->getUpgradeLevel() + 1);

                                //increment the damage
                                s.sets.towerobj->setDamage(s.sets.towerobj->getDamage() + 1.0f);

                                //visual upgrade
                                s.sets.towerobj->visualUpgrade(s.sets.entityobj);

                                //--------------------------SIMULATE THE REFRESHING OF THE VALUES BY RELOADING THE INTERFACE------------------------

                                for (auto e : Engine::GetActiveScene()->ents.find("upgradeInterface")) {
                                    e->setForDelete();
                                }
                                for (auto e : Engine::GetActiveScene()->ents.find("upgradeButton")) {
                                    e->setForDelete();
                                }

                                cout << "deleted previous interface" << endl;

                                create_upgradeInterface_AIRTOWER(s.sets.towerobj, s.sets.entityobj->getPosition());
                                //------------------------------------------------------------------------------------------------------------------

                                cout << "tower upgraded!" << endl;
                                money -= 15;
                                _clickTimeout = 0.5f;
                            }
                        }
                    }
                }             
            }
        }
        

        //Clicking on an existing tower
        if (LevelSystem::isOnGrid(cursorPos)) {
            if (Mouse::isButtonPressed(Mouse::Left) && LevelSystem::getTileAt(cursorPos) == LevelSystem::TOWERSPOTS && !_towerBeingPlaced) {
                for (auto lt : _locationTypes) {
                    if (LevelSystem::getTilePosition(Vector2ul(Vector2f(cursorPos.x, cursorPos.y) / LevelSystem::getTileSize())) == lt.location) {
                        //SET THE TYPE
                        _selectedTowerTypeWhenClicked = lt.type;

                        //IF TYPE IS ATTACK TOWER
                        if (_selectedTowerTypeWhenClicked == "attack") {
                            for (auto s : _attackTowerMappingSets) {
                                if (LevelSystem::getTilePosition(Vector2ul(Vector2f(cursorPos.x, cursorPos.y) / LevelSystem::getTileSize())) == s.position) {

                                    //----------------------CLOSE THE PREVIOUS UPGRADE INTERFACE INSTANCE-------------------
                                    if (_upgradeInterfaceOpen) {
                                        for (auto e : Engine::GetActiveScene()->ents.find("upgradeInterface")) {
                                            e->setForDelete();
                                        }
                                        for (auto e : Engine::GetActiveScene()->ents.find("upgradeButton")) {
                                            e->setForDelete();
                                        }

                                        _upgradeInterfaceOpen = false;
                                        cout << "deleted previous interface" << endl;
                                    }
                                    //--------------------------------------------------------------------------------------

                                    //----------------------CREATE A NEW INTERFACE INSTANCE---------------------------------
                                    create_upgradeInterface_ATTACKTOWER(s.sets.towerobj, s.sets.entityobj->getPosition());
                                    //--------------------------------------------------------------------------------------

                                    _currentSelectedTower = s.position;
                                    _upgradeInterfaceOpen = true;
                                    cout << "Upgrade interface opened!" << endl;
                                    _clickTimeout = 0.5f;
                                }
                            }
                        }
                        //IF TYPE IS AIR TOWER
                        if (_selectedTowerTypeWhenClicked == "air") {
                            for (auto s : _airTowerMappingSets) {
                                if (LevelSystem::getTilePosition(Vector2ul(Vector2f(cursorPos.x, cursorPos.y) / LevelSystem::getTileSize())) == s.position) {

                                    //----------------------CLOSE THE PREVIOUS UPGRADE INTERFACE INSTANCE-------------------
                                    if (_upgradeInterfaceOpen) {
                                        for (auto e : Engine::GetActiveScene()->ents.find("upgradeInterface")) {
                                            e->setForDelete();
                                        }
                                        for (auto e : Engine::GetActiveScene()->ents.find("upgradeButton")) {
                                            e->setForDelete();
                                        }

                                        _upgradeInterfaceOpen = false;
                                        cout << "deleted previous interface" << endl;
                                    }
                                    //--------------------------------------------------------------------------------------

                                    //----------------------CREATE A NEW INTERFACE INSTANCE---------------------------------
                                    create_upgradeInterface_AIRTOWER(s.sets.towerobj, s.sets.entityobj->getPosition());
                                    //--------------------------------------------------------------------------------------

                                    _currentSelectedTower = s.position;
                                    _upgradeInterfaceOpen = true;
                                    cout << "Upgrade interface opened!" << endl;
                                    _clickTimeout = 0.5f;
                                }
                            }
                        }

                    }
                }                                
            }
        }
        

        //Right clicking or left clicking anywhere on the map while the upgrade interface is open closes it and cleans up its' respective entities
        if (Mouse::isButtonPressed(Mouse::Right) && !_towerBeingPlaced && _upgradeInterfaceOpen) {
            for (auto e : Engine::GetActiveScene()->ents.find("upgradeInterface")) {
                e->setForDelete();
            }
            for (auto e : Engine::GetActiveScene()->ents.find("upgradeButton")) {
                e->setForDelete();
            }

            cout << "deleted opened interface with right click" << endl;
            _upgradeInterfaceOpen = false;
            _clickTimeout = 0.5f;
        }


        //Cancel buying a tower
        if (Mouse::isButtonPressed(Mouse::Right) && _towerBeingPlaced) {
            if (_selectedTowerTypeWhenPlacing == "attack") {
                _attackTowerSets[_indexAttack].entityobj->setForDelete(); //delete the tower entity itself
                delete(_attackTowerSets[_indexAttack].towerobj);    //delete the tower object itself
                _attackTowerSets.erase(_attackTowerSets.begin() + _indexAttack);  //delete the vector entry and resize it
                _towerBeingPlaced = false;
                _clickTimeout = 0.5f;
            }
            else if (_selectedTowerTypeWhenPlacing == "air") {
                _airTowerSets[_indexAir].entityobj->setForDelete(); //delete the tower entity itself
                delete(_airTowerSets[_indexAir].towerobj);    //delete the tower object itself
                _airTowerSets.erase(_airTowerSets.begin() + _indexAir);  //delete the vector entry and resize it
                _towerBeingPlaced = false;
                _clickTimeout = 0.5f;
            }            
        }

       
        
        //Placing the tower
        if (LevelSystem::isOnGrid(cursorPos) && _towerBeingPlaced) {
            if (LevelSystem::getTileAt(cursorPos) == LevelSystem::TOWERSPOTS && Mouse::isButtonPressed(Mouse::Left)) {
                if (std::find(_towerCoords.begin(), _towerCoords.end(), LevelSystem::getTilePosition(Vector2ul(Vector2f(cursorPos.x, cursorPos.y) / LevelSystem::getTileSize()))) != _towerCoords.end()) {
                    cout << "There's already a tower built here!" << endl;
                    _clickTimeout = 0.5f;
                }
                else {
                    //Set the location of the tower to the center of the tower spot tile
                    Vector2f location = LevelSystem::getTilePosition(Vector2ul(Vector2f(cursorPos.x, cursorPos.y) / LevelSystem::getTileSize()));
                    location.x -= LevelSystem::getTileSize();
                    location.y -= LevelSystem::getTileSize();

                    if (_selectedTowerTypeWhenPlacing == "attack") {
                        cout << "placed ATTACK tower" << endl;
                        _attackTowerSets[_indexAttack].entityobj->setPosition(location);
                        //allow the tower to fire once it has been placed
                        _attackTowerSets[_indexAttack].towerobj->setCanFire(true);
                        //update gold amount
                        money -= 20;

                        //Update Attack Tower Mapping Set
                        mappingAttackTowerSets newMappingAttackTowerSet;
                        newMappingAttackTowerSet.position = LevelSystem::getTilePosition(Vector2ul(Vector2f(cursorPos.x, cursorPos.y) / LevelSystem::getTileSize()));
                        newMappingAttackTowerSet.sets = _attackTowerSets[_indexAttack];
                        _attackTowerMappingSets.push_back(newMappingAttackTowerSet);

                        //update ATTACK tower index
                        _indexAttack++;
                    }
                    else if (_selectedTowerTypeWhenPlacing == "air") {
                        cout << "placed AIR tower" << endl;
                        location.y -= 30.0f;
                        _airTowerSets[_indexAir].entityobj->setPosition(location);
                        //allow the tower to fire once it has been placed
                        _airTowerSets[_indexAir].towerobj->setCanFire(true);
                        //update gold amount
                        money -= 25;

                        //Update Attack Tower Mapping Set
                        mappingAirTowerSets newMappingAirTowerSet;
                        newMappingAirTowerSet.position = LevelSystem::getTilePosition(Vector2ul(Vector2f(cursorPos.x, cursorPos.y) / LevelSystem::getTileSize()));
                        newMappingAirTowerSet.sets = _airTowerSets[_indexAir];
                        _airTowerMappingSets.push_back(newMappingAirTowerSet);

                        //update ATTACK tower index
                        _indexAir++;
                    }
                                             
                    //add location to the list of tower coordinates                   
                    _towerCoords.push_back(LevelSystem::getTilePosition(Vector2ul(Vector2f(cursorPos.x, cursorPos.y) / LevelSystem::getTileSize())));
                    
                    //add a new location type
                    locationTypes newLocationType;
                    newLocationType.location = LevelSystem::getTilePosition(Vector2ul(Vector2f(cursorPos.x, cursorPos.y) / LevelSystem::getTileSize()));
                    newLocationType.type = _selectedTowerTypeWhenPlacing;
                    _locationTypes.push_back(newLocationType);

                    //update global variables
                    _towerBeingPlaced = false;
                    _selectedTowerTypeWhenPlacing = "none";
                    _clickTimeout = 0.5f;
                }                              
            }
            else if(Mouse::isButtonPressed(Mouse::Left) && LevelSystem::getTileAt(cursorPos) != LevelSystem::TOWERSPOTS) {
                cout << "Can't build a tower here!" << endl;
                _clickTimeout = 0.5f;
            }
        }
    }
    
    
    
    
  
  

   Scene::Update(dt);
    
}

void Level1Scene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
