#include "scene_options.h"
#include "../game.h"
#include "../components/cmp_text.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "../gamestuff.h"

using namespace std;
using namespace sf;

void OptionsScene::Load() {
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

    //need to get this before creating the resolution text label
    _resolutionIndex = Engine::ResolutionIndex();

    //TITLE
    {
        auto title = Engine::GetActiveScene()->makeEntity();

        auto shapeTitle = title->addComponent<ShapeComponent>();
        shapeTitle->setShape<RectangleShape>(Vector2f(600, 100));
        shapeTitle->getShape().setOrigin(shapeTitle->getShape().getLocalBounds().width / 2, shapeTitle->getShape().getLocalBounds().height / 2);
        shapeTitle->getShape().setFillColor(Color::Transparent);

        auto t = title->addComponent<TextComponent>("OPTIONS");
        t->getText()->setOrigin(t->getText()->getLocalBounds().width / 2 + 135.0f, t->getText()->getLocalBounds().height / 2 + 100.0f);
        t->getText()->setCharacterSize(150.0f); 
        t->getText()->setStyle(sf::Text::Bold);
        t->getText()->setColor(Color::Color(219, 201, 158, 200));
        t->getText()->setFont(_font2);

        title->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 - 20.0f, Engine::GetWindow().getSize().y / 2 - 300.0f));

    }

    //BACK BUTTON
    {
        auto back = Engine::GetActiveScene()->makeEntity();
        back->addTag("backButton");

        auto shape = back->addComponent<ShapeComponent>();
        shape->setShape<RectangleShape>(Vector2f(270.0f, 100.0f));
        shape->getShape().setOrigin(shape->getShape().getLocalBounds().width / 2, shape->getShape().getLocalBounds().height / 2);
        shape->getShape().setScale(Vector2f(0.7, 0.7));

        auto t1 = back->addComponent<TextComponent>("BACK");
        t1->getText()->setOrigin(t1->getText()->getLocalBounds().width / 2 + 35.0f, t1->getText()->getLocalBounds().height / 2 + 65.0f);
        t1->getText()->setCharacterSize(100.0f);
        t1->getText()->setColor(Color::Color(196, 144, 120, 255));
        t1->getText()->setFont(_font2);

        auto buttonBack = back->addComponent<ButtonComponent>(shape, t1);
        buttonBack->setButtonType("menu");
        shape->getShape().setFillColor(Color::Color(219, 201, 158, 100));
        back->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 - 500.0f, Engine::GetWindow().getSize().y / 2 - 280.0f));
    }

    //VSYNC ON/OFF TEXT
    {
        auto vsyncText = Engine::GetActiveScene()->makeEntity();
        vsyncText->addTag("vsyncText");

        auto shapeVsync = vsyncText->addComponent<ShapeComponent>();
        shapeVsync->setShape<RectangleShape>(Vector2f(100, 100));
        shapeVsync->getShape().setOrigin(shapeVsync->getShape().getLocalBounds().width / 2, shapeVsync->getShape().getLocalBounds().height / 2);
        shapeVsync->getShape().setFillColor(Color::Transparent);

        auto vt = vsyncText->addComponent<TextComponent>(vsyncInterpreter(Engine::getVsyncStatus()));
        vt->getText()->setOrigin(vt->getText()->getLocalBounds().width / 2 + 20.0f, vt->getText()->getLocalBounds().height / 2 + 60.0f);
        vt->getText()->setCharacterSize(100.0f);
        vt->getText()->setStyle(sf::Text::Bold);
        vt->getText()->setColor(Engine::getVsyncStatus() ? Color::Green : Color::Red);
        vt->getText()->setFont(_font2);

        vsyncText->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 + 150.0f, Engine::GetWindow().getSize().y / 2 - 150.0f));

    }

    //VSYNC BUTTON TOGGLE
    {
        auto vsyncButton = Engine::GetActiveScene()->makeEntity();
        vsyncButton->addTag("vsyncButton");

        auto shape2 = vsyncButton->addComponent<ShapeComponent>();
        shape2->setShape<RectangleShape>(Vector2f(270.0f, 100.0f));
        shape2->getShape().setOrigin(shape2->getShape().getLocalBounds().width / 2, shape2->getShape().getLocalBounds().height / 2);

        auto t2 = vsyncButton->addComponent<TextComponent>("V-SYNC");
        t2->getText()->setOrigin(t2->getText()->getLocalBounds().width / 2 + 45.0f, t2->getText()->getLocalBounds().height / 2 + 60.0f);
        t2->getText()->setCharacterSize(100.0f);
        t2->getText()->setColor(Color::Color(196, 144, 120, 255));
        t2->getText()->setFont(_font2);

        auto buttonBack = vsyncButton->addComponent<ButtonComponent>(shape2, t2);
        buttonBack->setButtonType("menu");
        shape2->getShape().setFillColor(Color::Color(219, 201, 158, 100));
        vsyncButton->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 - 80.0f, Engine::GetWindow().getSize().y / 2 - 150.0f));
    }

    //RESOLUTION TEXT
    {
        auto resText = Engine::GetActiveScene()->makeEntity();
        resText->addTag("resText");

        auto shapeRes = resText->addComponent<ShapeComponent>();
        shapeRes->setShape<RectangleShape>(Vector2f(100, 100));
        shapeRes->getShape().setOrigin(shapeRes->getShape().getLocalBounds().width / 2, shapeRes->getShape().getLocalBounds().height / 2);
        shapeRes->getShape().setFillColor(Color::Transparent);

        auto rt = resText->addComponent<TextComponent>(resolutionInterpreter(_resolutionIndex));
        rt->getText()->setOrigin(rt->getText()->getLocalBounds().width / 2, rt->getText()->getLocalBounds().height / 2);
        rt->getText()->setCharacterSize(40.0f);
        rt->getText()->setStyle(sf::Text::Bold);
        rt->getText()->setColor(Color::Color(196, 144, 120, 255));

        resText->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 + 180.0f, Engine::GetWindow().getSize().y / 2 - 30.0f));
       
    }

    //RESOLUTION BUTTON
    {
        auto resButton = Engine::GetActiveScene()->makeEntity();
        resButton->addTag("resButton");

        auto shape3 = resButton->addComponent<ShapeComponent>();
        shape3->setShape<RectangleShape>(Vector2f(270.0f, 100.0f));
        shape3->getShape().setOrigin(shape3->getShape().getLocalBounds().width / 2, shape3->getShape().getLocalBounds().height / 2);

        auto t3 = resButton->addComponent<TextComponent>("CHANGE");
        t3->getText()->setOrigin(t3->getText()->getLocalBounds().width / 2 + 45.0f, t3->getText()->getLocalBounds().height / 2 + 60.0f);
        t3->getText()->setCharacterSize(100.0f);
        t3->getText()->setColor(Color::Color(196, 144, 120, 255));
        t3->getText()->setFont(_font2);

        auto buttonBack = resButton->addComponent<ButtonComponent>(shape3, t3);
        buttonBack->setButtonType("menu");
        shape3->getShape().setFillColor(Color::Color(219, 201, 158, 100));
        resButton->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 - 80.0f, Engine::GetWindow().getSize().y / 2 - 20.0f));
    }

    _clickTimeout = 0.5f;
    _goBack = false;
    _selectionSoundPlayedBack = false;
    _selectionSoundPlayedVsync = false;
    _selectionSoundPlayedResolution = false;
    _vsync = Engine::getVsyncStatus();
    

}

void OptionsScene::Update(const double& dt)
{
    if (_clickTimeout >= 0.0f) _clickTimeout -= dt;

    //BACK
    if (Engine::GetActiveScene()->ents.find("backButton")[0]->get_components<ButtonComponent>()[0]->isHighlighted() && !_selectionSoundPlayedBack) {
        _selectSound.play();
        _selectionSoundPlayedBack = true;
        Engine::GetActiveScene()->ents.find("backButton")[0]->get_components<ShapeComponent>()[0]->getShape().setScale(sf::Vector2f(1, 1));

    }
    else if (!Engine::GetActiveScene()->ents.find("backButton")[0]->get_components<ButtonComponent>()[0]->isHighlighted() && _selectionSoundPlayedBack) {
        _selectionSoundPlayedBack = false;
        Engine::GetActiveScene()->ents.find("backButton")[0]->get_components<ShapeComponent>()[0]->getShape().setScale(sf::Vector2f(0.7, 0.7));
    }

    //VSYNC
    if (Engine::GetActiveScene()->ents.find("vsyncButton")[0]->get_components<ButtonComponent>()[0]->isHighlighted() && !_selectionSoundPlayedVsync) {
        _selectSound.play();
        _selectionSoundPlayedVsync = true;
        Engine::GetActiveScene()->ents.find("vsyncButton")[0]->get_components<ShapeComponent>()[0]->getShape().setScale(sf::Vector2f(1.2, 1.2));

    }
    else if (!Engine::GetActiveScene()->ents.find("vsyncButton")[0]->get_components<ButtonComponent>()[0]->isHighlighted() && _selectionSoundPlayedVsync) {
        _selectionSoundPlayedVsync = false;
        Engine::GetActiveScene()->ents.find("vsyncButton")[0]->get_components<ShapeComponent>()[0]->getShape().setScale(sf::Vector2f(1, 1));
    }

    //RESOLUTION
    if (Engine::GetActiveScene()->ents.find("resButton")[0]->get_components<ButtonComponent>()[0]->isHighlighted() && !_selectionSoundPlayedResolution) {
        _selectSound.play();
        _selectionSoundPlayedResolution = true;
        Engine::GetActiveScene()->ents.find("resButton")[0]->get_components<ShapeComponent>()[0]->getShape().setScale(sf::Vector2f(1.2, 1.2));

    }
    else if (!Engine::GetActiveScene()->ents.find("resButton")[0]->get_components<ButtonComponent>()[0]->isHighlighted() && _selectionSoundPlayedResolution) {
        _selectionSoundPlayedResolution = false;
        Engine::GetActiveScene()->ents.find("resButton")[0]->get_components<ShapeComponent>()[0]->getShape().setScale(sf::Vector2f(1, 1));
    }
    


    if (_clickTimeout < 0.0f) {
        //BACK
        if (Engine::GetActiveScene()->ents.find("backButton")[0]->get_components<ButtonComponent>()[0]->isSelected() && Mouse::isButtonPressed(Mouse::Left)) {           
            _selectSound.play();
            _goBack = true;
            _clickTimeout = 0.5f;
        }

        //VSYNC
        if (Engine::GetActiveScene()->ents.find("vsyncButton")[0]->get_components<ButtonComponent>()[0]->isSelected() && Mouse::isButtonPressed(Mouse::Left)) {
            _selectSound.play();
           
            //Toggle vsync in the engine
            _vsync = !_vsync;
            Engine::setVsync(_vsync);
            Engine::GetActiveScene()->ents.find("vsyncText")[0]->get_components<TextComponent>()[0]->SetText(vsyncInterpreter(_vsync));

            //Set the respective color. On = green, Off = red
            if (_vsync) {
                Engine::GetActiveScene()->ents.find("vsyncText")[0]->get_components<TextComponent>()[0]->getText()->setColor(Color::Green);
            }
            else {
                Engine::GetActiveScene()->ents.find("vsyncText")[0]->get_components<TextComponent>()[0]->getText()->setColor(Color::Red);
            }          

            _clickTimeout = 0.5f;
        }


        //RESOLUTION
        if(Engine::GetActiveScene()->ents.find("resButton")[0]->get_components<ButtonComponent>()[0]->isSelected() && Mouse::isButtonPressed(Mouse::Left))
        {
            _selectSound.play();
           

            _resolutionIndex++;
            if (_resolutionIndex > 3) {
                _resolutionIndex = 0;
            }

            Engine::GetActiveScene()->ents.find("resText")[0]->get_components<TextComponent>()[0]->SetText(resolutionInterpreter(_resolutionIndex));          
            Engine::ChangeResolution(_resolutionIndex);
            
            _clickTimeout = 0.5f;
                       
        }

    }

    Scene::Update(dt);

    if (_goBack) {
        Engine::ChangeScene(&menu);
    }
}

