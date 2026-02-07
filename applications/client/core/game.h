#pragma once

#include <memory>

#include <cppgl.h>

/// @brief Game singleton to manage game state and game instance
/// @tparam GameClass The internal game class which instances is being managed
/// @tparam GameStates A enum containing all the possible game states
/// @note The GameClass must define a typename StartState referencing the GameStateBase implementation of the starting
/// state.
template <class GameClass, typename GameStates> class GameSingleton {
public:
    /// @brief Base class for GameStates.
    /// @note Each GameStateBase implementation must have a public static field type which holds a valid enum value of
    /// GameStates that is associated with this implementation.
    class GameStateBase {
    public:
        virtual ~GameStateBase() = default;

        /// @brief Called on enter of the state. State changes always happen at the beginning of a frame. This is where
        /// the state should be made game ready.
        /// @param game A convenience reference to the game instance
        /// @param old_state_type The previous game state type
        virtual void enter(GameClass& game, GameStates old_state_type) = 0;

        /// @brief This is the main loop for each state. It is called once per frame and should be responsible for e.g.
        /// updating, drawing, handling input, ...
        /// @param game A convenience reference to the game instance
        /// @param dt Elapsed frame time in seconds
        virtual void loop(GameClass& game, float dt) = 0;

        /// @brief Called on exit of the state. State changes always happen at the beginning of a frame. This is where
        /// the state can be cleaned up. In most cases this can be left empty, since the state instance will be
        /// destroyed shortly after.
        /// @param game A convenience reference to the game instance
        /// @param new_state_type The next game state type
        virtual void exit(GameClass& game, GameStates new_state_type) = 0;

        /// @brief Optional overridable keyboard callback, the default does nothing.
        /// @param game A convenience reference to the game instance
        /// @param ... GLFW keyboard callback parameters
        virtual void keyboard(GameClass& game, int key, int scancode, int action, int mods) = 0;
    };

    /// @brief Initializes the window context and creates the GameClass instance by calling it's default constructor.
    /// Then it switches to the StartState provided by GameClass. Should only be called once.
    /// @param params The context parameters for window creation
    static void init(const cppgl::ContextParameters& params) {
        cppgl::Context::init(params);
        cppgl::Context::set_keyboard_callback(keyboard_callback);

        std::filesystem::current_path(EXECUTABLE_DIR);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);

        auto& game = instance();

        game.m_new_state.type = GameClass::StartState::type;
        game.m_state.type = GameClass::StartState::type;
        game.m_state.state = std::make_unique<typename GameClass::StartState>();
        game.m_state.state->enter(game.m_game, GameClass::StartState::type);
    }

    /// @brief Run the main game loop
    static void run() {
        auto& game = instance();

        while (cppgl::Context::running()) {
            if (game.m_new_state.type != game.m_state.type) {
                game.m_state.state->exit(game.m_game, game.m_new_state.type);
                game.m_state.state = std::move(game.m_new_state.state);
                GameStates old_state_type = game.m_state.type;
                game.m_state.type = game.m_new_state.type;
                game.m_state.state->enter(game.m_game, old_state_type);
            }

            game.m_state.state->loop(game.m_game, cppgl::Context::frame_time() / 1000.0);

            cppgl::Context::swap_buffers();
        }
    }

    /// @brief Access the active game instance (GameClass).
    /// @return A reference to the active game instance
    static GameClass& game() { return instance().m_game; }

    /// @brief Schedule a state switch for the next frame.
    /// @tparam State The GameStateBase implementation of the next state
    template <class State> static void switch_state() {
        auto& game = instance();
        game.m_new_state.state = std::make_unique<State>();
        game.m_new_state.type = State::type;
    }

private:
    static GameSingleton& instance() {
        static GameSingleton instance;
        return instance;
    }

    static void keyboard_callback(int key, int scancode, int action, int mods) {
        instance().m_state.state->keyboard(game(), key, scancode, action, mods);
    };

    struct GameStateInstance {
        std::unique_ptr<GameStateBase> state;
        GameStates type;
    };

    GameStateInstance m_state;
    GameStateInstance m_new_state;

    GameClass m_game;
};
