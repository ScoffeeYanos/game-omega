#include "GameState.h"
#include <glm/glm.hpp>
using namespace cppgl;

void GameState::enter(MyGame& game, GameStates old_state_type) {
    world = std::make_unique<World>();

    const glm::vec3 target{11.7f, 0.0f, 6.75f};
    const glm::vec3 camera_pos{11.7f, 12.0f, 22.0f};
    cppgl::current_camera()->from_lookat(camera_pos, target, glm::vec3{0.0f, 1.0f, 0.0f});

}

void GameState::loop(MyGame& game, float dt) {
    // DebugDraw::clear();

    cppgl::CameraImpl::default_input_handler(cppgl::Context::frame_time());
    cppgl::current_camera()->update();

    // if (!paused) {
    //     update(dt);
    //
    render(game.renderer);
    //
    //     if (!ImGui::GetIO().WantCaptureMouse && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    //         build_mode->handle_click();
    //     }
    //
    //     if (show_hud) {
    //         build_mode->draw_ui();
    //         PlayerUI::ShowInventory(the_player->inventory_);
    //         game.draw_notification();
    //     }
    //     if (Context::instance().show_gui) {
    //         draw_debug_menu(game);
    //     }
    //
    //     //game.draw_notification();
    // } else {
    //     render(game.renderer);
    //
    //     switch (pause_menu) {
    //     case Menu::MAIN:
    //         draw_pause_menu();
    //         break;
    //     case Menu::INFO:
    //         if (game.draw_info_menu()) {
    //             pause_menu = Menu::MAIN;
    //         }
    //         break;
    //     case Menu::SETTINGS:
    //         if (game.draw_settings_menu()) {
    //             pause_menu = Menu::MAIN;
    //         }
    //     }
    // }
};

void GameState::keyboard(MyGame& game, int key, int scancode, int action, int mods) {
    // if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
    //     if (paused) {
    //         if (pause_menu == Menu::MAIN) {
    //             paused = false;
    //         } else {
    //             pause_menu = Menu::MAIN;
    //         }
    //     } else {
    //         paused = true;
    //     }
    // }
    //
    // if (action == GLFW_PRESS && key == GLFW_KEY_F3) {
    //     show_hud = !show_hud;
    // }
    // if (cppgl::current_camera()->name == "player_cam") {
    //     player->handle_keyboard(action, key);
    // }
}

void GameState::update(float dt) {
    // world->update(dt);

    // auto& cam = player->get_playercam();
    // Platform::sound_engine().set3dListenerPosition(cam->pos.x, cam->pos.y, cam->pos.z);
    // Platform::sound_engine().set3dListenerAt(cam->dir.x, cam->dir.y, cam->dir.z);
    // Platform::sound_engine().update3dAudio();

};

void GameState::render(Renderer& renderer) {
    renderer.setup_frame();

    world->submit(renderer);

    DirectionalLight sun;
    sun.dir = glm::normalize(glm::vec3{-0.4f, -1.0f, -0.3f});
    sun.ambient = glm::vec3{0.25f, 0.25f, 0.25f};
    sun.diffuse = glm::vec3{0.9f, 0.85f, 0.8f};
    sun.specular = glm::vec3{0.2f, 0.2f, 0.2f};
    renderer.submit_light(&sun);


    renderer.geometry_pass();
    // renderer.shadow_pass(&sun, glm::vec3{0.0f});
    // renderer.ssao_pass();
    renderer.lighting_pass();
    // renderer.bloom_pass();
    renderer.final_pass();
    // renderer.overlay_pass();

}

void GameState::draw_debug_menu(MyGame& game) {
    // if (ImGui::Begin("Debug")) {
    //     game.renderer.draw_debug_gui();
    //     ImGui::Separator();
    //     sky->draw_debug_gui();
    // }
    // ImGui::End();
    //
    // if (ImGui::Begin("Player Debug")) {
    //     if (!the_player) {
    //         ImGui::TextUnformatted("No player available.");
    //     } else {
    //         const glm::vec3 pos = the_player->position();
    //         const glm::vec3 vel = the_player->velocity();
    //         ImGui::Text("Position: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
    //         ImGui::Text("Velocity: (%.2f, %.2f, %.2f)", vel.x, vel.y, vel.z);
    //         ImGui::Text("Target Input: forward %.2f / right %.2f", the_player->target_forward(),
    //                     the_player->target_right());
    //         ImGui::Text("Cam Usage: Q/E cam rotation, Arrow buttons Zoom.");
    //     }
    // }
    // ImGui::End();
}

void GameState::draw_pause_menu() {
    // GameUi::BeginMenu(0, 4);
    // if (GameUi::MenuButton("Continue")) {
    //     paused = false;
    // }
    // GameUi::Spacing();
    // if (GameUi::MenuButton("Info")) {
    //     pause_menu = Menu::INFO;
    // }
    // GameUi::Spacing();
    // if (GameUi::MenuButton("Settings")) {
    //     pause_menu = Menu::SETTINGS;
    // }
    // GameUi::Spacing();
    // if (GameUi::MenuButton("Menu")) {
    //     Game::switch_state<MenuState>();
    // }
    // GameUi::EndMenu();
}
