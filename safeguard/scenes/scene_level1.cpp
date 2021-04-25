#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_button.h"
#include "../game.h"
#include "../gamestuff.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;


int money = 1000;

sf::Time delay;
sf::Clock timer;
float changeDelay;

void Level1Scene::Load() {
  cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/level1TEST.txt", 32.0f);

  changeDelay = 1.0f;
 

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


  
  _clickTimeout = 1.0f; //sensible timeout for the buttons
  setLoaded(true);

}



void Level1Scene::UnLoad() {
  cout << "Scene 1 Unload" << endl;
  ls::unload();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {

    
    if (_clickTimeout >= 0.0f) _clickTimeout -= dt;

    if (_clickTimeout < 0.0f) {
        if (_purchase_attacktower_btn->get_components<ButtonComponent>()[0]->isSelected() && money > 5) {
            towers.push_back(create_tower_ATTACK());
            money -= 5;
            _clickTimeout = 1.0f; //reset the timer after every button click
            cout << towers.size() << +"VECTOR SIZE" << endl;
            cout << money << +"GOLD" << endl;
           
        }
    }
    
    
    if (LevelSystem::getTileAt(Engine::GetWindow().mapPixelToCoords(Mouse::getPosition(Engine::GetWindow()))) == LevelSystem::TOWERSPOTS) {
        cout << "Indeed a tower spot!";
    }
    
  
  

   Scene::Update(dt);
    
}

void Level1Scene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
