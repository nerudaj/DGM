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

tgui::Label::Ptr AppStateMenuOptions::createOptionLabel(tgui::Layout2d position, tgui::Layout2d size, const std::string& text) {
	tgui::Label::Ptr label = tgui::Label::create(text);
	label->setAutoSize(true);
	label->setSize(size);
	label->setPosition(position);
	label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Left);
	label->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	return label;
}

void AppStateMenuOptions::createCheckbox(const std::string& label, tgui::Layout2d position, tgui::Layout2d size, bool checked, std::function<void(bool)> onChange, const std::string& name) {
	auto labelPtr = createOptionLabel({ "0%", "0%" }, { "50%", "100%" }, label);

	tgui::CheckBox::Ptr checkbox = tgui::CheckBox::create();
	checkbox->setSize("10%", "100%");
	checkbox->setPosition("50%", "0%");
	checkbox->setChecked(checked);
	checkbox->connect("Changed", onChange);
	
	tgui::Panel::Ptr panel = tgui::Panel::create(size);
	panel->setPosition(position);
	panel->add(checkbox, name);
	panel->add(labelPtr);
	gui.add(panel);
}

void AppStateMenuOptions::createSlider(const std::string& label, tgui::Layout2d position, tgui::Layout2d size, float value, std::function<void(void)> onChange, const std::string& name, float lo, float hi, float step) {
	auto labelPtr = createOptionLabel({ "0%", "0%" }, { "50%", "100%" }, label);

	tgui::Slider::Ptr slider = tgui::Slider::create(lo, hi);
	slider->setSize("50%", "60%");
	slider->setPosition("50%", "20%");
	slider->setStep(step);
	slider->setValue(value);
	slider->connect("ValueChanged", onChange);

	tgui::Panel::Ptr panel = tgui::Panel::create(size);
	panel->setPosition(position);
	panel->add(slider, name);
	panel->add(labelPtr);
	gui.add(panel);
}

void AppStateMenuOptions::createDropdown(const std::string& label, tgui::Layout2d position, tgui::Layout2d size, const std::vector<std::string>& items, const std::string& selected, std::function<void(void)> onSelect, const std::string& name) {
	auto labelPtr = createOptionLabel({ "0%", "0%" }, { "50%", "100%" }, label);

	// Dropdown for selecting resolution
	tgui::ComboBox::Ptr dropdown = tgui::ComboBox::create();
	dropdown->setSize("50%", "100%");
	dropdown->setPosition("50%", "0%");
	for (auto& item : items) {
		dropdown->addItem(item, item);
	}
	dropdown->setSelectedItem(selected);
	dropdown->connect("ItemSelected", onSelect);

	tgui::Panel::Ptr panel = tgui::Panel::create(size);
	panel->setPosition(position);
	panel->add(dropdown, name);
	panel->add(labelPtr);
	gui.add(panel);
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

	tgui::Button::Ptr backButton = tgui::Button::create("Back");
	backButton->setSize("15%", "5%");
	backButton->setPosition("84%", "94%");
	backButton->connect("pressed", [this]() { app->popState(); });
	gui.add(backButton, "ButtonExit");

	gui.saveWidgetsToFile("pokus.txt");
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
	sf::Font &font = resmgr.get<sf::Font>("cruft.ttf");

	gui.setFont(font);
	gui.setTarget(app->window.getWindowContext());

	buildLayout();

	return true;
}
