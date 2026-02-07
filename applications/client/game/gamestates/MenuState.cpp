#include "MenuState.h"

using namespace cppgl;

void MenuState::enter(MyGame& game, GameStates old_state_type) {}

void MenuState::loop(MyGame& game, float dt) {
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //
    // static Texture2D bg_image = Texture2D{"menu-brackground", "data/render-data/images/scale.jpeg", false, false};
    //
    // ImDrawList* bg = ImGui::GetBackgroundDrawList();
    //
    // ImVec2 vp_pos = ImGui::GetMainViewport()->Pos;
    // ImVec2 vp_size = ImGui::GetMainViewport()->Size;
    //
    // float img_w = (float)bg_image->w;
    // float img_h = (float)bg_image->h;
    //
    // float scale = std::max(vp_size.x / img_w, vp_size.y / img_h);
    //
    // ImVec2 draw_size = ImVec2(img_w * scale, img_h * scale);
    //
    // ImVec2 draw_pos = ImVec2(vp_pos.x + (vp_size.x - draw_size.x) * 0.5f, vp_pos.y + (vp_size.y - draw_size.y) * 0.5f);
    //
    // bg->AddImage((ImTextureID)(intptr_t)bg_image->id, draw_pos,
    //              ImVec2(draw_pos.x + draw_size.x, draw_pos.y + draw_size.y));
    //
    // switch (m_menu) {
    // case Menu::MAIN:
    //     draw_main_menu();
    //     break;
    // case Menu::INFO:
    //     if (game.draw_info_menu()) {
    //         m_menu = Menu::MAIN;
    //     };
    //     break;
    // case Menu::SETTINGS:
    //     if (game.draw_settings_menu()) {
    //         m_menu = Menu::MAIN;
    //     };
    //     break;
    // }
}

void MenuState::keyboard(MyGame& game, int key, int scancode, int action, int mods) {
    // if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    //     if (m_menu == Menu::MAIN) {
    //         glfwSetWindowShouldClose(Context::instance().glfw_window, 1);
    //     } else {
    //         m_menu = Menu::MAIN;
    //     }
    // }
}

void MenuState::draw_main_menu() {
    // static Texture2D title_image = Texture2D{"menu-title", "data/render-data/images/title.png", false, false};
    //
    // GameUi::BeginMenu(0, 4, 200);
    // CoreUi::CenterElement(600);
    // ImVec2 pos = ImGui::GetCursorScreenPos();
    // ImVec2 size(600, 200);
    //
    // ImDrawList* draw_list = ImGui::GetWindowDrawList();
    // draw_list->AddImage((ImTextureID)(intptr_t)title_image->id, pos, ImVec2(pos.x + size.x, pos.y + size.y));
    //
    // ImGui::Dummy({600, 200});
    // GameUi::Spacing();
    // if (GameUi::MenuButton("Play")) {
    //     Game::switch_state<IntroState>();
    // }
    // GameUi::Spacing();
    // if (GameUi::MenuButton("Info")) {
    //     m_menu = Menu::INFO;
    // }
    // GameUi::Spacing();
    // if (GameUi::MenuButton("Settings")) {
    //     m_menu = Menu::SETTINGS;
    // }
    // GameUi::Spacing();
    // if (GameUi::MenuButton("Exit")) {
    //     glfwSetWindowShouldClose(Context::instance().glfw_window, 1);
    // }
    // GameUi::EndMenu();
}
