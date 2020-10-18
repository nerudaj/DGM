#include "AppStateShell.hpp"
#include "../Globals.hpp"

std::string to_string(const dgm::RingBuffer<std::string, 10U>& buf) {
	std::string result = "";

	for (unsigned i = 0; i < buf.size(); i++) {
		result += std::to_string(i) + ": " + buf[i] + "\n";
	}

	return result;
}

void AppStateShell::buildLayout() {
	auto chatbox = tgui::ChatBox::create();
	chatbox->setSize("100%", "95%");
	chatbox->setTextSize(18);
	chatbox->setPosition("0%", "0%");
	chatbox->setLinesStartFromTop();
	gui.add(chatbox, "ChatBox");

	auto editBox = tgui::EditBox::create();
	editBox->setSize("100%", "5%");
	editBox->setTextSize(18);
	editBox->setPosition("0%", "95%");
	editBox->setDefaultText("Enter command...");
	editBox->connect("ReturnKeyPressed", [this]() {
		auto chatbox = gui.get<tgui::ChatBox>("ChatBox");
		auto editbox = gui.get<tgui::EditBox>("EditBox");
		chatbox->addLine("$> " + editbox->getText(), sf::Color(96, 96, 96));

		try {
			std::string result = shell.execute(editbox->getText());
			chatbox->addLine(result, sf::Color::Black);
		}
		catch (dgm::ShellException & e) {
			chatbox->addLine(std::string(e.what()) + "\n", sf::Color::Red);
		}

		historyPtr = shell.getHistory().size();
		editbox->setText("");

	});
	gui.add(editBox, "EditBox");
	editBox->setFocused(true);
}

void AppStateShell::input() {
	sf::Event event;
	while (app->window.pollEvent(event)) {
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Escape) {
				shell.execute("var export Globals.hpp");
				app->popState();
			}
			else if (event.key.code == sf::Keyboard::Up) {
				if (historyPtr < 1) continue;
				historyPtr--;

				auto editbox = gui.get<tgui::EditBox>("EditBox");
				editbox->setText(shell.getHistory()[historyPtr]);
			}
			else if (event.key.code == sf::Keyboard::Down) {
				if (historyPtr >= shell.getHistory().size() - 1) continue;
				historyPtr++;

				auto editbox = gui.get<tgui::EditBox>("EditBox");
				editbox->setText(shell.getHistory()[historyPtr]);
			}
		}

		gui.handleEvent(event);
	}
}

void AppStateShell::update() {}

void AppStateShell::draw() {
	app->window.beginDraw(sf::Color::White);

	gui.draw();

	app->window.endDraw();
}

#define ShellBindVar(var) shellVars.bindVariable(#var, &var)

bool AppStateShell::init() {
	gui.setFont(resmgr.get<sf::Font>("cruft.ttf"));
	gui.setTarget(app->window.getWindowContext());
	gui.setTabKeyUsageEnabled(true);

	buildLayout();

	shell.install(shellVars);
	ShellBindVar(settings.soundVolume);
	ShellBindVar(settings.musicVolume);

	// For educational purposes, remove in production
	shell.install(shellTutorial);

	return true;
}

std::string ShellModuleTutorial::getDescription() const {
	// NOTE: This text will be displayed when you type 'help' and overview of installed modules pops up
	return "How to create shell modules. Type 'tutorial help'.";
}

ShellModuleTutorial::ShellModuleTutorial() : ShellModule("tutorial") {
	// Note that we're calling ctor of ShellModule with string that will identify this module in the shell
	// In this ctor we also add all available actions of this module
	// Each added action is identified by it's name (similar to module itself)
	// Each action can have some number of parameters, description for help text and callback to execute
	// Callback accepts array of string arguments. You will always get the same number of arguments
	// that is specified in the argc parameter.
	// Callback is supposed to throw ShellException on error and return any stdout in a form of a string.
	addAction("lesson1", { 0, "lesson1: First lesson of the tutorial. Type 'tutorial lesson1'", [this](const Arguments& args) {
		return "To create new module, you first have to subclass ShellModule.";
	} });

	addAction("lesson2", { 0, "lesson2: How to write ctor", [this](const Arguments &args) {
		return "In ctor, you need to call ShellModule(<name>) where <name> will identify your "
			"module in the shell (e.g.: tutorial)";
	} });

	addAction("lesson3", { 0, "lesson3: What to do in ctor", [this](const Arguments& args) {
		return "In ctor, you also have to call addAction and add all actions your module should "
			"have as an interface with the user.";
	} });

	addAction("lesson4", { 0, "lesson4: getDescription()", [this](const Arguments& args) {
		return "You also need to override getDescription method and provide description that "
			"will be displayed as a part of help text";
	} });
}
