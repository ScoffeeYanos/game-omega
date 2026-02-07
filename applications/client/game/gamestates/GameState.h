#pragma once
#include <memory>

#include "core/game.h"
#include "core/rendering/renderer.h"
#include "game/game.h"
#include "game/world/world.h"
using Game = GameSingleton<MyGame, GameStates>;
class GameState : public Game::GameStateBase {
public:
    static constexpr GameStates type = GameStates::GAME;

    void enter(MyGame& game, GameStates old_state_type) override;
    void loop(MyGame& game, float dt) override;
    void exit(MyGame& game, GameStates new_state_type) override {};

    void keyboard(MyGame& game, int key, int scancode, int action, int mods) override;

private:
    void update(float dt);
    void render(Renderer& renderer);
    void draw_debug_menu(MyGame& game);
    void draw_pause_menu();

    std::unique_ptr<World> world;
};