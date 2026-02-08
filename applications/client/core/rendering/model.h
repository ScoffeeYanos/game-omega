#pragma once

#include <string>
#include <vector>

#include <cppgl.h>

/// @brief Helper class to manage models
class Model {
public:
    Model() = default;
    explicit Model(const std::string& path) { load(path); }

    /// @brief Load a model from a obj file
    /// @param path Filepath of the obj file
    /// @note Each model should only be loaded once, for lazy loading the methed loaded() can be used.
    void load(const std::string& path) {
        m_path = path;
        m_meshes = cppgl::load_meshes_gpu(path);
    }

    /// @brief Draw the stored meshes.
    /// @param shader Shader which will be used
    /// @param transform Model matrix which will be used to transform the mesh to world pos
    void draw(cppgl::Shader& shader, const glm::mat4& transform) const {
        const glm::mat4 combined_transform = transform * m_base_transform;
        shader->uniform("model", combined_transform);
        shader->uniform("model_normal", glm::transpose(glm::inverse(combined_transform)));
        for (const auto& mesh : m_meshes) {
            mesh->bind(shader);
            mesh->draw();
            mesh->unbind();
        }
    }

    std::string_view path() const { return m_path; }
    const std::vector<cppgl::Mesh>& meshes() const { return m_meshes; }
    const glm::mat4& base_transform() const { return m_base_transform; }
    void set_base_transform(const glm::mat4& transform) { m_base_transform = transform; }

    /// @brief Can be used for lazy loading.
    /// @return true if the model object has loaded mesh data, false otherwise
    bool loaded() const { return !m_path.empty(); }

private:
    std::string m_path;
    std::vector<cppgl::Mesh> m_meshes;
    glm::mat4 m_base_transform{1.0f};
};
