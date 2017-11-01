/* CREDITS:
	Laurent Gomila for making the SFML	(http://sfml-dev.org)
	'Mr Foo' for his circle to rect detection algorithm (http://lazyfoo.net)
*/

#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#ifndef _DEBUG
	#pragma comment(lib, "sfml-main.lib")
	#pragma comment(lib, "sfml-system.lib")
	#pragma comment(lib, "sfml-graphics.lib")
	#pragma comment(lib, "sfml-window.lib")
	#pragma comment(lib, "sfml-audio.lib")
#else
	#pragma comment(lib, "sfml-main-d.lib")
	#pragma comment(lib, "sfml-system-d.lib")
	#pragma comment(lib, "sfml-graphics-d.lib")
	#pragma comment(lib, "sfml-window-d.lib")
	#pragma comment(lib, "sfml-audio-d.lib")
#endif

#include <iostream>	// cout, cerr, endl
#include <fstream>	// ifstream/ofstream
#include <vector>	// Vector
#include <cstring>	// String
#include <utility>	// Pair
#include <ciso646>	// keywords: and, or, and_eq, or_eq, xor, xor_eq, bitand, bitor, not, compl, not_eq
#include <stack>	// Stack

// *** CLASSES ***
#include "classes/Buffer.hpp"
#include "classes/Objects.hpp"
#include "classes/Conversion.hpp"
#include "classes/Dice.hpp"
#include "classes/Counter.hpp"
#include "classes/Countdown.hpp"
#include "classes/TimeCountdown.hpp"
#include "classes/Clip.hpp"
#include "classes/AnimationState.hpp"
#include "classes/AnimationData.hpp"
#include "classes/Animation.hpp"
#include "classes/Controller.hpp"
#include "classes/Strings.hpp"
#include "classes/Collision.hpp"
#include "classes/TileRenderer.hpp"
#include "classes/ParticleEffect.hpp"

// *** MOLDED CLASSES ***
#include "classes/Molded/Flaggable.hpp"
#include "classes/Molded/GameCore.hpp"