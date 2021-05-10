#pragma once

#include "engine.h"
#include <SFML/Window/Keyboard.hpp>
#include "../components/cmp_button.h"
#include <SFML/Audio.hpp>


class OptionsScene : public Scene {
public:
	OptionsScene() = default;
	~OptionsScene() override = default;

	void Load() override;

	void Update(const double& dt) override;

private:

	//Sounds
	sf::SoundBuffer _bufferSelectSound;
	sf::Sound _selectSound;

	//aesthetics
	sf::Font _font1;
	sf::Font _font2;
	sf::Texture _background;


	float _clickTimeout;
	int _resolutionIndex;
	bool _selectionSoundPlayedBack;
	bool _selectionSoundPlayedVsync;
	bool _selectionSoundPlayedResolution;
	bool _vsync;
	

	bool _goBack;

};