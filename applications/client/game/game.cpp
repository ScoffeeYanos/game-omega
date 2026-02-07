#include "game.h"

#include <fstream>

#include "core/debug/debug-draw.h"
#include "core/rendering/util.h"

#include "ui/ui.h"

using namespace cppgl;

void on_resize(int width, int height) { GameSingleton<MyGame, GameStates>::game().renderer.on_resize({width, height}); }

MyGame::MyGame() {
    Context::set_resize_callback(on_resize);

    sound_manager.create_bus("music");
    sound_manager.create_bus("fx");
    sound_manager.create_bus("ui");

    deserialize_settings();

    music_ = sound_manager.load_sound<Stream>("data/sound-data/music/music.mp3", "music");
    music_.sound().setLooping(true);
    music_.play();

    GameUi::Init(sound_manager);
    DebugDraw::init();
}

MyGame::~MyGame() { serialize_settings(); }



void MyGame::serialize_settings() {
    std::ofstream settings_file{"settings.ini"};

    settings_file << "music volume=" << sound_manager.get_volume("music") << "\n";
    settings_file << "ui volume=" << sound_manager.get_volume("ui") << "\n";
    settings_file << "fx volume=" << sound_manager.get_volume("fx") << "\n";
}

void MyGame::deserialize_settings() {
    std::ifstream settings_file{"settings.ini"};
    if (!settings_file)
        return;

    std::string line;
    while (std::getline(settings_file, line)) {
        std::string key;
        float value;

        std::istringstream iss(line);
        if (!std::getline(iss, key, '='))
            continue;

        key.erase(key.find_last_not_of(" \t") + 1);

        if (!(iss >> value))
            continue;

        value = std::clamp(value, 0.0f, 1.0f);

        if (key == "music volume") {
            sound_manager.set_volume("music", value);
        } else if (key == "ui volume") {
            sound_manager.set_volume("ui", value);
        } else if (key == "fx volume") {
            sound_manager.set_volume("fx", value);
        }
    }
}
