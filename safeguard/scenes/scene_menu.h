#pragma once

#include "engine.h"
#include <SFML/Window/Keyboard.hpp>
#include "../components/cmp_button.h"
#include <SFML/Audio.hpp>

class MenuScene : public Scene {
public:
  MenuScene() = default;
  ~MenuScene() override = default;

  void Load() override;

  void Update(const double& dt) override;

private:

	//Sounds
	sf::SoundBuffer _bufferSelectSound;
	sf::Sound _selectSound;
	//
	sf::SoundBuffer _bufferClickSound;
	sf::Sound _clickSound;

	//Whether the selection sound has been played
	bool _selectionSoundPlayedStart;
	bool _selectionSoundPlayedExit;


	sf::Font _font1;
	sf::Font _font2;
	sf::Texture _background;
	float _clickMenuTimeout;
	bool _startGame;
	float _initialTimeout;
	
};
