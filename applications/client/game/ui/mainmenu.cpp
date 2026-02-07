#include "mainmenu.h"
#include "ui.h"
#include "core/ui.h"
bool draw_info_menu() {
    bool should_close{false};
    GameUi::BeginMenu(0, 2);
    if (GameUi::MenuButton("Do Stuff")) {
    }
    GameUi::Spacing();
    should_close = GameUi::MenuButton("Back");
    GameUi::EndMenu();

    return should_close;
}
bool draw_settings_menu(SoundManager sound_manager) {
    bool should_close{false};
    GameUi::BeginMenu(3, 1);

    float music_volume = sound_manager.get_volume("music");
    float fx_volume = sound_manager.get_volume("fx");
    float ui_volume = sound_manager.get_volume("ui");

    CoreUi::CenterText("Music volume:");
    if (CoreUi::CenterSlider("##music volume", &music_volume, 0.0, 1.0, 220)) {
        sound_manager.set_volume("music", music_volume);
    }
    GameUi::Spacing();

    CoreUi::CenterText("Vfx volume:");
    if (CoreUi::CenterSlider("##fx volume", &fx_volume, 0.0, 1.0, 220)) {
        sound_manager.set_volume("fx", fx_volume);
    }
    GameUi::Spacing();

    CoreUi::CenterText("Ui volume:");
    if (CoreUi::CenterSlider("##ui volume", &ui_volume, 0.0, 1.0, 220)) {
        sound_manager.set_volume("ui", ui_volume);
    }
    GameUi::Spacing();

    should_close = GameUi::MenuButton("Back");
    GameUi::EndMenu();
    return should_close;
}