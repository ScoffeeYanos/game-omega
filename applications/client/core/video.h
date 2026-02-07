#pragma once

#include <cppgl.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

// FFmpeg includes
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}
class Video {
public:
    Video();

    /// @brief Initialize video player with a video file
    /// @param filepath Path to the video file (mp4, avi, etc.)
    /// @return true if initialization successful, false otherwise
    bool init(const std::string& filepath);

    /// @brief Clean up video resources
    void cleanup();

    /// @brief Play the video
    void play();

    /// @brief Update video playback state (advances frame)
    /// @param dt Delta time in seconds since last update
    void update(float dt);

    /// @brief Render the current video frame to the specified position
    void render() const;

    /// @brief Check if video has finished
    /// @return true if playback has reached the end
    bool is_finished() const { return m_is_finished; }

private:
    // Video metadata
    std::string m_filepath;
    int m_width = 0;
    int m_height = 0;
    float m_fps = 30.0f;
    float m_duration = 0.0f;
    int m_frame_count = 0;

    // Playback state
    float m_current_time = 0.0f;
    int m_current_frame = 0;
    bool m_is_playing = false;
    bool m_is_finished = false;
    int m_last_decoded_frame = -1;

    // OpenGL resources
    static cppgl::Texture2D m_frame_texture;

    // FFmpeg context
    AVFormatContext* m_format_context;
    AVCodecContext* m_codec_context;
    int m_video_stream_index;
    AVFrame* m_frame;
    AVFrame* m_frame_rgb;
    SwsContext* m_sws_context;
    std::vector<uint8_t> m_frame_buffer;

    // Helper functions
    void calculate_frame_index();
    void update_frame_texture();
    bool open_video_file(const std::string& filepath);
    void close_video_file();
    bool decode_next_frame();
    bool seek_to_frame(int frame_index);
};