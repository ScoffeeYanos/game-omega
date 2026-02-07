#pragma once

#include <cppgl.h>
#include <imgui/imgui.h>

namespace CoreUi {

inline ImVec2 ToImVec(const glm::vec2& v) { return {v.x, v.y}; }

inline bool BeginEmpty(const char* title, bool* ptr = nullptr) {
    return ImGui::Begin(title, ptr,
                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground |
                                ImGuiWindowFlags_NoSavedSettings);
}

inline bool BeginTextureWindow(const char* title, const cppgl::Texture2D& texture, const glm::vec2& pos,
                               float scale = 1.0) {
    glm::vec2 size = {scale * texture->w, scale * texture->h};

    ImGui::SetNextWindowPos(ToImVec(pos), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ToImVec(size));
    if (BeginEmpty(title)) {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        draw_list->AddImage((ImTextureID)(intptr_t)texture->id, ToImVec(pos), ToImVec(pos + size), ImVec2(0, 1),
                            ImVec2(1, 0));

        return true;
    }

    return false;
}

inline bool BeginMenu() {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    return BeginEmpty("##game menu");
};

inline void CenterContainer(float height) {
    float screenHeight = ImGui::GetWindowSize().y;
    ImGui::SetCursorPosY((screenHeight - height) * 0.5f);
}

inline void CenterElement(float width) {
    float windowWidth = ImGui::GetWindowSize().x;
    ImGui::SetCursorPosX((windowWidth - width) * 0.5f);
};

inline void CenterText(const char* text) {
    CenterElement(ImGui::CalcTextSize(text).x);
    ImGui::Text(text);
}

inline bool CenterButton(const char* text, const ImVec2& size) {
    CenterElement(size.x);
    return ImGui::Button(text, size);
}

inline bool CenterSlider(const char* text, float* v, float v_min, float v_max, float width) {
    CenterElement(width);
    ImGui::PushItemWidth(width);
    bool b = ImGui::SliderFloat(text, v, v_min, v_max);
    ImGui::PopItemWidth();
    return b;
}

}; // namespace CoreUi