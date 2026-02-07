#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <soloud.h>
#include <soloud_bus.h>
#include <soloud_wav.h>
#include <soloud_wavstream.h>

#include "platform.h"

/// @brief A wrapper class around a SoLoud::Wav object holding a reference to the bus it should use for playback.
template <typename TSound> class SoundInternal {
public:
    SoundInternal() = default;
    SoundInternal(const std::string& file, SoLoud::Bus* bus) : m_file(file), m_bus(bus) {}

    SoundInternal& operator=(SoundInternal&& other) {
        assert(m_file.empty() && "trying to override a sound");
        m_sound.load(other.m_file.c_str());
        m_bus = other.m_bus;
        m_file = std::move(other.m_file);
        return *this;
    }

    /// @brief Play the loaded sound on it's corresponding bus using it's default volume;
    SoLoud::handle play() { return m_bus->play(m_sound); }

    SoLoud::handle play3d(const glm::vec3& pos) { return m_bus->play3d(m_sound, pos.x, pos.y, pos.z); }

    bool loaded() const { return !m_file.empty(); }

    TSound& sound() { return m_sound; }

private:
    std::string m_file;
    TSound m_sound;
    SoLoud::Bus* m_bus;
};

using Sound = SoundInternal<SoLoud::Wav>;
using Stream = SoundInternal<SoLoud::WavStream>;

/// @brief A wrapper class around Soloud to make bus handling and sound loading easier.
class SoundManager {
public:
    SoundManager() : m_engine(Platform::sound_engine()) {};

    /// @brief Creates a bus to which sounds can be assigned and their volumes controlled.
    /// @param name The name of the bus (must be unique)
    /// @param volume The default volume the bus starts at
    void create_bus(const std::string& name, float volume = 1.0) {
        auto& bus = m_busses[name];
        bus.bus.setVolume(volume);
        bus.volume = volume;
        bus.handle = m_engine.play(bus.bus);
    };

    /// @brief Set the volume of one bus
    /// @param name The name of the bus
    void set_volume(const std::string& name, float volume) {
        auto& bus = m_busses.at(name);
        bus.volume = volume;
        m_engine.fadeVolume(bus.handle, bus.volume, 0.1);
    }

    /// @brief Get the current bus volume
    /// @param name The name of the bus
    float get_volume(const std::string& name) { return m_busses.at(name).volume; }

    /// @brief Load a sound from a file and associated it with a bus.
    /// @param file The filename of the sound
    /// @param bus The unique name of the bus
    /// @return A sound instance that can be used for playback
    template <typename TSound = Sound> TSound load_sound(const std::string& file, const std::string& bus) {
        return {file, &m_busses.at(bus).bus};
    }

private:
    struct Bus {
        SoLoud::Bus bus;
        float volume{1.0};
        SoLoud::handle handle;
    };

    SoLoud::Soloud& m_engine;
    std::unordered_map<std::string, Bus> m_busses;
};