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

std::vector<std::shared_ptr<Entity>> towers;
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




  _purchase_attacktower_btn = create_purchase_tower_button("Attack\nTower\n  $5");
  _attack_tower = create_tower();


  
  _clickTimeout = 0.2f;
  setLoaded(true);

}



void Level1Scene::UnLoad() {
  cout << "Scene 1 Unload" << endl;
  ls::unload();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {

    
   
    
    
     
    
  
  if (_purchase_attacktower_btn->get_components<ButtonComponent>()[0]->isSelected() && money > 5) {
      towers.push_back(_attack_tower);
      money -= 5;
      cout << towers.size() << + "VECTOR SIZE" << endl;
      cout << money << + "GOLD" << endl;     
  }

   Scene::Update(dt);
    
}

void Level1Scene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
