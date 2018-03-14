/**
 *  \file Objects.cpp
 *  \author doomista
 */

#include <DGM\dgm.hpp>

// **************
// *** CIRCLE ***
// **************
const sf::Vector2f &dgm::Circle::GetPosition() const { return position; }

const float dgm::Circle::GetRadius() const { return radius; }
		
void dgm::Circle::SetPosition(const float x, const float y) {
	position.x = x;
	position.y = y;
}

void dgm::Circle::SetPosition(const sf::Vector2f &position) { Circle::position = position; }

void dgm::Circle::SetRadius(const float radius) { Circle::radius = radius; }

void dgm::Circle::Move(const float x, const float y) {
	position.x += x;
	position.y += y;
}

void dgm::Circle::Move(const sf::Vector2f &forward) {
	position.x += forward.x;
	position.y += forward.y;
}

dgm::Circle::Circle() {
	SetPosition(0.f, 0.f);
	radius = 0.f;
}

dgm::Circle::Circle(const float x, const float y) { SetPosition(x, y); }
dgm::Circle::Circle(const sf::Vector2f &position) { SetPosition(position); }
dgm::Circle::~Circle() {}

// ************
// *** RECT ***
// ************
const sf::Vector2f &dgm::Rect::GetPosition() const { return position; }

const sf::Vector2f &dgm::Rect::GetSize() const { return size; }

void dgm::Rect::SetPosition(const float x, const float y) {
	position.x = x;
	position.y = y;
}

void dgm::Rect::SetPosition(const sf::Vector2f &position) { Rect::position = position; }

void dgm::Rect::Move(const float x, const float y) {
	position.x += x;
	position.y += y;
}

void dgm::Rect::Move(const sf::Vector2f &forward) {
	position.x += forward.x;
	position.y += forward.y;
}

void dgm::Rect::SetSize(const float width, const float height) {
	size.x = width;
	size.y = height;
}

void dgm::Rect::SetSize(const sf::Vector2f &size) { Rect::size = size; }

dgm::Rect::Rect() {
	SetPosition(0.f, 0.f);
	SetSize(0.f, 0.f);
}

dgm::Rect::Rect(const float x, const float y, const float width, const float height) {
	SetPosition(x, y);
	SetSize(width, height);
}

dgm::Rect::Rect(const sf::Vector2f &position, const sf::Vector2f &size) {
	SetPosition(position);
	SetSize(size);
}

dgm::Rect::~Rect() {
	
}

// ************
// *** MESH ***
// ************
void dgm::Mesh::Free() {
	if (data != NULL) {
		delete[] data;
		data = NULL;
	}
}

bool dgm::Mesh::Alloc() {
	data = new int [dataSize.x * dataSize.y];
	if (data == NULL)
		return false;
	return true;
}

int &dgm::Mesh::operator[] (std::size_t index) { return data[index]; }

const int &dgm::Mesh::operator[] (std::size_t index) const { return data[index]; }

const sf::Vector2f &dgm::Mesh::GetPosition() const { return position; }

void dgm::Mesh::SetPosition(const float x, const float y) {
	position.x = x;
	position.y = y;
}

const sf::Vector2i &dgm::Mesh::GetDataSize() const { return dataSize; }

const sf::Vector2i &dgm::Mesh::GetVoxelSize() const { return voxelSize; }

void dgm::Mesh::SetPosition(const sf::Vector2f &position) { Mesh::position = position; }

void dgm::Mesh::SetVoxelSize(const int width, const int height) {
	voxelSize.x = width;
	voxelSize.y = height;
}

void dgm::Mesh::SetVoxelSize(const sf::Vector2i &size) { voxelSize = size; }

bool dgm::Mesh::SetDataSize(const int width, const int height) { return SetDataSize(sf::Vector2i(width, height)); }

bool dgm::Mesh::SetDataSize(const sf::Vector2i &size) {
	Free(); // Frees *data if necessary
	dataSize = size;
	return Alloc();// Allocates new memory
}

void dgm::Mesh::Move(const float x, const float y) {
	position.x += x;
	position.y += y;
}

void dgm::Mesh::Move(const sf::Vector2f &forward) {
	position.x += forward.x;
	position.y += forward.y;
}

dgm::Mesh::Mesh() {
	data = NULL;
	position = sf::Vector2f(0.f, 0.f);
	dataSize = sf::Vector2i(0, 0);
	voxelSize= sf::Vector2i(0, 0);
}

dgm::Mesh::~Mesh() {
	Free();
}