#pragma once

#include "engine.h"
#include <SFML/Window/Keyboard.hpp>
#include "../components/cmp_button.h"
#include <SFML/Audio.hpp>

class VictoryScene : public Scene {
public:
	VictoryScene() = default;
	~VictoryScene() override = default;

	void Load() override;

	void Update(const double& dt) override;

private:

	//Sounds
	sf::SoundBuffer _bufferSelectSound;
	sf::Sound _selectSound;

	//Whether the selection sound has been played
	bool _selectionSoundPlayedRestart;
	bool _selectionSoundPlayedExitMenu;



	sf::Font _font2;
	sf::Texture _background;
	float _clickMenuTimeout;
	bool _restartGame;
	bool _quitGameMenu;

};