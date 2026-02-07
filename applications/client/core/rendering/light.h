#pragma once

#include <cppgl.h>
#include <glm/glm.hpp>

struct PointLight {
    glm::vec3 pos;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float radius;
    float intensity;

    void setup_light(cppgl::Shader& shader, size_t i) const {
        shader->uniform(std::string("p_lights[") + std::to_string(i) + "].pos", pos);
        shader->uniform(std::string("p_lights[") + std::to_string(i) + "].ambient", ambient);
        shader->uniform(std::string("p_lights[") + std::to_string(i) + "].diffuse", diffuse);
        shader->uniform(std::string("p_lights[") + std::to_string(i) + "].specular", specular);
        shader->uniform(std::string("p_lights[") + std::to_string(i) + "].radius", radius);
        shader->uniform(std::string("p_lights[") + std::to_string(i) + "].intensity", intensity);
    }
};

struct DirectionalLight {
    glm::vec3 dir;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    void setup_light(cppgl::Shader& shader, size_t i) const {
        shader->uniform(std::string("d_lights[") + std::to_string(i) + "].dir", dir);
        shader->uniform(std::string("d_lights[") + std::to_string(i) + "].ambient", ambient);
        shader->uniform(std::string("d_lights[") + std::to_string(i) + "].diffuse", diffuse);
        shader->uniform(std::string("d_lights[") + std::to_string(i) + "].specular", specular);
    }
};
