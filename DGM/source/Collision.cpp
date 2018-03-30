/**
 *  \brief Collision.cpp
 *  \author doomista
 */
 
#include <DGM\dgm.hpp>

bool dgm::Collision::basic(const dgm::Rect &rect, const sf::Vector2i &point) {
	sf::Vector2f pos = rect.getPosition();
	sf::Vector2f sze = rect.getSize();

	if (pos.x <= point.x && point.x <= (pos.x + sze.x) && pos.y <= point.y && point.y <= (pos.y + sze.y)) {
		return true;
	}
	
	return false;
}
		
bool dgm::Collision::basic(const dgm::Circle &circle, const sf::Vector2i &point) {
	sf::Vector2f cpos = circle.getPosition();
	
	float dX = abs(point.x - cpos.x);
	float dY = abs(point.y - cpos.y);
	
	if ((dX * dX + dY * dY) < (circle.getRadius() * circle.getRadius())) {
		return true;
	}
	
	return false;
}

bool dgm::Collision::basic(const dgm::Circle &A, const dgm::Circle &B) {
	sf::Vector2f cpos1 = A.getPosition();
	sf::Vector2f cpos2 = B.getPosition();
	
	float dX = abs(cpos1.x - cpos2.x);
	float dY = abs(cpos1.y - cpos2.y);

	if ((dX * dX + dY * dY) < ((A.getRadius() + B.getRadius()) * (A.getRadius() + B.getRadius()))) {
		return true;
	}

	return false;
}

bool dgm::Collision::basic(const dgm::Rect &A, const dgm::Circle &B) {
	sf::Vector2f apos = A.getPosition();
	sf::Vector2f size = A.getSize();
	sf::Vector2f bpos = B.getPosition();
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

	if (sqrt(cX * cX + cY * cY) < B.getRadius()) {
		return true;
	}
	
return false;
}

bool dgm::Collision::basic(const dgm::Rect &A, const dgm::Rect &B) {
	sf::Vector2f cA = A.getPosition() + A.getSize() / 2.f;
	sf::Vector2f cB = B.getPosition() + B.getSize() / 2.f;
	
	float dX = abs(cA.x - cB.x) * 2.f;
	float dY = abs(cA.y - cB.y) * 2.f;
	
	if (dX <= (A.getSize().x + B.getSize().x) && dY <= (A.getSize().y + B.getSize().y)) {
		return true;
	}
	
	return false;
}

void normalizeBoundaries(sf::IntRect &dst, const sf::IntRect *src, const dgm::Mesh &mesh) {
	sf::Vector2i meshSize = mesh.getDataSize();
	sf::Vector2i tileSize = mesh.getVoxelSize();
	
	dst.left	= src->left / tileSize.x;
	dst.top		= src->top  / tileSize.y;
	dst.width	= (src->left + src->width)  / tileSize.x;
	dst.height	= (src->top  + src->height) / tileSize.y;
	
	if (dst.left < 0)				dst.left	= 0;
	if (dst.width  >= meshSize.x)	dst.width	= meshSize.x - 1;
	if (dst.top  < 0) 				dst.top		= 0;
	if (dst.height >= meshSize.y)	dst.height	= meshSize.y - 1;
}

bool dgm::Collision::basic(const dgm::Mesh &A, const dgm::Circle &B, int *meshHitPosition) {
	sf::IntRect outBody;
	sf::IntRect bounds;
	sf::Vector2i tileSize = A.getVoxelSize();
	sf::Vector2i meshSize = A.getDataSize();
	sf::Vector2f meshPos = A.getPosition();
	
	dgm::Conversion::circleToIntRect(B, outBody);
	normalizeBoundaries(bounds, &outBody, A);

	dgm::Rect box;
	box.setSize(float(tileSize.x), float(tileSize.y));
	
	for (int y = bounds.top; y <= bounds.height; y++) {
		for (int x = bounds.left; x <= bounds.width; x++) {
			if (A[y * meshSize.x + x] > 0) {
				box.setPosition(float(x * tileSize.x) + meshPos.x, float(y * tileSize.y) + meshPos.y);
				
				if (dgm::Collision::basic(box, B)) {
					if (meshHitPosition != nullptr) *meshHitPosition = y * meshSize.x + x;
					return true;
				}
			}
		}
	}
	
	return false;
}

bool dgm::Collision::basic(const dgm::Mesh &A, const dgm::Rect &B, int *meshHitPosition) {
	sf::IntRect outBody(sf::FloatRect(B.getPosition() - A.getPosition(), B.getSize()));
	sf::IntRect bounds;
	sf::Vector2i meshSize = A.getDataSize();

	normalizeBoundaries(bounds, &outBody, A);

	for (int y = bounds.top; y <= bounds.height; y++) {
		for (int x = bounds.left; x <= bounds.width; x++) {
			if (A[y * meshSize.x + x] > 0) {
				if (meshHitPosition != nullptr) *meshHitPosition = y * meshSize.x + x;
				return true;
			}
		}
	}

	return false;
}

bool dgm::Collision::advanced(const dgm::Mesh &mesh, const dgm::Circle &body, sf::Vector2f &forward) {
	dgm::Circle aux = body;
	aux.move(forward);
	
	if (!dgm::Collision::basic(mesh, aux)) {
		return false;
	}
	
	aux = body;
	aux.move(forward.x, 0.f);
	if (!dgm::Collision::basic(mesh, aux)) {
		forward.y = 0.f;
		return true;
	}
	
	aux = body;
	aux.move(0.f, forward.y);
	if (!dgm::Collision::basic(mesh, aux)) {
		forward.x = 0.f;
		return true;
	}
	
	forward = sf::Vector2f (0.f, 0.f);
	return true;
}

bool dgm::Collision::advanced(const dgm::Mesh &mesh, const dgm::Rect &body, sf::Vector2f &forward) {
	dgm::Rect aux = body;
	aux.move(forward);
	
	if (!dgm::Collision::basic(mesh, aux)) {
		return false;
	}
	
	aux = body;
	aux.move(forward.x, 0.f);
	if (!dgm::Collision::basic(mesh, aux)) {
		forward.y = 0.f;
		return true;
	}
	
	aux = body;
	aux.move(0.f, forward.y);
	if (!dgm::Collision::basic(mesh, aux)) {
		forward.x = 0.f;
		return true;
	}
	
	forward = sf::Vector2f (0.f, 0.f);
	return true;
}