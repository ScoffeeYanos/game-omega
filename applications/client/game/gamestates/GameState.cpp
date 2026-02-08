#include "GameState.h"
using namespace cppgl;

void GameState::enter(MyGame& game, GameStates old_state_type) {
    world = std::make_unique<World>();

    //make_camera_current(the_player->get_playercam());
}

void GameState::loop(MyGame& game, float dt) {
    // DebugDraw::clear();
    //
    // if (cppgl::current_camera()->name == "default") {
    //     cppgl::CameraImpl::default_input_handler(cppgl::Context::frame_time());
    //     cppgl::current_camera()->update();
    // }
    //
    // if (!paused) {
    //     update(dt);
    //
    //     render(game.renderer);
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

    // renderer.submit_light(sky->get_sun());
    // PointLight player_light;
    // player_light.pos = player->position() + glm::vec3{0, 0.5, 0};
    // player_light.ambient = {.8, 0.6, 0.5};
    // player_light.diffuse = {.8, 0.6, 0.5};
    // player_light.radius = 3;
    // player_light.intensity = 0.5;
    // renderer.submit_light(&player_light);
    //
    // renderer.geometry_pass();
    // wasteland->render(renderer);
    // renderer.shadow_pass(sky->get_sun(), player->position());
    // renderer.ssao_pass();
    // renderer.lighting_pass();
    // renderer.bloom_pass();
    // renderer.final_pass();
    //
    // renderer.overlay_pass();
    //
    // world->draw_particles(renderer);
    // player->draw_particles(&renderer);
    //
    // sky->render();

    // if (Context::instance().show_gui) {
    //     DebugDraw::draw();
    // }
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
