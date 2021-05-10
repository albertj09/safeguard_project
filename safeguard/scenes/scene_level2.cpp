#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_button.h"
#include "../game.h"
#include <iostream>
#include <thread>


using namespace std;
using namespace sf;






void Level2Scene::Load() {
    cout << " Scene 2 Load" << endl;
    ls::loadLevelFile("res/level_2.txt", 32.0f);




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


    //LOAD PURCHASE SOUND
    _bufferSoundPurchase.loadFromFile("res/purchase.wav");
    _soundPurchase.setBuffer(_bufferSoundPurchase);
    _soundPurchase.setVolume(10);

    //LOAD ENEMY DEATH SOUND
    _bufferSoundDeath.loadFromFile("res/enemy_death.flac");
    _soundDeath.setBuffer(_bufferSoundDeath);
    _soundDeath.setVolume(3);

    //LOAD BASE HIT SOUND
    _bufferSoundHit.loadFromFile("res/hit.wav");
    _soundBaseHit.setBuffer(_bufferSoundHit);



    //_soundBaseHit.play();

    createWayPointEntities(_level);

    //_soundPurchase = Sound(*Resources::get<SoundBuffer>("purchase.wav"));



    //spawn_enemy(_level);   //Specify which level

    createBaseEntity();

    createMoneyEntity();

    createLevelEntity(_level);

    createWaveEntity(1);


    //ADDITIONAL VARIABLES
    _clickTimeout = 0.5f; //sensible timeout for the buttons
    _spawnTimeout = 10.0f; //Initial timeout is 10 seconds before the first wave starts
    _towerBeingPlaced = false;
    _indexAttack = 0;
    _indexAir = 0;
    _indexBomb = 0;
    _upgradeInterfaceOpen = false;
    _buyInterfaceOpen = false;

    _selectedTowerTypeWhenPlacing = "none";
    _selectedTowerTypeWhenClicked = "none";

    _money = 100;
    _wave = 1;

    _wave_1_enemiesSpawned = 0;
    _wave_2_enemiesSpawned = 0;
    _wave_3_enemiesSpawned = 0;

    _nextScene = false;
    _paused = false;
    _pauseClickTimeout = 0.5f;
    _goToMainMenu = false;
    _gameOver = false;


    setLoaded(true);

}





void Level2Scene::UnLoad() {
    cout << "Scene 2 Unload" << endl;
    for (auto s : _attackTowerSets) {
        delete(s.towerobj);
        s.towerobj = nullptr;
    }

    for (auto s : _airTowerSets) {
        delete(s.towerobj);
        s.towerobj = nullptr;
    }

    _towerCoords.clear();
    _locationTypes.clear();
    _airTowerSets.clear();
    _attackTowerSets.clear();
    _airTowerMappingSets.clear();
    _attackTowerMappingSets.clear();
    _baseHealth = 200;


    ls::unload();
    Scene::UnLoad();
}

void Level2Scene::Update(const double& dt) {


    if (_paused) {
        if (_pauseClickTimeout >= 0.0f) _pauseClickTimeout -= dt;


        if (_pauseClickTimeout < 0.0f) {
            //if the player presses P again
            if (Keyboard::isKeyPressed(Keyboard::P)) {
                _paused = false;
                Engine::GetActiveScene()->ents.find("pauseMenu")[0]->setForDelete();
                Engine::GetActiveScene()->ents.find("continue")[0]->setForDelete();
                Engine::GetActiveScene()->ents.find("mainMenuButton")[0]->setForDelete();
                _pauseClickTimeout = 0.5f;
                _clickTimeout = 0.5f;
            }
            //if the player presses continue
            if (Mouse::isButtonPressed(Mouse::Left) && Engine::GetActiveScene()->ents.find("continue")[0]->get_components<ButtonComponent>()[0]->isSelected()) {
                _paused = false;
                Engine::GetActiveScene()->ents.find("pauseMenu")[0]->setForDelete();
                Engine::GetActiveScene()->ents.find("continue")[0]->setForDelete();
                Engine::GetActiveScene()->ents.find("mainMenuButton")[0]->setForDelete();
                _pauseClickTimeout = 0.5f;
                _clickTimeout = 0.5f;
            }
            //if the player presses main menu
            if (Mouse::isButtonPressed(Mouse::Left) && Engine::GetActiveScene()->ents.find("mainMenuButton")[0]->get_components<ButtonComponent>()[0]->isSelected()) {
                _pauseClickTimeout = 0.5f;
                _clickTimeout = 0.5f;
                _goToMainMenu = true;

            }
        }

        //update the delta time for the necessary pause menu buttons
        Engine::GetActiveScene()->ents.find("continue")[0]->get_components<ButtonComponent>()[0]->update(dt);
        Engine::GetActiveScene()->ents.find("mainMenuButton")[0]->get_components<ButtonComponent>()[0]->update(dt);

        //change to main menu scene
        if (_goToMainMenu) {
            Engine::ChangeScene(&menu);
        }

    }



    if (!_paused) {
        Vector2f cursorPos = Engine::GetWindow().mapPixelToCoords(Mouse::getPosition(Engine::GetWindow()));

        //Clicking timeout
        if (_clickTimeout >= 0.0f) _clickTimeout -= dt;

        if (_spawnTimeout >= 0.0f) _spawnTimeout -= dt;


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



        //SPAWN ENEMIES BASED ON WAVE
        if (_spawnTimeout < 0.0f) {
            if (_wave == 1) {
                if (_wave_1_enemiesSpawned < _wave_1_amount) {
                    spawn_enemy_FLYING(_level);
                    _wave_1_enemiesSpawned++;
                    _spawnTimeout = 2.5f;
                }
                if (_wave_1_enemiesSpawned >= _wave_1_amount && Engine::GetActiveScene()->ents.find("enemy").empty()) {
                    _wave++;
                    Engine::GetActiveScene()->ents.find("wave")[0]->get_components<TextComponent>()[0]->SetText("Wave: " + to_string(_wave) + "/3");
                    _spawnTimeout = 10.0f;

                }
            }
            else if (_wave == 2) {
                if (_wave_2_enemiesSpawned < _wave_2_amount) {
                    ((_wave_2_enemiesSpawned == _wave_2_amount - 1) ? spawn_enemy_BOSS(_level) : spawn_enemy(_level));                    
                    _wave_2_enemiesSpawned++;
                    _spawnTimeout = 2.5f;
                }
                if (_wave_2_enemiesSpawned >= _wave_2_amount && Engine::GetActiveScene()->ents.find("enemy").empty()) {
                    _wave++;
                    Engine::GetActiveScene()->ents.find("wave")[0]->get_components<TextComponent>()[0]->SetText("Wave: " + to_string(_wave) + "/3");
                    _spawnTimeout = 10.0f;
                }
            }
            else if (_wave == 3) {
                if (_wave_3_enemiesSpawned < _wave_3_amount) {
                    ((_wave_3_enemiesSpawned == _wave_3_amount - 1) ? spawn_enemy_FLYING_BOSS(_level) : spawn_enemy_FLYING(_level));                   
                    _wave_3_enemiesSpawned++;
                    _spawnTimeout = 2.5f;
                }
                if (_wave_3_enemiesSpawned >= _wave_3_amount && Engine::GetActiveScene()->ents.find("enemy").empty()) {
                    //NEXT SCENE
                    _nextScene = true;

                }
            }
        }


        



        //MOVE VIEW AND UI WHEN USING LOWER RESOLUTIONS
        if (Engine::ResolutionIndex() != 0) {

            View view = Engine::GetWindow().getView(); //get current view

            //DOWN
            if (Keyboard::isKeyPressed(Keyboard::Down)) {
                if ((view.getCenter().y + view.getSize().y / 2) < 1080.0f) {
                    view.setCenter(Vector2f(view.getCenter().x, view.getCenter().y + dt * 300));
                    Engine::GetActiveScene()->ents.find("money")[0]->setPosition(Engine::GetActiveScene()->ents.find("money")[0]->getPosition() + Vector2f(0.0f, dt * 300));
                    Engine::GetActiveScene()->ents.find("wave")[0]->setPosition(Engine::GetActiveScene()->ents.find("wave")[0]->getPosition() + Vector2f(0.0f, dt * 300));
                    Engine::GetActiveScene()->ents.find("level")[0]->setPosition(Engine::GetActiveScene()->ents.find("level")[0]->getPosition() + Vector2f(0.0f, dt * 300));
                    Engine::GetWindow().setView(view);
                    //Match buying interface with view
                    if (!Engine::GetActiveScene()->ents.find("buyInterface").empty()) {
                        Engine::GetActiveScene()->ents.find("buyInterface")[0]->setPosition(Engine::GetActiveScene()->ents.find("buyInterface")[0]->getPosition() + Vector2f(0.0f, dt * 300));
                        Engine::GetActiveScene()->ents.find("purchase_tower_button_ATTACK")[0]->setPosition(Engine::GetActiveScene()->ents.find("purchase_tower_button_ATTACK")[0]->getPosition() + Vector2f(0.0f, dt * 300));
                        Engine::GetActiveScene()->ents.find("purchase_tower_button_AIR")[0]->setPosition(Engine::GetActiveScene()->ents.find("purchase_tower_button_AIR")[0]->getPosition() + Vector2f(0.0f, dt * 300));
                    }
                }
            }
            //UP
            if (Keyboard::isKeyPressed(Keyboard::Up)) {
                if ((view.getCenter().y - view.getSize().y / 2) > 0.0f) {
                    view.setCenter(Vector2f(view.getCenter().x, view.getCenter().y - dt * 300));
                    Engine::GetActiveScene()->ents.find("money")[0]->setPosition(Engine::GetActiveScene()->ents.find("money")[0]->getPosition() + Vector2f(0.0f, -dt * 300));
                    Engine::GetActiveScene()->ents.find("wave")[0]->setPosition(Engine::GetActiveScene()->ents.find("wave")[0]->getPosition() + Vector2f(0.0f, -dt * 300));
                    Engine::GetActiveScene()->ents.find("level")[0]->setPosition(Engine::GetActiveScene()->ents.find("level")[0]->getPosition() + Vector2f(0.0f, -dt * 300));
                    Engine::GetWindow().setView(view);
                    //Match buying interface with view
                    if (!Engine::GetActiveScene()->ents.find("buyInterface").empty()) {
                        Engine::GetActiveScene()->ents.find("buyInterface")[0]->setPosition(Engine::GetActiveScene()->ents.find("buyInterface")[0]->getPosition() + Vector2f(0.0f, -dt * 300));
                        Engine::GetActiveScene()->ents.find("purchase_tower_button_ATTACK")[0]->setPosition(Engine::GetActiveScene()->ents.find("purchase_tower_button_ATTACK")[0]->getPosition() + Vector2f(0.0f, -dt * 300));
                        Engine::GetActiveScene()->ents.find("purchase_tower_button_AIR")[0]->setPosition(Engine::GetActiveScene()->ents.find("purchase_tower_button_AIR")[0]->getPosition() + Vector2f(0.0f, -dt * 300));
                    }
                }
            }
            //RIGHT
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                if ((view.getCenter().x + view.getSize().x / 2) < 1920.0f) {
                    view.setCenter(Vector2f(view.getCenter().x + dt * 300, view.getCenter().y));
                    Engine::GetActiveScene()->ents.find("money")[0]->setPosition(Engine::GetActiveScene()->ents.find("money")[0]->getPosition() + Vector2f(dt * 300, 0.0f));
                    Engine::GetActiveScene()->ents.find("wave")[0]->setPosition(Engine::GetActiveScene()->ents.find("wave")[0]->getPosition() + Vector2f(dt * 300, 0.0f));
                    Engine::GetActiveScene()->ents.find("level")[0]->setPosition(Engine::GetActiveScene()->ents.find("level")[0]->getPosition() + Vector2f(dt * 300, 0.0f));
                    Engine::GetWindow().setView(view);
                    //Match buying interface with view
                    if (!Engine::GetActiveScene()->ents.find("buyInterface").empty()) {
                        Engine::GetActiveScene()->ents.find("buyInterface")[0]->setPosition(Engine::GetActiveScene()->ents.find("buyInterface")[0]->getPosition() + Vector2f(dt * 300, 0.0f));
                        Engine::GetActiveScene()->ents.find("purchase_tower_button_ATTACK")[0]->setPosition(Engine::GetActiveScene()->ents.find("purchase_tower_button_ATTACK")[0]->getPosition() + Vector2f(dt * 300, 0.0f));
                        Engine::GetActiveScene()->ents.find("purchase_tower_button_AIR")[0]->setPosition(Engine::GetActiveScene()->ents.find("purchase_tower_button_AIR")[0]->getPosition() + Vector2f(dt * 300, 0.0f));
                    }
                }
            }
            //LEFT
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                if ((view.getCenter().x - view.getSize().x / 2) > 0.0f) {
                    view.setCenter(Vector2f(view.getCenter().x - dt * 300, view.getCenter().y));
                    Engine::GetActiveScene()->ents.find("money")[0]->setPosition(Engine::GetActiveScene()->ents.find("money")[0]->getPosition() + Vector2f(-dt * 300, 0.0f));
                    Engine::GetActiveScene()->ents.find("wave")[0]->setPosition(Engine::GetActiveScene()->ents.find("wave")[0]->getPosition() + Vector2f(-dt * 300, 0.0f));
                    Engine::GetActiveScene()->ents.find("level")[0]->setPosition(Engine::GetActiveScene()->ents.find("level")[0]->getPosition() + Vector2f(-dt * 300, 0.0f));
                    Engine::GetWindow().setView(view);
                    //Match buying interface with view
                    if (!Engine::GetActiveScene()->ents.find("buyInterface").empty()) {
                        Engine::GetActiveScene()->ents.find("buyInterface")[0]->setPosition(Engine::GetActiveScene()->ents.find("buyInterface")[0]->getPosition() + Vector2f(-dt * 300, 0.0f));
                        Engine::GetActiveScene()->ents.find("purchase_tower_button_ATTACK")[0]->setPosition(Engine::GetActiveScene()->ents.find("purchase_tower_button_ATTACK")[0]->getPosition() + Vector2f(-dt * 300, 0.0f));
                        Engine::GetActiveScene()->ents.find("purchase_tower_button_AIR")[0]->setPosition(Engine::GetActiveScene()->ents.find("purchase_tower_button_AIR")[0]->getPosition() + Vector2f(-dt * 300, 0.0f));
                    }
                }
            }
        }










        //BULLETS COLLIDING WITH THE ENEMIES
        if (!Engine::GetActiveScene()->ents.find("bullet").empty() && !Engine::GetActiveScene()->ents.find("enemy").empty()) {
            for (auto bullet : Engine::GetActiveScene()->ents.find("bullet")) {
                for (auto enemy : Engine::GetActiveScene()->ents.find("enemy")) {
                    if (enemy->get_components<SpriteComponent>()[0]->getSprite().getGlobalBounds().contains(bullet->getPosition())) {

                        if ((enemy->get_components<EnemyAIComponent>()[0]->isAirType() && bullet->get_components<BulletComponent>()[0]->isBulletAir()) || (!enemy->get_components<EnemyAIComponent>()[0]->isAirType() && !bullet->get_components<BulletComponent>()[0]->isBulletAir())) {
                            //decrease health based on the tower damage
                            enemy->get_components<EnemyAIComponent>()[0]->setHealth(enemy->get_components<EnemyAIComponent>()[0]->getHealth() - bullet->get_components<BulletComponent>()[0]->getBulletDamage());

                            //update the health text 
                            enemy->get_components<TextComponent>()[0]->SetText("HP:" + to_string(enemy->get_components<EnemyAIComponent>()[0]->getHealth()));

                            if (enemy->get_components<EnemyAIComponent>()[0]->getHealth() <= 0) {
                                enemy->setForDelete();
                                _soundDeath.play();
                                //when enemy dies, +20 money
                                _money += 20;
                                Engine::GetActiveScene()->ents.find("money")[0]->get_components<TextComponent>()[0]->SetText("$" + to_string(_money));
                            }
                            bullet->setForDelete();
                        }
                    }
                }
            }
        }


        //DECREASE HP OF THE BASE WHEN ENEMIES COLLIDE WITH IT
        if (!Engine::GetActiveScene()->ents.find("enemy").empty()) {
            for (auto enemy : Engine::GetActiveScene()->ents.find("enemy")) {
                if (Engine::GetActiveScene()->ents.find("base")[0]->get_components<SpriteComponent>()[0]->getSprite().getGlobalBounds().contains(enemy->getPosition())) {
                    enemy->get_components<EnemyAIComponent>()[0]->isBossType() ? _baseHealth -= 50 : _baseHealth -= 20;
                    _soundBaseHit.play();
                    enemy->setForDelete();
                    Engine::GetActiveScene()->ents.find("base")[0]->get_components<TextComponent>()[0]->SetText("HP:" + to_string(_baseHealth));
                    if (_baseHealth <= 0) {
                        //GAME OVER
                        _gameOver = true;
                    }
                }
            }
        }




        //ATTACK towers shoot based on their respective attributes
        if (!_attackTowerSets.empty() && !Engine::GetActiveScene()->ents.find("enemy").empty()) {
            for (auto enemy : Engine::GetActiveScene()->ents.find("enemy")) {
                for (auto set : _attackTowerSets) {
                    if (set.towerobj->getFireRateStatus() < 0.0f && set.towerobj->getCanFire() && calculateDistance(enemy->getPosition(), set.entityobj->getPosition()) <= set.towerobj->getRange() && !enemy->get_components<EnemyAIComponent>()[0]->isAirType()) {
                        auto bullet = set.towerobj->create_tower_bullet(set.entityobj.get(), normalize(enemy->getPosition() - set.entityobj->getPosition()));
                        bullet->get_components<BulletComponent>()[0]->setBulletDamage(set.towerobj->getDamage());
                    }
                }
            }
        }

        //AIR towers shoot based on their respective attributes
        if (!_airTowerSets.empty() && !Engine::GetActiveScene()->ents.find("enemy").empty()) {
            for (auto enemy : Engine::GetActiveScene()->ents.find("enemy")) {
                for (auto set : _airTowerSets) {
                    if (set.towerobj->getFireRateStatus() < 0.0f && set.towerobj->getCanFire() && calculateDistance(enemy->getPosition(), set.entityobj->getPosition()) <= set.towerobj->getRange() && enemy->get_components<EnemyAIComponent>()[0]->isAirType()) {
                        auto bulletAir = set.towerobj->create_tower_bullet(set.entityobj.get(), normalize(enemy->getPosition() - set.entityobj->getPosition()));
                        bulletAir->get_components<BulletComponent>()[0]->setBulletDamage(set.towerobj->getDamage());
                    }
                }
            }
        }




        if (_clickTimeout < 0.0f) {

            if (Keyboard::isKeyPressed(Keyboard::P)) {
                _paused = true;
                createPauseMenu();
                _clickTimeout = 0.5;
                _pauseClickTimeout = 0.5;
            }


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
                    new_attack_tower->setRange(400.0f);
                    new_attack_tower->setUpgradeLevel(1);
                    new_attack_tower->setDamage(10.0f);
                    new_attack_tower->setTowerType("attack");
                    //-------------------------------------------------------------------------------------------------------------------------------------

                    //entities
                    auto newtower = new_attack_tower->create_tower();


                    //create and add a new tower set (tower obj + its' entity)
                    attackTowerSets newset;
                    newset.towerobj = new_attack_tower;
                    newset.entityobj = newtower;
                    _attackTowerSets.push_back(newset);


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
                    new_air_tower->setDamage(10.0f);
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
                if (Engine::GetActiveScene()->ents.find("upgradeButton")[0]->get_components<ButtonComponent>()[0]->isSelected() && _money >= 15 && _upgradeInterfaceOpen && Mouse::isButtonPressed(Mouse::Left)) {
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
                                    s.sets.towerobj->setDamage(s.sets.towerobj->getDamage() + 10.0f);

                                    //visual upgrade
                                    s.sets.towerobj->visualUpgrade(s.sets.entityobj);

                                    //--------------------------SIMULATE THE REFRESHING OF THE VALUES BY RELOADING THE INTERFACE------------------------

                                    for (auto e : Engine::GetActiveScene()->ents.find("upgradeInterface")) {
                                        e->setForDelete();
                                    }
                                    for (auto e : Engine::GetActiveScene()->ents.find("upgradeButton")) {
                                        e->setForDelete();
                                    }
                                    for (auto e : Engine::GetActiveScene()->ents.find("rangeCircle")) {
                                        e->setForDelete();
                                    }

                                    cout << "deleted previous interface" << endl;

                                    create_upgradeInterface_ATTACKTOWER(s.sets.towerobj, s.sets.entityobj->getPosition());
                                    //------------------------------------------------------------------------------------------------------------------

                                    cout << "tower upgraded!" << endl;
                                    _money -= 15;
                                    _soundPurchase.play();
                                    Engine::GetActiveScene()->ents.find("money")[0]->get_components<TextComponent>()[0]->SetText("$" + to_string(_money));
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
                                    s.sets.towerobj->setDamage(s.sets.towerobj->getDamage() + 10.0f);

                                    //visual upgrade
                                    s.sets.towerobj->visualUpgrade(s.sets.entityobj);

                                    //--------------------------SIMULATE THE REFRESHING OF THE VALUES BY RELOADING THE INTERFACE------------------------

                                    for (auto e : Engine::GetActiveScene()->ents.find("upgradeInterface")) {
                                        e->setForDelete();
                                    }
                                    for (auto e : Engine::GetActiveScene()->ents.find("upgradeButton")) {
                                        e->setForDelete();
                                    }
                                    for (auto e : Engine::GetActiveScene()->ents.find("rangeCircle")) {
                                        e->setForDelete();
                                    }

                                    cout << "deleted previous interface" << endl;

                                    create_upgradeInterface_AIRTOWER(s.sets.towerobj, s.sets.entityobj->getPosition());
                                    //------------------------------------------------------------------------------------------------------------------

                                    cout << "tower upgraded!" << endl;
                                    _money -= 15;
                                    _soundPurchase.play();
                                    Engine::GetActiveScene()->ents.find("money")[0]->get_components<TextComponent>()[0]->SetText("$" + to_string(_money));
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
                                            for (auto e : Engine::GetActiveScene()->ents.find("rangeCircle")) {
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
                                            for (auto e : Engine::GetActiveScene()->ents.find("rangeCircle")) {
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
                for (auto e : Engine::GetActiveScene()->ents.find("rangeCircle")) {
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
                            _money -= 20;
                            Engine::GetActiveScene()->ents.find("money")[0]->get_components<TextComponent>()[0]->SetText("$" + to_string(_money));

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
                            _money -= 25;
                            Engine::GetActiveScene()->ents.find("money")[0]->get_components<TextComponent>()[0]->SetText("$" + to_string(_money));

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
                        _soundPurchase.play();
                        _clickTimeout = 0.5f;
                    }
                }
                else if (Mouse::isButtonPressed(Mouse::Left) && LevelSystem::getTileAt(cursorPos) != LevelSystem::TOWERSPOTS) {
                    cout << "Can't build a tower here!" << endl;
                    _clickTimeout = 0.5f;
                }
            }
        }


        Scene::Update(dt);

        if (_nextScene) {
            Engine::ChangeScene((Scene*)&level3);
        }
        else if (_gameOver) {
            Engine::ChangeScene((Scene*)&game_over_scene);
        }
    }


}

void Level2Scene::Render() {
    ls::render(Engine::GetWindow());
    Scene::Render();
}
