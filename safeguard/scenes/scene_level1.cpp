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

void Level1Scene::Load() {
  cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/level1TEST.txt", 32.0f);

  ////Create a tower
  //tower = makeEntity();
  //tower->setPosition(Vector2f(20.0f, 20.0f));
  //

  ////Load the texture for wall sprites
  //sf::Texture defaultTexture;

  //if (!defaultTexture.loadFromFile("res/img/stone_tile.png")) {
  //    cerr << "Failed to load texture!" << std::endl;
  //};
  //
  



  

  // Create player
  /*{
    player = makeEntity();
    player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
    auto s = player->addComponent<ShapeComponent>();
    s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
    s->getShape().setFillColor(Color::Magenta);
    s->getShape().setOrigin(10.f, 15.f);

    player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
    
  }*/

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

  //Simulate long loading times
  //std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  //cout << " Scene 1 Load Done" << endl;



  _purchase_attacktower_btn = create_purchase_tower_button("Attack\nTower\n  $5");
  
  _clickTimeout = 1.0f;
  setLoaded(true);

  
  
  



  
  

  

}



void Level1Scene::UnLoad() {
  cout << "Scene 1 Unload" << endl;
  ls::unload();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {

    if (_clickTimeout >= 0.0f) _clickTimeout -= dt;
    
    if (_clickTimeout < 0.00000f) {
        if (_purchase_attacktower_btn->get_components<ButtonComponent>().at(0)->isSelected() && money > 5) {
            towers.push_back(create_tower());
            money -= 5;
            cout << towers.size() << + "VECTOR SIZE" << endl;
            cout << money << + "GOLD" << endl;
        }
    }

    
    Scene::Update(dt);
}

void Level1Scene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
