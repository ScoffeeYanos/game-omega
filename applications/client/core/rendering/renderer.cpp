#include "renderer.h"

#include "buffer.h"
#include "util.h"
#include <string>

using namespace cppgl;
using namespace rendering;

Renderer::Renderer() {
    auto res = Context::resolution();

    init_geometry(res);
    init_shadows(res);
    init_ssao(res);
    init_lighting(res);
    init_bloom(res);
    init_final();
}

void Renderer::init_geometry(const glm::ivec2& res) {
    auto geom = Framebuffer{"geom", res.x, res.y};
    geom->attach_depthbuffer(
            Texture2D{"geom.depth", res.x, res.y, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT});
    geom->attach_colorbuffer(Texture2D{"geom.diff", res.x, res.y, GL_RGB32F, GL_RGB, GL_FLOAT});
    geom->attach_colorbuffer(Texture2D{"geom.pos", res.x, res.y, GL_RGB32F, GL_RGB, GL_FLOAT});
    geom->attach_colorbuffer(Texture2D{"geom.norm", res.x, res.y, GL_RGB32F, GL_RGB, GL_FLOAT});
    util::clamp_to_edge(geom->depth_texture);
    for (const auto& tex : geom->color_textures) {
        util::clamp_to_edge(tex);
    }
    geom->check();

    m_buffers.geometry = geom;

    m_shaders.geometry = Shader{"pos_norm_tc", "core/shader/pos+norm+tc.vs", "core/shader/pos+norm+tc.fs"};
    m_shaders.colored_preview =
            Shader{"colored_preview", "core/shader/pos+norm+tc.vs", "core/shader/colored_preview.fs"};
}

void shadow_map_setup(Texture2D& tex) {
    glTextureParameteri(tex->id, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTextureParameteri(tex->id, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glTextureParameteri(tex->id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(tex->id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTextureParameterfv(tex->id, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTextureParameteri(tex->id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(tex->id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Renderer::init_shadows(const glm::ivec2& res) {
    auto shadow = Framebuffer{"shadow-near", 2048, 2048};
    shadow->attach_depthbuffer(
            Texture2D{"shadow-near.depth", 2048, 2048, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT});
    shadow_map_setup(shadow->depth_texture);
    shadow->check();
    m_buffers.shadows_near = shadow;

    shadow = Framebuffer{"shadow-far", 2048, 2048};
    shadow->attach_depthbuffer(
            Texture2D{"shadow-far.depth", 2048, 2048, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT});
    shadow_map_setup(shadow->depth_texture);

    m_buffers.shadows_far = shadow;

    m_shaders.shadow_shader = Shader{"shadow", "core/shader/pos.vs", "core/shader/empty.fs"};

    m_shadow_cam_near = Camera{"shadow_cam_near"};
    m_shadow_cam_near->perspective = false;
    m_shadow_cam_near->far = 200;
    m_shadow_cam_near->near = 50;
    m_shadow_cam_near->left = -10;
    m_shadow_cam_near->right = 10;
    m_shadow_cam_near->top = 10;
    m_shadow_cam_near->bottom = -10;

    m_shadow_cam_far = Camera{"shadow_cam_far"};
    m_shadow_cam_far->perspective = false;
    m_shadow_cam_far->far = 200;
    m_shadow_cam_far->near = 30;
    m_shadow_cam_far->left = -30;
    m_shadow_cam_far->right = 30;
    m_shadow_cam_far->top = 30;
    m_shadow_cam_far->bottom = -30;
}

void Renderer::init_ssao(const glm::ivec2& res) {
    glm::ivec2 half_res = max(res / 2, glm::ivec2(1));
    auto ssao = Texture2D{"ssao", half_res.x, half_res.y, GL_R16F, GL_RED, GL_FLOAT};
    auto ssao_blur = Texture2D{"ssao_blur", half_res.x, half_res.y, GL_R16F, GL_RED, GL_FLOAT};

    util::clamp_to_edge(ssao);
    util::clamp_to_edge(ssao_blur);
    util::linear_filter(ssao);
    util::linear_filter(ssao_blur);

    m_buffers.ssao = ssao;
    m_buffers.ssao_blur = ssao_blur;

    m_shaders.ssao_shader = Shader{"ssao", "core/shader/ssao.cs"};
    m_shaders.ssao_blur_shader = Shader{"ssao_blur", "core/shader/ssao_blur.cs"};
}

void Renderer::init_lighting(const glm::ivec2& res) {
    auto lighting = Framebuffer{"lighting", res.x, res.y};
    lighting->attach_depthbuffer(
            Texture2D{"lighting.depth", res.x, res.y, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT});
    lighting->attach_colorbuffer(Texture2D{"lighting.color", res.x, res.y, GL_RGB32F, GL_RGB, GL_FLOAT});
    util::clamp_to_edge(lighting->depth_texture);
    for (const auto& tex : lighting->color_textures) {
        util::clamp_to_edge(tex);
    }
    lighting->check();

    m_buffers.lighting = lighting;

    m_shaders.lighting = Shader{"deferred_lighting", "core/shader/copytex.vs", "core/shader/deferred_lighting.fs"};
    m_shaders.unlit = Shader{"unlit", "core/shader/copytex.vs", "core/shader/unlit.fs"};
}

void Renderer::init_bloom(const glm::ivec2& res) {
    glm::ivec2 quarter_res = max(res / 4, glm::ivec2(1));
    auto bloom_brightness = Texture2D{"bloom.brightness", res.x, res.y, GL_RGBA16F, GL_RGBA, GL_FLOAT, nullptr, true};
    auto bloom_blur = Texture2D{"bloom.blur", quarter_res.x, quarter_res.y, GL_RGBA16F, GL_RGBA, GL_FLOAT};

    util::clamp_to_edge(bloom_brightness);
    util::clamp_to_edge(bloom_blur);
    glTextureParameteri(bloom_brightness->id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(bloom_brightness->id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    util::linear_filter(bloom_blur);

    m_buffers.bloom_brightness = bloom_brightness;
    m_buffers.bloom_blur = bloom_blur;

    m_shaders.bloom_extract_shader = Shader{"bloom_extract", "core/shader/bloom_extract.cs"};
    m_shaders.bloom_blur_shader = Shader{"bloom_blur", "core/shader/bloom_blur.cs"};
}

void Renderer::init_final() { m_shaders.final = Shader{"final", "core/shader/copytex.vs", "core/shader/final.fs"}; }

void Renderer::on_resize(const glm::ivec2& res) {
    m_buffers.geometry->resize(res.x, res.y);
    m_buffers.lighting->resize(res.x, res.y);

    glm::ivec2 half_res = max(res / 2, glm::ivec2(1));
    m_buffers.ssao->resize(half_res.x, half_res.y);
    m_buffers.ssao_blur->resize(half_res.x, half_res.y);

    glm::ivec2 quarter_res = max(res / 4, glm::ivec2(1));
    m_buffers.bloom_brightness->resize(quarter_res.x, quarter_res.y);
    m_buffers.bloom_blur->resize(quarter_res.x, quarter_res.y);
}

void Renderer::setup_frame() {
    m_instances.clear();
    m_colored_instances.clear();
    m_point_lights.clear();
    m_directional_lights.clear();

    static uint32_t reload_counter = 0;
    if (reload_counter++ % 100 == 0) {
        reload_counter = 1;
        cppgl::reload_modified_shaders();
    }

    glDepthMask(GL_TRUE);
}

void Renderer::geometry_pass() {
    auto& buffer = m_buffers.geometry;

    buffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_pass_settings.geometry_enabled) {
        auto& shader = m_shaders.geometry;
        shader->bind();
        util::setup_camera(shader, current_camera());
        shader->uniform("view", current_camera()->view);
        shader->uniform("view_normal", current_camera()->view_normal);
        shader->uniform("proj", current_camera()->proj);

        for (const auto& inst : m_instances) {
            inst.model->draw(shader, inst.transform);
        }
        shader->unbind();
    }

    buffer->unbind();
}

void Renderer::shadow_pass(const DirectionalLight* light, const glm::vec3& pos) {
    if (!m_pass_settings.shadows_enabled || light->dir.y > 0.0) {
        float depth = 1.0f;
        glClearTexImage(m_buffers.shadows_near->depth_texture->id, 0, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        glClearTexImage(m_buffers.shadows_far->depth_texture->id, 0, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        return;
    }

    m_shadow_intensity = glm::smoothstep(0.0f, 0.2f, -light->dir.y);

    m_shadow_cam_near->from_lookat(pos - light->dir * 100.0f, pos);

    m_shadow_cam_far->from_lookat(pos - light->dir * 100.0f, pos);

    m_buffers.shadows_near->bind();

    glClear(GL_DEPTH_BUFFER_BIT);

    auto& shader = m_shaders.shadow_shader;
    shader->bind();
    util::setup_camera(shader, m_shadow_cam_near);
    shader->uniform("view", m_shadow_cam_near->view);
    shader->uniform("proj", m_shadow_cam_near->proj);

    for (const auto& inst : m_instances) {
        if (inst.shadows) {
            inst.model->draw(shader, inst.transform);
        }
    }
    shader->unbind();
    m_buffers.shadows_near->unbind();

    m_buffers.shadows_far->bind();

    glClear(GL_DEPTH_BUFFER_BIT);

    shader->bind();

    util::setup_camera(shader, m_shadow_cam_far);
    shader->uniform("view", m_shadow_cam_far->view);
    shader->uniform("proj", m_shadow_cam_far->proj);

    for (const auto& inst : m_instances) {
        if (inst.shadows) {
            inst.model->draw(shader, inst.transform);
        }
    }
    shader->unbind();
    m_buffers.shadows_far->unbind();
}

void Renderer::ssao_pass() {
    if (!m_pass_settings.ssao_enabled) {
        float white = 1.0;
        glClearTexImage(m_buffers.ssao_blur->id, 0, GL_RED, GL_FLOAT, &white);
        return;
    }

    auto& shader = m_shaders.ssao_shader;
    auto& ssao = m_buffers.ssao;
    auto& ssao_blur = m_buffers.ssao_blur;
    shader->bind();
    shader->uniform("geom.depth", m_buffers.geometry->depth_texture, 0);
    shader->uniform("geom.pos", m_buffers.geometry->color_textures[1], 1);
    shader->uniform("geom.norm", m_buffers.geometry->color_textures[2], 2);
    shader->uniform("view", current_camera()->view);
    shader->uniform("proj", current_camera()->proj);
    shader->uniform("radius", m_ssao_settings.radius);
    shader->uniform("sample_count", m_ssao_settings.sample_count);
    shader->uniform("bias", m_ssao_settings.bias);
    ssao->bind_image(0, GL_WRITE_ONLY, GL_R16F);
    shader->dispatch_compute(ssao->w, ssao->h, 1, GL_ALL_BARRIER_BITS);
    ssao->unbind_image(0);
    shader->unbind();

    shader = m_shaders.ssao_blur_shader;
    shader->bind();
    shader->uniform("input_tex", ssao, 0);
    shader->uniform("blur_radius", m_ssao_settings.blur_radius);
    ssao_blur->bind_image(0, GL_WRITE_ONLY, GL_R16F);
    shader->dispatch_compute(ssao_blur->w, ssao_blur->h, 1, GL_ALL_BARRIER_BITS);
    ssao_blur->unbind_image(0);
    shader->unbind();
}

void Renderer::lighting_pass() {
    auto& buffer = m_buffers.lighting;

    buffer->bind();
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    if (m_pass_settings.lighting_enabled) {
        auto& shader = m_shaders.lighting;
        shader->bind();

        util::setup_camera(shader, current_camera());
        shader->uniform("geom.depth", m_buffers.geometry->depth_texture, 0);
        shader->uniform("geom.diff", m_buffers.geometry->color_textures[0], 1);
        shader->uniform("geom.pos", m_buffers.geometry->color_textures[1], 2);
        shader->uniform("geom.norm", m_buffers.geometry->color_textures[2], 3);

        shader->uniform("shadow_near.view", m_shadow_cam_near->view);
        shader->uniform("shadow_near.proj", m_shadow_cam_near->proj);
        shader->uniform("shadow_near.depth", m_buffers.shadows_near->depth_texture, 4);

        shader->uniform("shadow_far.view", m_shadow_cam_far->view);
        shader->uniform("shadow_far.proj", m_shadow_cam_far->proj);
        shader->uniform("shadow_far.depth", m_buffers.shadows_far->depth_texture, 5);

        shader->uniform("shadow_intensity", m_shadow_intensity);

        // SSAO uniforms
        shader->uniform("ssao", m_buffers.ssao_blur, 6);
        shader->uniform("ssao_intensity", m_ssao_settings.intensity);

        shader->uniform("p_light_count", (int)m_point_lights.size());
        for (size_t i = 0; i < m_point_lights.size(); ++i) {
            m_point_lights[i]->setup_light(shader, i);
        }

        shader->uniform("d_light_count", (int)m_directional_lights.size());
        for (size_t i = 0; i < m_directional_lights.size(); ++i) {
            m_directional_lights[i]->setup_light(shader, i);
        }

        util::quad_with_depth();

        shader->unbind();
    } else {
        auto& shader = m_shaders.unlit;

        shader->bind();

        shader->uniform("geom.depth", m_buffers.geometry->depth_texture, 0);
        shader->uniform("geom.diff", m_buffers.geometry->color_textures[0], 1);

        util::quad_with_depth();

        shader->unbind();
    }
    buffer->unbind();
}

void Renderer::bloom_pass() {
    if (!m_pass_settings.bloom_enabled) {
        float black[4] = {0.0, 0.0, 0.0, 0.0};
        glClearTexImage(m_buffers.bloom_blur->id, 0, GL_RGBA, GL_FLOAT, &black);
        return;
    }

    auto& shader = m_shaders.bloom_extract_shader;
    auto& bloom_brightness = m_buffers.bloom_brightness;
    auto& bloom_blur = m_buffers.bloom_blur;
    shader->bind();
    shader->bind();
    shader->uniform("input_color", m_buffers.lighting->color_textures[0], 0);
    shader->uniform("threshold_low", m_bloom_settings.threshold_low);
    shader->uniform("threshold_high", m_bloom_settings.threshold_high);
    shader->uniform("contribution_min", m_bloom_settings.contribution_min);
    shader->uniform("contribution_max", m_bloom_settings.contribution_max);
    bloom_brightness->bind_image(0, GL_WRITE_ONLY, GL_RGBA16F);
    shader->dispatch_compute(bloom_brightness->w, bloom_brightness->h, 1, GL_ALL_BARRIER_BITS);
    bloom_brightness->unbind_image(0);
    shader->unbind();

    glGenerateTextureMipmap(bloom_brightness->id);

    shader = m_shaders.bloom_blur_shader;
    shader->bind();
    shader->uniform("input_tex", bloom_brightness, 0);
    bloom_blur->bind_image(0, GL_WRITE_ONLY, GL_RGBA16F);
    shader->dispatch_compute(bloom_blur->w, bloom_blur->h, 1, GL_ALL_BARRIER_BITS);
    bloom_blur->unbind_image(0);
    shader->unbind();
}

void Renderer::final_pass() {
    if (!m_pass_settings.final_enabled) {
        return;
    }

    glEnable(GL_FRAMEBUFFER_SRGB);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto& shader = m_shaders.final;

    shader->bind();

    shader->uniform("lighting.depth", m_buffers.lighting->depth_texture, 0);
    shader->uniform("lighting.color", m_buffers.lighting->color_textures[0], 1);
    shader->uniform("bloom_tex", m_buffers.bloom_blur, 2);
    shader->uniform("bloom_intensity", m_bloom_settings.intensity);

    util::quad_with_depth();

    shader->unbind();

    glDisable(GL_FRAMEBUFFER_SRGB);
}

void Renderer::overlay_pass() {
    if (!m_colored_instances.empty()) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        auto& colored_shader = m_shaders.colored_preview;
        colored_shader->bind();
        util::setup_camera(colored_shader, current_camera());
        colored_shader->uniform("view", current_camera()->view);
        colored_shader->uniform("proj", current_camera()->proj);

        for (const auto& inst : m_colored_instances) {
            colored_shader->uniform("model", inst.transform);
            colored_shader->uniform("objectColor", inst.color);
            colored_shader->uniform("alpha", inst.alpha);
            inst.model->draw(colored_shader, inst.transform);
        }

        colored_shader->unbind();
        glDisable(GL_BLEND);
    }
}

void Renderer::draw_debug_gui() {
    ImGui::Text("Passes");
    ImGui::Checkbox("Geometry enabled", &m_pass_settings.geometry_enabled);
    ImGui::Checkbox("Shadows enabled", &m_pass_settings.shadows_enabled);
    ImGui::Checkbox("Ssao enabled", &m_pass_settings.ssao_enabled);
    ImGui::Checkbox("Lighting enabled", &m_pass_settings.lighting_enabled);
    ImGui::Checkbox("Bloom enabled", &m_pass_settings.bloom_enabled);
    ImGui::Checkbox("Final enabled", &m_pass_settings.final_enabled);
    ImGui::Text("SSAO");
    ImGui::SliderFloat("Radius", &m_ssao_settings.radius, 0.01f, 2.0f);
    ImGui::SliderInt("Samples", &m_ssao_settings.sample_count, 4, 128);
    ImGui::SliderInt("Blur Radius", &m_ssao_settings.blur_radius, 1, 5);
    ImGui::SliderFloat("Bias", &m_ssao_settings.bias, 0.0f, 0.1f);
    ImGui::SliderFloat("Intensity", &m_ssao_settings.intensity, 0.0f, 2.0f);
    ImGui::Separator();
    ImGui::Text("Bloom");
    ImGui::SliderFloat("Threshold Low", &m_bloom_settings.threshold_low, 0.0f, 5.0f);
    ImGui::SliderFloat("Threshold High", &m_bloom_settings.threshold_high, 0.0f, 10.0f);
    ImGui::SliderFloat("Contribution Min", &m_bloom_settings.contribution_min, 0.0f, 2.0f);
    ImGui::SliderFloat("Contribution Max", &m_bloom_settings.contribution_max, 0.0f, 2.0f);
    ImGui::SliderFloat("Intensity", &m_bloom_settings.intensity, 0.0f, 2.0f);
    ImGui::Separator();
}