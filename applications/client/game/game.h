#pragma once

#include "core/game.h"
#include "core/rendering/renderer.h"
#include "core/sound-manager.h"
#include "core/video.h"

enum class GameStates : uint8_t { MENU, GAME };

class GameState;

class MyGame {
public:
    using StartState = GameState;

    MyGame();
    ~MyGame();

    Renderer renderer;
    SoundManager sound_manager;

private:
    void deserialize_settings();
    void serialize_settings();

    Stream music_;
};