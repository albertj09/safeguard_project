#pragma once

#include "engine.h"
#include <SFML/Window/Keyboard.hpp>

class MenuScene : public Scene {
public:
  MenuScene() = default;
  ~MenuScene() override = default;

  void Load() override;

  void Update(const double& dt) override;
};
