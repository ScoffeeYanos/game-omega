#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

/// @brief A managed debug value store for values of type int, float or bool. It provides easy access through named keys
/// and an ImGui helper to draw value controls (InputInt, SliderFloat, Checkbox).
class DebugManager {
    using ValueType = std::variant<int, float, bool>;

    struct Node {
        std::unordered_map<std::string, ValueType> values;
        std::unordered_map<std::string, std::unique_ptr<Node>> nodes;
    } root{};

public:
    DebugManager() = default;

    /// @brief Add a tracked debug value to this manager. Must be called once for every key-value pair.
    /// @tparam T The type of the value (int, float, bool)
    /// @param key Dots ('.') in the key will be interpreted as a tree hierarchy when drawn.
    /// @param value The default value
    template <typename T> void add(const std::string& key, T value) {
        auto& v = create_value(split_key(key));
        v = ValueType{value};
    }

    /// @brief Get a tracked value.
    /// @tparam T The type of the value (int, float, bool)
    /// @param key The key for the value
    /// @return The stored value
    template <typename T> T get(const std::string& key) { return std::get<T>(get_value(split_key(key))); }

    /// @brief Draws all key-value pairs. For each subtree first the values get drawn and then further subtrees if any
    /// exist. The order of addition won't be preserved.
    void draw_gui() {
        for (auto& [value_key, value] : root.values) {
            draw_value(value, value_key);
        }
        for (auto& [node_key, node] : root.nodes) {
            draw_node(node.get(), node_key);
        }
    }

private:
    ValueType& get_value(const std::vector<std::string>& keys);
    ValueType& create_value(const std::vector<std::string>& keys);

    void draw_node(Node* current, const std::string& key);
    void draw_value(ValueType& current, const std::string& key);

    static std::vector<std::string> split_key(const std::string& key);
};