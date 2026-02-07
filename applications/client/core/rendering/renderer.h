#pragma once
#include <vector>
#include "light.h"
#include "model.h"
#include "ecs/storage.h"
struct RenderModel
{
    const Model*              model;
    uint32_t                  flags;
    static constexpr uint32_t kShadowFlag=1u<<0;
    RenderModel(const Model* model_in,uint32_t flags_in): model(model_in), flags(flags_in)
    {
    }
};
struct Transform
{
    glm::mat4 matrix;
    explicit  Transform(const glm::mat4 &matrix_in): matrix(matrix_in)
    {
    }
};
struct Tint
{
    glm::vec3 color{1.0f};
    float     alpha{1.0f};
    Tint(const glm::vec3 &color_in,float alpha_in): color(color_in), alpha(alpha_in)
    {
    }
};
/// @brief Manages different render passes and their buffers and shaders. Tracks geometry and lights through render
/// queues, to which data can be submitted for each frame.
class Renderer
{
public:
    Renderer();
    /// @brief Draw submitted geometry into the gbuffer for deferred rendering.
    void geometry_pass();
    /// @brief Draw submitted geometry with shadow flag into shadow map.
    /// @param shadow_cam Camera from which shadow map will be rendered
    void shadow_pass(const DirectionalLight* light,const glm::vec3 &pos);
    /// @brief Light the scene with submitted lights using deferred lighting.
    void lighting_pass();
    /// @brief Apply screen space ambient occlusion to rendered scene.
    void ssao_pass();
    /// @brief Apply bloom to rendered scene.
    void bloom_pass();
    /// @brief Blits the final color + depth result to the screen.
    void final_pass();
    /// @brief Renders transparent objects
    void overlay_pass();
    /// @brief Resets render queues, must be called at the beginning of each frame.
    void setup_frame();
    /// @brief Submit a point light for deferred lighting
    void submit_light(const PointLight* light)
    {
        m_point_lights.push_back(light);
    };
    /// @brief Submit a direction light for deferred lighting
    void submit_light(const DirectionalLight* light)
    {
        m_directional_lights.push_back(light);
    };
    /// @brief Submit a model for deferred rendering
    /// @param shadows Controls whether the geometry can cast shadows
    void submit_model(const Model*     model,const glm::mat4 &transform=glm::mat4{1.0},
                      const glm::vec3 &color=glm::vec3{1.0f},float alpha=1.0f,bool shadows=true)
    {
        auto entity=m_render_storage.create_entity();
        m_render_storage.emplace<RenderModel>(entity,model,shadows?RenderModel::kShadowFlag:0u);
        m_render_storage.emplace<Transform>(entity,transform);
        m_render_storage.emplace<Tint>(entity,color,alpha);
    }
    void  draw_debug_gui();
    void  on_resize(const glm::ivec2 &res);
    auto &buffers()
    {
        return m_buffers;
    }
private:
    void init_geometry(const glm::ivec2 &res);
    void init_shadows(const glm::ivec2 &res);
    void init_ssao(const glm::ivec2 &res);
    void init_lighting(const glm::ivec2 &res);
    void init_bloom(const glm::ivec2 &res);
    void init_final();
    struct
    {
        cppgl::Framebuffer geometry;
        cppgl::Framebuffer lighting;
        cppgl::Framebuffer shadows_near;
        cppgl::Framebuffer shadows_far;
        cppgl::Texture2D   ssao;
        cppgl::Texture2D   ssao_blur;
        cppgl::Texture2D   bloom_brightness;
        cppgl::Texture2D   bloom_blur;
    } m_buffers;
    struct
    {
        cppgl::Shader geometry;
        cppgl::Shader lighting;
        cppgl::Shader unlit;
        cppgl::Shader final;
        // Shadows
        cppgl::Shader shadow_shader;
        // SSAO
        cppgl::Shader ssao_shader;
        cppgl::Shader ssao_blur_shader;
        // Bloom
        cppgl::Shader bloom_extract_shader;
        cppgl::Shader bloom_blur_shader;
        // previews
        cppgl::Shader colored_preview;
    } m_shaders;
    struct PassSettings
    {
        bool geometry_enabled{true};
        bool shadows_enabled{true};
        bool ssao_enabled{true};
        bool lighting_enabled{true};
        bool bloom_enabled{true};
        bool final_enabled{true};
    } m_pass_settings;
    struct SSAOSettings
    {
        float radius{0.6};
        int   sample_count{32};
        int   blur_radius{3};
        float bias{0.025};
        float intensity{1.0};
    } m_ssao_settings;
    struct BloomSettings
    {
        float threshold_low{1.0};
        float threshold_high{2.0};
        float contribution_min{0.8};
        float contribution_max{1.2};
        float intensity{0.8};
    }                                    m_bloom_settings;
    cppgl::Camera                        m_shadow_cam_near;
    cppgl::Camera                        m_shadow_cam_far;
    float                                m_shadow_intensity{1.0};
    Storage                              m_render_storage;
    std::vector<const PointLight*>       m_point_lights;
    std::vector<const DirectionalLight*> m_directional_lights;
};
