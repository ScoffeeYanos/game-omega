#pragma once

#include <cppgl.h>
#include <imgui/imgui.h>

#include "core/sound-manager.h"

class Inventory;
struct ItemStack;
struct ItemDef;

namespace UiDefs {
void Init(SoundManager& sound_manager);

bool MenuButton(const char* text);

void Spacing();

void BeginMenu(int lines, int buttons, float reserved = 0.0);
void EndMenu();
} // namespace GameUi
