/**
 *  \file Controller.hpp
 *  \author doomista
 */
 
#pragma once

#include <map>
#include <DGM/dgm.hpp>

namespace dgm {
	/**
	 *  \brief Interface for creating controllers
	 */
	class AbstractController {
	public:
		/**
		 *  \brief Test if key associated with code is pressed
		 */
		virtual bool keyPressed(const int code) = 0;
		
		/**
		 *  \brief Mark key as released so subsequent call to keyPressed returns FALSE
		 */
		virtual void releaseKey(const int code) = 0;
	};

	/**
	 *  \brief Class which can detect inputs of keyboard and mouse
	 * 
	 *  This is concrete implementation of AbstractController.
	 *  It allows user to associate keyboard key or mouse button
	 *  with particular code and then test if it is pressed or not.
	 * 
	 *  \note One code can't be associated with multiple keys and/or buttons at the same time
	 */
	class Controller : public AbstractController {
	protected:
		/**
		 *  \brief Helper structure for storing code associations and press state
		 */
		struct Binding {
			bool released;  //< Whether the code was released via releaseKey
			bool isKey;     //< Whether this Binding checks agains key or mouse button
			sf::Keyboard::Key key;
			sf::Mouse::Button btn;
		};

		std::map<int, Binding> bindings;  //< Map of codes to bindings

		/**
		 *  \brief Helper method for registering associations
		 */
		void bindCode(const int code, sf::Keyboard::Key key, sf::Mouse::Button btn);

	public:

		/**
		 * \brief Test whether particular input code is pressed
		 * 
		 * Input code might be mapped to keyboard key XOR mouse button.
		 * If input code was previously released using releaseKey then
		 * physical key/button must first be released, this function called
		 * and only then will this function return TRUE again.
		 */
		bool keyPressed(const int code);

		/**
		 * \brief Marks input as released
		 *
		 * \details Only works in conjuction with simpler version of
		 * keyPressed(). Once an action is marked as released then
		 * keyPressed() will return FALSE until point where user
		 * had released the input physically and then pressed it again.
		 * With this, one can emulate sf::Event::keyPressed behaviour.
		 *
		 * \note In order to function properly, keyPressed should be called
		 * every frame (to ensure that controller will notice the released key)
		 */
		void releaseKey(const int code);

		/**
		 *  Bind numerical input code to keyboard key
		 * 
		 *  \warn There must not be binding of the same input code to both
		 *  keyboard key and mouse button. Exception is thrown in such case.
		 */
		void bindKeyboardKey(const int code, sf::Keyboard::Key key);

		/**
		 *  Bind numerical input code to mouse button
		 *
		 *  \warn There must not be binding of the same input code to both
		 *  keyboard key and mouse button. Exception is thrown in such case.
		 */
		void bindMouseButton(const int code, sf::Mouse::Button btn);

		Controller();
		~Controller();
	};
}