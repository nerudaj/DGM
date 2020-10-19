#include "AppStateMenuOptions.hpp"

static const std::vector<std::string> STRING_RESOLUTIONS = {
	"640x480", "800x600", "1024x768", "1280x720", "1366x768", "1600x900", "1920x1080"
};

static const std::vector<sf::Vector2u> NUM_RESOLUTIONS = {
	{640, 480}, {800, 600}, {1024, 768}, {1280, 720}, {1366, 768}, {1600, 900}, {1920, 1080}
};

std::string getWindowResolutionAsString(const dgm::Window& window) {
	auto size = window.getSize();
	return std::to_string(size.x) + "x" + std::to_string(size.y);
}

void AppStateMenuOptions::buildLayout() {
	auto title = createOptionLabel({ "0%", "5%" }, { "100%", "25%" }, "Options");
	title->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	title->setTextSize(72);
	gui.add(title);

	tgui::Layout2d rowSize = { "40%", "5%" };

	createCheckbox("Toggle fullscreen", { "30%", "35%" }, rowSize,
		app->window.isFullscreen(), [this](bool checked) { app->window.toggleFullscreen(); }, "CheckboxFullscreen"
	);
	createSlider("Sound volume", { "30%", "40%" }, rowSize,
		settings.soundVolume, [this]() {
			settings.soundVolume = gui.get<tgui::Slider>("SliderSoundVolume")->getValue();
		}, "SliderSoundVolume"
	);
	createSlider("Music volume", { "30%", "45%" }, rowSize,
		settings.musicVolume, [this]() {
			settings.musicVolume = gui.get<tgui::Slider>("SliderMusicVolume")->getValue();
		}, "SliderMusicVolume"
	);
	createDropdown("Change resolution", { "30%", "50%" }, rowSize,
		STRING_RESOLUTIONS, getWindowResolutionAsString(app->window),
		[this]() {
			auto item = gui.get<tgui::ComboBox>("ComboResolution");
			auto index = item->getSelectedItemIndex();
			if (index == -1) return;
			bool fs = app->window.isFullscreen();

			// Restart window with new resolution
			app->window.changeResolution(NUM_RESOLUTIONS[index]);

			// Force gui to update viewport and resolution
			gui.setView(app->window.getWindowContext().getView());
		}, "ComboResolution"
	);

	createButton("Back", { "84%", "94%" }, { "15%", "5%" }, [this]() { app->popState(); });
}

void AppStateMenuOptions::input() {
	sf::Event event;
	while (app->window.pollEvent(event)) {
		gui.handleEvent(event);
	}
}

void AppStateMenuOptions::update() {}

void AppStateMenuOptions::draw() {
	app->window.beginDraw(sf::Color::White);

	gui.draw();

	app->window.endDraw();
}

bool AppStateMenuOptions::init() {
	gui.setTarget(app->window.getWindowContext());

	buildLayout();

	return true;
}
