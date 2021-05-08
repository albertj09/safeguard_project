#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void MenuScene::Load() {
  cout << "Menu Load \n";

  //load the custom font
  if (!_font1.loadFromFile("res/fonts/blackjack.otf")) {
      cout << "font1 not loaded" << endl;
  };
  if (!_font2.loadFromFile("res/fonts/TropicalAsianDemoRegular.ttf")) {
      cout << "font2 not loaded" << endl;
  }

  //load background image
  if (!_background.loadFromFile("res/background.jpg")) {
      cout << "background not loaded" << endl;
  }

  //load all the sounds
  _bufferSelectSound.loadFromFile("res/menu_selected.wav");
  _selectSound.setBuffer(_bufferSelectSound);
  _selectSound.setVolume(10.0f);
  
  //
  _bufferClickSound.loadFromFile("res/menu_confirm.wav");
  _clickSound.setBuffer(_bufferClickSound);
  _clickSound.setVolume(10.0f);


  //BACKGROUND IMAGE
  {
    auto background = Engine::GetActiveScene()->makeEntity();
    auto sprite1 = background->addComponent<SpriteComponent>();
    sprite1->getSprite().setTexture(_background, true);
  }
  


  //LOGO TITLE
  {
      auto logo = Engine::GetActiveScene()->makeEntity();
      //logo->addTag("menustart");

      auto t = logo->addComponent<TextComponent>("Safeguard");
      t->getText()->setOrigin(t->getText()->getLocalBounds().width / 2 - 800.0f, t->getText()->getLocalBounds().height / 2 - 200.0f);
      t->getText()->setCharacterSize(100.0f);
      t->getText()->setStyle(sf::Text::Bold);
      t->getText()->setFont(_font1);

  }

  //--------------------------------------------BUTTONS-----------------------------------------

  //START
  {
      auto start = Engine::GetActiveScene()->makeEntity();
      start->addTag("MenuStartButton");

      auto shape = start->addComponent<ShapeComponent>();
      shape->setShape<RectangleShape>(Vector2f(270.0f, 100.0f));
      shape->getShape().setOrigin(shape->getShape().getLocalBounds().width / 2, shape->getShape().getLocalBounds().height / 2);

      auto t1 = start->addComponent<TextComponent>("START");
      t1->getText()->setOrigin(t1->getText()->getLocalBounds().width / 2 + 30.0f, t1->getText()->getLocalBounds().height / 2 + 60.0f);
      t1->getText()->setCharacterSize(100.0f);
      t1->getText()->setColor(Color::Blue);
      t1->getText()->setFont(_font2);
      
      auto buttonStart = start->addComponent<ButtonComponent>(shape, t1);
      buttonStart->setButtonType("menu");
      shape->getShape().setFillColor(Color::Color(219, 201, 158, 100));
      start->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 - 20.0f, Engine::GetWindow().getSize().y / 2 - 150.0f));
  }

  //EXIT
  {
      auto exit = Engine::GetActiveScene()->makeEntity();
      exit->addTag("MenuExitButton");

      auto shape2 = exit->addComponent<ShapeComponent>();
      shape2->setShape<RectangleShape>(Vector2f(270.0f, 100.0f));
      shape2->getShape().setOrigin(shape2->getShape().getLocalBounds().width / 2, shape2->getShape().getLocalBounds().height / 2);

      auto t2 = exit->addComponent<TextComponent>("QUIT");
      t2->getText()->setOrigin(t2->getText()->getLocalBounds().width / 2 + 30.0f, t2->getText()->getLocalBounds().height / 2 + 60.0f);
      t2->getText()->setCharacterSize(100.0f);
      t2->getText()->setColor(Color::Blue);
      t2->getText()->setFont(_font2);

      auto buttonExit = exit->addComponent<ButtonComponent>(shape2, t2);
      buttonExit->setButtonType("menu");
      shape2->getShape().setFillColor(Color::Color(219, 201, 158, 100));
      exit->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 - 20.0f, Engine::GetWindow().getSize().y / 2 - 20.0f));
  }

  _initialTimeout = 0.5f;

  _selectionSoundPlayedStart = false;
  _selectionSoundPlayedExit = false;
  _startGame = false;
  _clickMenuTimeout = 0.5f;
  setLoaded(true);
}

void MenuScene::Update(const double& dt) {
  // cout << "Menu Update "<<dt<<"\n";
  if (_clickMenuTimeout >= 0.0f) _clickMenuTimeout -= dt;

  if (_initialTimeout >= 0.0f) _initialTimeout -= dt;

  //START
  if (Engine::GetActiveScene()->ents.find("MenuStartButton")[0]->get_components<ButtonComponent>()[0]->isHighlighted() && !_selectionSoundPlayedStart) {     
      _selectSound.play();
      _selectionSoundPlayedStart = true;
      
  }
  else if (!Engine::GetActiveScene()->ents.find("MenuStartButton")[0]->get_components<ButtonComponent>()[0]->isHighlighted() && _selectionSoundPlayedStart) {
      _selectionSoundPlayedStart = false;
  }

  //EXIT
  if (Engine::GetActiveScene()->ents.find("MenuExitButton")[0]->get_components<ButtonComponent>()[0]->isHighlighted() && !_selectionSoundPlayedExit) {
      _selectSound.play();
      _selectionSoundPlayedExit = true;
     
  }
  else if (!Engine::GetActiveScene()->ents.find("MenuExitButton")[0]->get_components<ButtonComponent>()[0]->isHighlighted() && _selectionSoundPlayedExit) {
      _selectionSoundPlayedExit = false;
  }


  if (_clickMenuTimeout < 0.0f) {
       //START
       if (Engine::GetActiveScene()->ents.find("MenuStartButton")[0]->get_components<ButtonComponent>()[0]->isSelected() && Mouse::isButtonPressed(Mouse::Left)) {          
           _clickSound.play();
           _startGame = true;
           _clickMenuTimeout = 0.5f;                                    
       }
       //EXIT (should be the last condition in the function)
       if (Engine::GetActiveScene()->ents.find("MenuExitButton")[0]->get_components<ButtonComponent>()[0]->isSelected() && Mouse::isButtonPressed(Mouse::Left)) {
           _clickSound.play();
           _clickMenuTimeout = 0.5f;
           cout << "Exit pressed, quitting game" << endl;
           Engine::GetWindow().close();
       }
  }


  
  

  Scene::Update(dt);

  if (_startGame) {
      Engine::ChangeScene(&level1);
      cout << "Start pressed, switching to level 1" << endl;
  }
}
