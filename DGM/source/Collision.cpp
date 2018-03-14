/**
 *  \brief Collision.cpp
 *  \author doomista
 */
 
#include <DGM\dgm.hpp>

bool dgm::Collision::Basic(const dgm::Rect &rect, const sf::Vector2i &point) {
	sf::Vector2f pos = rect.GetPosition();
	sf::Vector2f sze = rect.GetSize();

	if (pos.x <= point.x && point.x <= (pos.x + sze.x) && pos.y <= point.y && point.y <= (pos.y + sze.y)) {
		return true;
	}
	
	return false;
}
		
bool dgm::Collision::Basic(const dgm::Circle &circle, const sf::Vector2i &point) {
	sf::Vector2f cpos = circle.GetPosition();
	
	float dX = abs(point.x - cpos.x);
	float dY = abs(point.y - cpos.y);
	
	if ((dX * dX + dY * dY) < (circle.GetRadius() * circle.GetRadius())) {
		return true;
	}
	
	return false;
}

bool dgm::Collision::Basic(const dgm::Circle &A, const dgm::Circle &B) {
	sf::Vector2f cpos1 = A.GetPosition();
	sf::Vector2f cpos2 = B.GetPosition();
	
	float dX = abs(cpos1.x - cpos2.x);
	float dY = abs(cpos1.y - cpos2.y);

	if ((dX * dX + dY * dY) < ((A.GetRadius() + B.GetRadius()) * (A.GetRadius() + B.GetRadius()))) {
		return true;
	}

	return false;
}

bool dgm::Collision::Basic(const dgm::Rect &A, const dgm::Circle &B) {
	sf::Vector2f apos = A.GetPosition();
	sf::Vector2f size = A.GetSize();
	sf::Vector2f bpos = B.GetPosition();
	// Closest coordinates to circle on border of the rect
	// This Black Woman Magic is the courtesy of lazyfoo.net tutorials
	float cX = 0.f, cY = 0.f;
	
	if (bpos.x < apos.x) {
		cX = apos.x;
	}
	else if (bpos.x > (apos.x + size.x)) {
		cX = apos.x + size.x;
	}
	else {
		cX = bpos.x;
	}

	if (bpos.y < apos.y) {
		cY = apos.y;
	}
	else if (bpos.y > (apos.y + size.y)) {
		cY = apos.y + size.y;
	}
	else {
		cY = bpos.y;
	}

cX = bpos.x - cX;
cY = bpos.y - cY;

	if (sqrt(cX * cX + cY * cY) < B.GetRadius()) {
		return true;
	}
	
return false;
}

bool dgm::Collision::Basic(const dgm::Rect &A, const dgm::Rect &B) {
	sf::Vector2f cA = A.GetPosition() + A.GetSize() / 2.f;
	sf::Vector2f cB = B.GetPosition() + B.GetSize() / 2.f;
	
	float dX = abs(cA.x - cB.x) * 2.f;
	float dY = abs(cA.y - cB.y) * 2.f;
	
	if (dX <= (A.GetSize().x + B.GetSize().x) && dY <= (A.GetSize().y + B.GetSize().y)) {
		return true;
	}
	
	return false;
}

void normalizeBoundaries(sf::IntRect &dst, const sf::IntRect *src, const dgm::Mesh &mesh) {
	sf::Vector2i meshSize = mesh.GetDataSize();
	sf::Vector2i tileSize = mesh.GetVoxelSize();
	
	dst.left	= src->left / tileSize.x;
	dst.top		= src->top  / tileSize.y;
	dst.width	= (src->left + src->width)  / tileSize.x;
	dst.height	= (src->top  + src->height) / tileSize.y;
	
	if (dst.left < 0)				dst.left	= 0;
	if (dst.width  >= meshSize.x)	dst.width	= meshSize.x - 1;
	if (dst.top  < 0) 				dst.top		= 0;
	if (dst.height >= meshSize.y)	dst.height	= meshSize.y - 1;
}

bool dgm::Collision::Basic(const dgm::Mesh &A, const dgm::Circle &B) {
	sf::IntRect outBody;
	sf::IntRect bounds;
	sf::Vector2i tileSize = A.GetVoxelSize();
	sf::Vector2i meshSize = A.GetDataSize();
	
	dgm::Conversion::CircleToIntRect(B, outBody);
	normalizeBoundaries(bounds, &outBody, A);

	dgm::Rect box;
	box.SetSize(float(tileSize.x), float(tileSize.y));
	
	for (int y = bounds.top; y <= bounds.height; y++) {
		for (int x = bounds.left; x <= bounds.width; x++) {
			if (A[y * meshSize.x + x] > 0) {
				box.SetPosition(float(x * tileSize.x), float(y * tileSize.y));
				
				if (dgm::Collision::Basic(box, B)) return true;
			}
		}
	}
	
	return false;
}

bool dgm::Collision::Basic(const dgm::Mesh &A, const dgm::Rect &B) {
	sf::IntRect outBody(sf::FloatRect(B.GetPosition(), B.GetSize()));
	sf::IntRect bounds;
	sf::Vector2i meshSize = A.GetDataSize();

	normalizeBoundaries(bounds, &outBody, A);

	for (int y = bounds.top; y <= bounds.height; y++) {
		for (int x = bounds.left; x <= bounds.width; x++) {
			if (A[y * meshSize.x + x] > 0) return true;
		}
	}

	return false;
}

bool dgm::Collision::Advanced(const dgm::Mesh &mesh, const dgm::Circle &body, sf::Vector2f &forward) {
	dgm::Circle aux = body;
	aux.Move(forward);
	
	if (!dgm::Collision::Basic(mesh, aux)) {
		return false;
	}
	
	aux = body;
	aux.Move(forward.x, 0.f);
	if (!dgm::Collision::Basic(mesh, aux)) {
		forward.y = 0.f;
		return true;
	}
	
	aux = body;
	aux.Move(0.f, forward.y);
	if (!dgm::Collision::Basic(mesh, aux)) {
		forward.x = 0.f;
		return true;
	}
	
	forward = sf::Vector2f (0.f, 0.f);
	return true;
}

bool dgm::Collision::Advanced(const dgm::Mesh &mesh, const dgm::Rect &body, sf::Vector2f &forward) {
	dgm::Rect aux = body;
	aux.Move(forward);
	
	if (!dgm::Collision::Basic(mesh, aux)) {
		return false;
	}
	
	aux = body;
	aux.Move(forward.x, 0.f);
	if (!dgm::Collision::Basic(mesh, aux)) {
		forward.y = 0.f;
		return true;
	}
	
	aux = body;
	aux.Move(0.f, forward.y);
	if (!dgm::Collision::Basic(mesh, aux)) {
		forward.x = 0.f;
		return true;
	}
	
	forward = sf::Vector2f (0.f, 0.f);
	return true;
}