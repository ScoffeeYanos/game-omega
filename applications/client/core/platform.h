#pragma once

#include <soloud.h>

class Platform {
public:
    static Platform& instance() {
        static Platform platform;
        return platform;
    }

    static SoLoud::Soloud& sound_engine() { return instance().m_engine; }

private:
    Platform() { m_engine.init(); };

    SoLoud::Soloud m_engine;
};