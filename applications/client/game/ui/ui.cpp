#include "ui.h"

#include "core/ui.h"

namespace GameUi {

static constexpr float BUTTON_H = 42.0f;
static constexpr float SPACING = 10.0f;

static Sound hover_sound;
static Sound click_sound;
static ImGuiStorage* storage;

void Init(SoundManager& sound_manager) {
    storage = ImGui::GetStateStorage();
    hover_sound = sound_manager.load_sound("data/sound-data/ui/hover.mp3", "ui");
    click_sound = sound_manager.load_sound("data/sound-data/ui/click.mp3", "ui");
}

void BeginMenu(int lines, int buttons, float reserved) {
    CoreUi::BeginMenu();
    float height = lines * ImGui::GetTextLineHeight() + buttons * BUTTON_H +
                   std::min(lines + buttons - (reserved == 0), 0) * SPACING + reserved;
    CoreUi::CenterContainer(height);

    ImGui::PushStyleColor(ImGuiCol_Button, {33. / 255., 37. / 255., 40. / 255., 0.9});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {75. / 255., 78. / 255., 79. / 255., 0.9});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, {33. / 255., 37. / 255., 40. / 255., 1.0});

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
}

void EndMenu() {
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::End();
}

void Spacing() { ImGui::Dummy({0, SPACING}); }

bool MenuButton(const char* text) {
    bool pressed = CoreUi::CenterButton(text, {220, BUTTON_H});
    if (pressed) {
        click_sound.play();
    }
    ImGuiID id = ImGui::GetItemID();
    bool was_hovered = storage->GetBool(id, false);
    bool is_hovered = ImGui::IsItemHovered();
    if (is_hovered && !was_hovered) {
        hover_sound.play();
    }
    storage->SetBool(id, is_hovered);
    return pressed;
};

} // namespace GameUi
