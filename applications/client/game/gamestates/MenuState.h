#pragma once
#include "core/game.h"
#include "game/game.h"

using Game = GameSingleton<MyGame, GameStates>;

class MenuState : public Game::GameStateBase {
public:
    static constexpr GameStates type = GameStates::MENU;

    void enter(MyGame& game, GameStates old_state_type) override;
    void loop(MyGame& game, float dt) override;
    void exit(MyGame& game, GameStates new_state_type) override {};

    void keyboard(MyGame& game, int key, int scancode, int action, int mods) override;

private:
    void draw_main_menu();
};