#pragma once

#include <DGM/dgm.hpp>

class AppStateBootstrap : public dgm::AppState {
private:
	dgm::ResourceManager resmgr;
	std::string rootDir;

	enum class State {
		Enter, Exit
	};

	State state = State::Enter;

public:
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool init() override;

	AppStateBootstrap(const std::string root) : rootDir(root) {}
};