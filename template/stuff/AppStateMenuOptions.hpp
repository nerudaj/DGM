#pragma once

#include <DGM/dgm.hpp>

class AppStateMenuOptions : public dgm::AppState {
public:
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool init() override;
};