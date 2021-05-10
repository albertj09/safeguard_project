#include "engine.h"
#include "game.h"
//#include "scenes/scene_menu.h"

using namespace std;

MenuScene menu;
OptionsScene menu_options;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;
GameOverScene game_over_scene;
VictoryScene victory_scene;

int main() {
  Engine::Start(1920, 1080, "Safeguard", &menu);
}