#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_button.h"
#include "../game.h"
#include "../gamestuff.h"
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


  
  _clickTimeout = 0.5f; //sensible timeout for the buttons
  _shootingDelay = 2.0f;
  _towerBeingPlaced = false;
  _index = 0;


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

    if (_shootingDelay >= 0.0f) _shootingDelay -= dt;

    //Match the tower to the mouse cursor while the player is placing it
    if (_towerBeingPlaced == true) {
        _towers[_index].get()->setPosition(cursorPos);

    }

    //Towers shoot
    if (_shootingDelay < 0.0f) {
        if (_towers.size() == _attackTowers.size() && !_towers.empty() && !_attackTowers.empty()) {
            for (auto set : _towerSets) {
                set.towerobj->create_tower_bullet(set.entityobj.get(), normalize(sf::Vector2f(1.f, 1.f)));
            }
        }
        _shootingDelay = 2.0f;
    }
    

    if (_clickTimeout < 0.0f) {

        //CLicking on the purchase attack tower button
        if (_purchase_attacktower_btn->get_components<ButtonComponent>()[0]->isSelected() && money > 5 && _towerBeingPlaced == false) {

            std::shared_ptr<AttackTower> new_attack_tower;

            //attack tower objects
            _attackTowers.push_back(new_attack_tower);

            //entities
            auto newtower = new_attack_tower->create_tower();
            _towers.push_back(newtower);

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

        
        //Clicking on a tower tile
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
                    _towers[_index].get()->setPosition(location);



                    //update gold amount
                    money -= 5;

                    //debug info
                    cout << "\n\nIndeed a tower spot! Tower placed!" << endl;
                    cout << "Number of total towers: " << +_towers.size() << endl;
                    cout << "Gold: " << +money << endl;

                    //misc
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
