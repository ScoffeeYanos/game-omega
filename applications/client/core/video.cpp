#include "video.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace cppgl;

Texture2D Video::m_frame_texture;

Video::Video() {
    if (!m_frame_texture) {
        m_frame_texture = Texture2D("video_frame", 0, 0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
    }
}

bool Video::init(const std::string& filepath) {
    m_filepath = filepath;
    m_format_context = nullptr;
    m_codec_context = nullptr;
    m_frame = nullptr;
    m_frame_rgb = nullptr;
    m_sws_context = nullptr;
    m_video_stream_index = -1;
    m_last_decoded_frame = -1;

    // Open and parse video file
    if (!open_video_file(filepath)) {
        return false;
    }

    m_is_finished = false;
    m_current_time = 0.0f;
    m_current_frame = 0;

    // Seek to first frame
    seek_to_frame(0);
    decode_next_frame();

    m_frame_texture->resize(m_width, m_height);

    update_frame_texture();

    return true;
}

void Video::cleanup() { close_video_file(); }

void Video::update(float dt) {
    if (!m_is_playing) {
        return;
    }

    m_current_time += dt;

    // Check if we've reached the end
    if (m_current_time >= m_duration) {
        m_current_time = m_duration;
        m_is_finished = true;
        m_is_playing = false;
        return;
    }

    calculate_frame_index();

    // Only decode if we need the next frame
    if (m_current_frame > m_last_decoded_frame) {
        decode_next_frame();
    }

    update_frame_texture();
}

void Video::play() {
    m_is_playing = true;
    m_is_finished = false;
}

void Video::render() const {
    static auto shader = Shader{"video", "core/shader/copytex.vs", "core/shader/video.fs"};

    shader->bind();

    shader->uniform("frame", m_frame_texture, 0);

    glDisable(GL_DEPTH_TEST);
    Quad::draw();
    glEnable(GL_DEPTH_TEST);

    shader->unbind();
}

void Video::calculate_frame_index() {
    if (m_fps > 0.0f) {
        m_current_frame = static_cast<int>(m_current_time * m_fps);
        if (m_current_frame >= m_frame_count) {
            m_current_frame = m_frame_count - 1;
        }
        if (m_current_frame < 0) {
            m_current_frame = 0;
        }
    }
}

void Video::update_frame_texture() {
    if (m_frame_texture && !m_frame_buffer.empty()) {
        glTextureSubImage2D(m_frame_texture->id, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE,
                            m_frame_buffer.data());
    }
}

bool Video::open_video_file(const std::string& filepath) {
    if (avformat_open_input(&m_format_context, filepath.c_str(), nullptr, nullptr) < 0) {
        std::cerr << "Could not open video file: " << filepath << std::endl;
        return false;
    }

    if (avformat_find_stream_info(m_format_context, nullptr) < 0) {
        std::cerr << "Could not find stream info" << std::endl;
        avformat_close_input(&m_format_context);
        return false;
    }

    m_video_stream_index = -1;
    for (unsigned int i = 0; i < m_format_context->nb_streams; i++) {
        if (m_format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_video_stream_index = i;
            break;
        }
    }

    if (m_video_stream_index == -1) {
        std::cerr << "Could not find video stream" << std::endl;
        avformat_close_input(&m_format_context);
        return false;
    }

    AVStream* video_stream = m_format_context->streams[m_video_stream_index];
    const AVCodec* codec = avcodec_find_decoder(video_stream->codecpar->codec_id);

    if (!codec) {
        std::cerr << "Codec not found" << std::endl;
        avformat_close_input(&m_format_context);
        return false;
    }

    m_codec_context = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(m_codec_context, video_stream->codecpar);

    if (avcodec_open2(m_codec_context, codec, nullptr) < 0) {
        std::cerr << "Could not open codec" << std::endl;
        avcodec_free_context(&m_codec_context);
        avformat_close_input(&m_format_context);
        return false;
    }

    m_width = m_codec_context->width;
    m_height = m_codec_context->height;
    m_fps = static_cast<float>(av_q2d(video_stream->r_frame_rate));
    m_duration = static_cast<float>(m_format_context->duration) / AV_TIME_BASE;
    m_frame_count = static_cast<int>(m_duration * m_fps);

    m_frame = av_frame_alloc();
    m_frame_rgb = av_frame_alloc();

    if (!m_frame || !m_frame_rgb) {
        std::cerr << "Could not allocate frames" << std::endl;
        return false;
    }

    int num_bytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, m_width, m_height, 1);
    m_frame_buffer.resize(num_bytes);

    av_image_fill_arrays(m_frame_rgb->data, m_frame_rgb->linesize, m_frame_buffer.data(), AV_PIX_FMT_RGBA, m_width,
                         m_height, 1);

    m_sws_context = sws_getContext(m_width, m_height, m_codec_context->pix_fmt, m_width, m_height, AV_PIX_FMT_RGBA,
                                   SWS_BILINEAR, nullptr, nullptr, nullptr);

    if (!m_sws_context) {
        std::cerr << "Could not create color conversion context" << std::endl;
        return false;
    }

    std::cout << "Video loaded: " << m_width << "x" << m_height << " @ " << m_fps << "fps, " << m_duration << "s"
              << std::endl;

    return true;
}

void Video::close_video_file() {
    if (m_sws_context) {
        sws_freeContext(m_sws_context);
        m_sws_context = nullptr;
    }

    if (m_frame) {
        av_frame_free(&m_frame);
        m_frame = nullptr;
    }

    if (m_frame_rgb) {
        av_frame_free(&m_frame_rgb);
        m_frame_rgb = nullptr;
    }

    if (m_codec_context) {
        avcodec_close(m_codec_context);
        avcodec_free_context(&m_codec_context);
        m_codec_context = nullptr;
    }

    if (m_format_context) {
        avformat_close_input(&m_format_context);
        m_format_context = nullptr;
    }

    m_frame_buffer.clear();
}

bool Video::seek_to_frame(int frame_index) {
    if (!m_format_context || !m_codec_context || frame_index < 0 || frame_index >= m_frame_count) {
        return false;
    }

    double target_seconds = static_cast<double>(frame_index) / (m_fps > 0.0f ? m_fps : 1.0f);
    AVStream* video_stream = m_format_context->streams[m_video_stream_index];

    int64_t global_ts = static_cast<int64_t>(target_seconds * AV_TIME_BASE);
    int64_t target_ts = av_rescale_q(global_ts, AVRational{1, AV_TIME_BASE}, video_stream->time_base);

    if (av_seek_frame(m_format_context, m_video_stream_index, target_ts, AVSEEK_FLAG_BACKWARD) < 0) {
        if (av_seek_frame(m_format_context, -1, global_ts, AVSEEK_FLAG_BACKWARD) < 0) {
            return false;
        }
    }
    avcodec_flush_buffers(m_codec_context);
    return true;
}

bool Video::decode_next_frame() {
    if (!m_format_context || !m_codec_context) {
        return false;
    }

    AVPacket packet;
    while (av_read_frame(m_format_context, &packet) >= 0) {
        if (packet.stream_index != m_video_stream_index) {
            av_packet_unref(&packet);
            continue;
        }

        int response = avcodec_send_packet(m_codec_context, &packet);
        if (response < 0) {
            av_packet_unref(&packet);
            continue;
        }

        int ret = avcodec_receive_frame(m_codec_context, m_frame);
        av_packet_unref(&packet);

        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            continue;
        }
        if (ret < 0) {
            continue;
        }

        // Perform color space conversion
        uint8_t* dst_data[AV_NUM_DATA_POINTERS];
        int dst_linesize[AV_NUM_DATA_POINTERS];
        av_image_fill_arrays(dst_data, dst_linesize, m_frame_buffer.data(), AV_PIX_FMT_RGBA, m_width, m_height, 1);

        sws_scale(m_sws_context, m_frame->data, m_frame->linesize, 0, m_height, dst_data, dst_linesize);

        m_last_decoded_frame++;
        return true;
    }

    return false;
}