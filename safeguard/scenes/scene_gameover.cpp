#include "scene_gameover.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void GameOverScene::Load() {

    //load the custom font
    
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


    //BACKGROUND IMAGE
    {
        auto background = Engine::GetActiveScene()->makeEntity();
        auto sprite1 = background->addComponent<SpriteComponent>();
        sprite1->getSprite().setTexture(_background, true);
    }



    //LOGO TITLE
    {
        auto logo = Engine::GetActiveScene()->makeEntity();

        auto shapeLogo = logo->addComponent<ShapeComponent>();
        shapeLogo->setShape<RectangleShape>(Vector2f(600, 100));
        shapeLogo->getShape().setOrigin(shapeLogo->getShape().getLocalBounds().width / 2, shapeLogo->getShape().getLocalBounds().height / 2);
        shapeLogo->getShape().setFillColor(Color::Transparent);

        auto t = logo->addComponent<TextComponent>("GAME OVER!");
        t->getText()->setOrigin(t->getText()->getLocalBounds().width / 2 + 135.f, t->getText()->getLocalBounds().height / 2 + 100.0f);
        t->getText()->setCharacterSize(150.0f);
        t->getText()->setStyle(sf::Text::Bold);
        t->getText()->setColor(Color::Color(219, 201, 158, 200));
        t->getText()->setFont(_font2);

        logo->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 - 20.0f, Engine::GetWindow().getSize().y / 2 - 300.0f));

    }

    //--------------------------------------------BUTTONS-----------------------------------------

    //RESTART
    {
        auto start = Engine::GetActiveScene()->makeEntity();
        start->addTag("GameOverRestart");

        auto shape = start->addComponent<ShapeComponent>();
        shape->setShape<RectangleShape>(Vector2f(270.0f, 100.0f));
        shape->getShape().setOrigin(shape->getShape().getLocalBounds().width / 2, shape->getShape().getLocalBounds().height / 2);

        auto t1 = start->addComponent<TextComponent>("RESTART");
        t1->getText()->setOrigin(t1->getText()->getLocalBounds().width / 2 + 35.0f, t1->getText()->getLocalBounds().height / 2 + 60.0f);
        t1->getText()->setCharacterSize(100.0f);
        t1->getText()->setColor(Color::Color(196, 144, 120, 255));
        t1->getText()->setFont(_font2);

        auto buttonStart = start->addComponent<ButtonComponent>(shape, t1);
        buttonStart->setButtonType("menu");
        shape->getShape().setFillColor(Color::Color(219, 201, 158, 100));
        start->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 - 20.0f, Engine::GetWindow().getSize().y / 2 - 150.0f));
    }


    //EXIT TO MAIN MENU
    {
        auto exit = Engine::GetActiveScene()->makeEntity();
        exit->addTag("GameOverMainMenu");

        auto shape2 = exit->addComponent<ShapeComponent>();
        shape2->setShape<RectangleShape>(Vector2f(320.0f, 100.0f));
        shape2->getShape().setOrigin(shape2->getShape().getLocalBounds().width / 2, shape2->getShape().getLocalBounds().height / 2);

        auto t2 = exit->addComponent<TextComponent>("MAIN MENU");
        t2->getText()->setOrigin(t2->getText()->getLocalBounds().width / 2 + 60.0f, t2->getText()->getLocalBounds().height / 2 + 60.0f);
        t2->getText()->setCharacterSize(100.0f);
        t2->getText()->setColor(Color::Color(196, 144, 120, 255));
        t2->getText()->setFont(_font2);

        auto buttonExit = exit->addComponent<ButtonComponent>(shape2, t2);
        buttonExit->setButtonType("menu");
        shape2->getShape().setFillColor(Color::Color(219, 201, 158, 100));
        exit->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 - 20.0f, Engine::GetWindow().getSize().y / 2 - 20.0f));
    }  

    _selectionSoundPlayedRestart = false;
    _selectionSoundPlayedExitMenu = false;
    _restartGame = false;
    _quitGameMenu = false;
    setLoaded(true);
}

void GameOverScene::Update(const double& dt) {
    if (_clickMenuTimeout >= 0.0f) _clickMenuTimeout -= dt;

    //RESTART 
    if (Engine::GetActiveScene()->ents.find("GameOverRestart")[0]->get_components<ButtonComponent>()[0]->isHighlighted() && !_selectionSoundPlayedRestart) {
        _selectSound.play();
        _selectionSoundPlayedRestart = true;
        Engine::GetActiveScene()->ents.find("GameOverRestart")[0]->get_components<ShapeComponent>()[0]->getShape().setScale(sf::Vector2f(1.2, 1.2));

    }
    else if (!Engine::GetActiveScene()->ents.find("GameOverRestart")[0]->get_components<ButtonComponent>()[0]->isHighlighted() && _selectionSoundPlayedRestart) {
        _selectionSoundPlayedRestart = false;
        Engine::GetActiveScene()->ents.find("GameOverRestart")[0]->get_components<ShapeComponent>()[0]->getShape().setScale(sf::Vector2f(1, 1));
    }

    //MAIN MENU
    if (Engine::GetActiveScene()->ents.find("GameOverMainMenu")[0]->get_components<ButtonComponent>()[0]->isHighlighted() && !_selectionSoundPlayedExitMenu) {
        _selectSound.play();
        _selectionSoundPlayedExitMenu = true;
        Engine::GetActiveScene()->ents.find("GameOverMainMenu")[0]->get_components<ShapeComponent>()[0]->getShape().setScale(sf::Vector2f(1.2, 1.2));

    }
    else if (!Engine::GetActiveScene()->ents.find("GameOverMainMenu")[0]->get_components<ButtonComponent>()[0]->isHighlighted() && _selectionSoundPlayedExitMenu) {
        _selectionSoundPlayedExitMenu = false;
        Engine::GetActiveScene()->ents.find("GameOverMainMenu")[0]->get_components<ShapeComponent>()[0]->getShape().setScale(sf::Vector2f(1, 1));
    }


    if (_clickMenuTimeout < 0.0f) {
        //RESTART
        if (Engine::GetActiveScene()->ents.find("GameOverRestart")[0]->get_components<ButtonComponent>()[0]->isSelected() && Mouse::isButtonPressed(Mouse::Left)) {
            _selectSound.play();
            _restartGame = true;
            _clickMenuTimeout = 0.5f;
        }
        //EXIT MENU
        if (Engine::GetActiveScene()->ents.find("GameOverMainMenu")[0]->get_components<ButtonComponent>()[0]->isSelected() && Mouse::isButtonPressed(Mouse::Left)) {
            _selectSound.play();
            _quitGameMenu = true;
            _clickMenuTimeout = 0.5f;
        }
    }





    Scene::Update(dt);

    if (_restartGame) {
        Engine::ChangeScene(&level1);
    }
    else if (_quitGameMenu) {
        Engine::ChangeScene(&menu);
    }
}
