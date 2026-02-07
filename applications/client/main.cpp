#include "game/gamestates/GameState.h"

int main() {

    using Game = GameSingleton<MyGame, GameStates>;

    cppgl::ContextParameters params;
    params.title = "AAA";
    //params.decorated = false;
    params.font_ttf_filename = EXECUTABLE_DIR + std::string{"/render-data/fonts/DroidSansMono.ttf"};
    params.font_size_pixels = 15;
    Game::init(params);
    Game::run();
}