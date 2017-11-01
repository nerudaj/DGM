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
			 *  \param [in] APP_WIDTH Parameter_Description
			 *  \param [in] APP_HEIGHT Parameter_Description
			 *  \param [in] APP_NAME Parameter_Description
			 *  \param [in] Default Parameter_Description
			 *  \param [in] APP_FPS Parameter_Description
			 */
			void init(const int APP_WIDTH, const int APP_HEIGHT, const std::string &APP_NAME, const int APP_STYLE = sf::Style::Default, const int APP_FPS = 40);

			void deinit();

			App();
			~App();
		};

		class AppState {
		protected:
			App *game;

		public:
			virtual void draw() = 0;

			virtual void update() = 0;

			virtual void input() = 0;
		};
	};
};