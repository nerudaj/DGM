#pragma once

namespace dgm {
	namespace Molded {
		class AppState;

		/**
		 *  \brief Core class of your app
		 *  
		 *  \details Upon creation, push your main game state into the App, then run loop. Other
		 *  states should be pushed dynamically from the app context.
		 */
		class App {
		protected:
			std::stack<AppState*> states; ///< Stack of game states

		public:
			sf::RenderWindow window;

			/**
			 *  \brief Add new AppState to stack
			 *  
			 *  \param [in] state Pointer to new state
			 *  
			 *  \details Old state will be kept in memory and new one will be executed. To remove
			 *  it, use popState()
			 */
			void pushState(AppState *state);

			/**
			 *  \brief Removes AppState from stack
			 *  
			 *  \details Memory held by state is deleted and state is removed from stack. State
			 *  lying under will become new active state.
			 */
			void popState();

			/**
			 *  \brief Look at the top of the state stack
			 *  
			 *  \return Pointer to top state
			 */
			AppState *peekState();

			/**
			 *  \brief Pop old state from stack and push a new one
			 *  
			 *  \param [in] state Pointer to new stack
			 */
			void changeState(AppState *state);
			
			/**
			 *  \brief Main program loop (single threaded)
			 *  
			 *  \details Window events are handled and propagated to AppState method, then state's
			 *  logic and then state's drawing.
			 */
			void loop();

			/**
			 *  \brief Initialize sf::RenderWindow
			 *  
			 *  \param [in] videoMode Display setttings
			 *  \param [in] name Window label
			 *  \param [in] style Window style (sf::Style)
			 *  \param [in] fps Frames per Second. Value <= 0 enables vsync
			 *  \param [in] multithreaded Whether app will use multiple threads that might affect this class
			 *
			 *  \details Positive non-zero fps will set framerate limit on window, other values will cause
			 *  vsync to be turned on. If there will be multiple threads that might affect stack of AppStates,
			 *  turn multithreaded to true. If there is no way that other than a main threads will affect stack
			 *  of states, leave it on false.
			 */
			void init(const sf::VideoMode &videoMode, const std::string &name, const int style = sf::Style::Default, const int fps = 40, const bool multithreaded = false);

			void deinit();

			App();
			~App();
		};

		class AppState {
		protected:
			App *app;

		public:
			void setApp(App *app);

			virtual bool init() = 0;

			virtual void draw() = 0;

			virtual void update() = 0;

			virtual void input() = 0;
		};

		class DefaultState : public AppState {
		public:
			// Inherited via AppState
			virtual bool init() override;

			virtual void draw() override;

			virtual void update() override;

			virtual void input() override;

			DefaultState() {}
			~DefaultState() {}
		};
	};
};