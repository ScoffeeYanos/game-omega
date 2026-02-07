#include "debug-manager.h"

#include <functional>

#include <imgui/imgui.h>

std::vector<std::string> DebugManager::split_key(const std::string& key) {
    std::vector<std::string> keys;
    size_t start = 0, pos;
    while ((pos = key.find('.', start)) != std::string::npos) {
        keys.push_back(key.substr(start, pos - start));
        start = pos + 1;
    }
    keys.push_back(key.substr(start));

    return keys;
};

DebugManager::ValueType& DebugManager::get_value(const std::vector<std::string>& keys) {
    size_t i = 0;
    Node* current = &root;
    while (i < keys.size() - 1) {
        current = current->nodes.at(keys[i]).get();
        i++;
    };
    return current->values.at(keys[i]);
}

DebugManager::ValueType& DebugManager::create_value(const std::vector<std::string>& keys) {
    size_t i = 0;
    Node* current = &root;
    while (i < keys.size() - 1) {
        if (current->nodes.find(keys[i]) == current->nodes.end()) {
            current->nodes[keys[i]] = std::make_unique<Node>();
        }
        current = current->nodes[keys[i]].get();
        i++;
    };
    return current->values[keys[i]];
}

void DebugManager::draw_value(ValueType& current, const std::string& key) {
    std::visit(
            [name = key.c_str()](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>) {
                    ImGui::InputInt(name, &arg);
                } else if constexpr (std::is_same_v<T, float>) {
                    ImGui::SliderFloat(name, &arg, 0, 1);
                } else if constexpr (std::is_same_v<T, bool>) {
                    ImGui::Checkbox(name, &arg);
                } else {
                    static_assert(false, "Non-exhaustive visitor");
                }
            },
            current);
}

void DebugManager::draw_node(Node* current, const std::string& key) {
    if (ImGui::TreeNode((key + "##tree").c_str())) {
        for (auto& [value_key, value] : current->values) {
            draw_value(value, value_key);
        }
        for (auto& [node_key, node] : current->nodes) {
            draw_node(node.get(), node_key);
        }
        ImGui::TreePop();
    }
};