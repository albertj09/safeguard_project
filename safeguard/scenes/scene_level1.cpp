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
  _purchase_attacktower_btn = create_purchase_tower_button_ATTACK("Attack\nTower\n  $5");


  //ADDITIONAL VARIABLES
  _clickTimeout = 0.5f; //sensible timeout for the buttons
  _towerBeingPlaced = false;
  _index = 0;
  _upgradeInterfaceOpen = false;
  

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
    if (_towerBeingPlaced == true && !_towerSets.empty()) {        
         _towerSets[_index].entityobj.get()->setPosition(cursorPos);
    }

   

    //updateFirerates

    if (!_towerSets.empty()) {
        for (auto set : _towerSets) {
            set.towerobj->updateTime(dt);
        }
    }
    
    

    //Towers shoot based on their respective attributes
    if (!_towerSets.empty()) {
        for (auto set : _towerSets) {
            if (set.towerobj->getFireRateStatus() < 0.0f && set.towerobj->getCanFire()) {
                set.towerobj->create_tower_bullet(set.entityobj.get(), normalize(sf::Vector2f(1.f, 1.f)));
            }
        }
    }
    
    

    if (_clickTimeout < 0.0f) {

        //CLicking on the purchase attack tower button
        if (_purchase_attacktower_btn->get_components<ButtonComponent>()[0]->isSelected() && money > 5 && _towerBeingPlaced == false) {

            AttackTower* new_attack_tower = new AttackTower();  //initialize the memory of the object. These objects must be deleted between each scene switch to avoid the memory leaks.


            //----------------------------------------------DEFAULT STARTING VALUES FOR ATTACK TOWERS----------------------------------------------
            new_attack_tower->setBaseFireRate(4.0f);    
            new_attack_tower->setCanFire(false);
            new_attack_tower->setRange(200.0f);
            //-------------------------------------------------------------------------------------------------------------------------------------

            //entities
            auto newtower = new_attack_tower->create_tower();
            

            //create and add a new tower set (tower obj + its' entity)
            towerSets newset;
            newset.towerobj = new_attack_tower;
            newset.entityobj = newtower;         
            _towerSets.push_back(newset);
            cout << "towerSets size: " << + _towerSets.size() << endl;
            
            
            
            //misc
            _towerBeingPlaced = true;
            _clickTimeout = 0.5f; //reset the timer after every button click     
        }

        //Clicking on an existing tower
        if (Mouse::isButtonPressed(Mouse::Left) && LevelSystem::getTileAt(cursorPos) == LevelSystem::TOWERSPOTS && !_towerBeingPlaced) {
            for (auto s : _attackTowerMappingSets) {
                if (LevelSystem::getTilePosition(Vector2ul(Vector2f(cursorPos.x, cursorPos.y) / LevelSystem::getTileSize())) == s.position) {

                    //CLOSE THE PREVIOUS UPGRADE INTERFACE INSTANCE
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

                    create_upgradeInterface_ATTACKTOWER(s.sets.towerobj, s.sets.entityobj->getPosition());
                    _upgradeInterfaceOpen = true;
                    cout << "Upgrade interface opened!" << endl;
                    _clickTimeout = 0.5f;
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
            _towerSets[_index].entityobj->setForDelete(); //delete the tower entity itself
            delete(_towerSets[_index].towerobj);    //delete the tower object itself
            _towerSets.erase(_towerSets.begin() + _index);  //delete the vector entry and resize it
            _towerBeingPlaced = false;
            _clickTimeout = 0.5f;
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
                    _towerSets[_index].entityobj->setPosition(location);                                  

                    //allow the tower to fire once it has been placed
                    _towerSets[_index].towerobj->setCanFire(true);  
                 
                    //update gold amount
                    money -= 5;

                    //debug info
                    cout << "\n\nIndeed a tower spot! Tower placed!" << endl;
                    cout << "Number of total towers: " << +_towerSets.size() << endl;
                    cout << "Gold: " << +money << endl;

                    //Update Attack Tower Mapping Set
                    mappingAttackTowerSets newMappingAttackTowerSet;
                    newMappingAttackTowerSet.position = LevelSystem::getTilePosition(Vector2ul(Vector2f(cursorPos.x, cursorPos.y) / LevelSystem::getTileSize()));
                    newMappingAttackTowerSet.sets = _towerSets[_index];
                
                    //misc
                    _attackTowerMappingSets.push_back(newMappingAttackTowerSet);
                    _towerCoords.push_back(LevelSystem::getTilePosition(Vector2ul(Vector2f(cursorPos.x, cursorPos.y) / LevelSystem::getTileSize())));
                    _index++;
                    _towerBeingPlaced = false;
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
