#include "AppStateMenuOptions.hpp"

void AppStateMenuOptions::createOptionLabel(tgui::Layout x, tgui::Layout y, const std::string& text) {
	tgui::Label::Ptr label = tgui::Label::create(text);
	label->setAutoSize(true);
	label->setSize("30%", "5%");
	label->setPosition(x, y);
	label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Left);
	label->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	gui.add(label);
}

void AppStateMenuOptions::buildLayout() {
	tgui::Label::Ptr title = tgui::Label::create("Options");
	title->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	title->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	title->setTextSize(72);
	title->setSize("50%", "25%");
	title->setPosition("25%", "5%");
	gui.add(title);

	tgui::CheckBox::Ptr fullscreenCheckbox = tgui::CheckBox::create();
	fullscreenCheckbox->setSize("5%", "5%");
	fullscreenCheckbox->setPosition("60%", "35%");
	fullscreenCheckbox->setChecked(app->window.isFullscreen());
	gui.add(fullscreenCheckbox, "CheckboxFullscreen");
	fullscreenCheckbox->connect("checked", [this]() {
		app->window.toggleFullscreen();
	});
	fullscreenCheckbox->connect("unchecked", [this]() {
		app->window.toggleFullscreen();
	});
	createOptionLabel("30%", "35%", "Toggle fullscreen");

	tgui::Slider::Ptr sliderSoundVolume = tgui::Slider::create(0.f, 100.f);
	sliderSoundVolume->setSize("15%", "3%");
	sliderSoundVolume->setPosition("60%", "43%");
	sliderSoundVolume->setStep(1.f);
	sliderSoundVolume->setValue(settings.soundVolume);
	sliderSoundVolume->connect("ValueChanged", [this]() {
		settings.soundVolume = gui.get<tgui::Slider>("SliderSoundVolume")->getValue();
	});
	gui.add(sliderSoundVolume, "SliderSoundVolume");
	createOptionLabel("30%", "42%", "Sound volume");

	tgui::Slider::Ptr sliderMusicVolume = tgui::Slider::create(0.f, 100.f);
	sliderMusicVolume->setSize("15%", "3%");
	sliderMusicVolume->setPosition("60%", "49%");
	sliderMusicVolume->setStep(1.f);
	sliderMusicVolume->setValue(settings.musicVolume);
	sliderMusicVolume->connect("ValueChanged", [this]() {
		settings.musicVolume = gui.get<tgui::Slider>("SliderMusicVolume")->getValue();
	});
	gui.add(sliderMusicVolume, "SliderMusicVolume");
	createOptionLabel("30%", "48%", "Music volume");

	tgui::Button::Ptr backButton = tgui::Button::create("Back");
	backButton->setSize("15%", "5%");
	backButton->setPosition("84%", "94%");
	backButton->connect("pressed", [this]() { app->popState(); });
	gui.add(backButton, "ButtonExit");
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
