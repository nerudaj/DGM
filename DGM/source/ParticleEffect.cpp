#include <DGM\dgm.hpp>

const float PIOVER180 = 0.01745329f;

void dgm::ParticleEffect::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = texture;
	target.draw(vertices.data(), used * 4, sf::Quads, states);
}

sf::Vector2f dgm::ParticleEffect::toCartesian(const float angle, const float distance) const {
	return sf::Vector2f(sin(angle * PIOVER180) * distance, -cos(angle * PIOVER180) * distance);
}

sf::Int32 dgm::ParticleEffect::getRandomNumber(const sf::Int32 base, const sf::Int32 delta) const {
	sf::Int32 d = 0;

	if (delta != 0) {
		d = rand() % (delta * 2) - delta;
	}

	return base + d;
}

float dgm::ParticleEffect::getRandomNumber(const float base, const float delta) const {
	float d = 0.f;

	if (delta != 0.f) {
		int frac = int(delta * 100.f);

		d = float(rand() % (frac * 2) - frac) / 100.f;
	}

	return base + d;
}

void dgm::ParticleEffect::setVerticesPosition(const std::size_t index, const sf::Vector2f & position) {
	vertices[index * 4].position = position - particleSize / 2.f;
	vertices[index * 4 + 1].position = sf::Vector2f(position.x + particleSize.x / 2.f, position.y - particleSize.y / 2.f);
	vertices[index * 4 + 2].position = position + particleSize / 2.f;
	vertices[index * 4 + 3].position = sf::Vector2f(position.x - particleSize.x / 2.f, position.y + particleSize.y / 2.f);
}

void dgm::ParticleEffect::spawn(const std::size_t index) {
	particles[index].lifespan = sf::milliseconds(getRandomNumber(particleLifespan.asMilliseconds(), particleLifespanDelta.asMilliseconds()));
	float direction, distance;

	setVerticesPosition(index, sf::Vector2f(0.f, 0.f));
	direction = getRandomNumber(0.f, emitRange) + emitAngle;
	distance = getRandomNumber(particleVelocity, particleVelocityDelta);

	particles[index].forward = toCartesian(direction, distance);
}

void dgm::ParticleEffect::update(const sf::Time & elapsed) {
	if (used < vertices.size() / 4) {
		int generateCount = particlesPerSecond * elapsed.asMilliseconds() / 1000;

		for (int i = 0; i < generateCount and used < vertices.size() / 4; i++) {
			spawn(used);
			used++;
		}
	}

	for (std::size_t i = 0; i < used; i++) {
		particles[i].lifespan -= elapsed;

		if (particles[i].lifespan <= sf::Time::Zero) {
			spawn(i);
		}

		vertices[i * 4].position += particles[i].forward + enviroForce;
		vertices[i * 4 + 1].position += particles[i].forward + enviroForce;
		vertices[i * 4 + 2].position += particles[i].forward + enviroForce;
		vertices[i * 4 + 3].position += particles[i].forward + enviroForce;
	}
}

void dgm::ParticleEffect::setEmitterRange(const float range) {
	emitRange = range;
}

void dgm::ParticleEffect::setParticleLifespan(const sf::Time & lifespan, const sf::Time & deviation) {
	particleLifespan = lifespan;
	particleLifespanDelta = deviation;
}

void dgm::ParticleEffect::setParticleVelocity(const float velocity, const float deviation) {
	particleVelocity = velocity;
	particleVelocityDelta = deviation;
}

void dgm::ParticleEffect::setEnvironmentForce(const sf::Vector2f & force) {
	enviroForce = force;
}

void dgm::ParticleEffect::setEmitterRotation(const float angle) {
	emitAngle = angle;
}

void dgm::ParticleEffect::rotateEmitter(const float diff) {
	emitAngle += diff;

	if (emitAngle > 360.f) {
		emitAngle -= 360.f;
	}
	else if (emitAngle < 0.f) {
		emitAngle += 360.f;
	}
}

void dgm::ParticleEffect::setTexture(const sf::Texture * texture) {
	ParticleEffect::texture = texture;
}

void dgm::ParticleEffect::setParticlesPerSecond(const int pps) {
	particlesPerSecond = pps;
}

bool dgm::ParticleEffect::init(const int size, const dgm::Clip & clip) {
	particles.resize(size);
	//vertices.setPrimitiveType(sf::Quads);
	vertices.resize(4 * size);

	std::size_t r;
	for (int i = 0; i < size * 4; i += 4) {
		r = rand() % clip.getSize();
		const sf::IntRect &rect = clip.getFrame(r);

		vertices[i].texCoords = sf::Vector2f(float(rect.left), float(rect.top));
		vertices[i + 1].texCoords = sf::Vector2f(float(rect.left + rect.width), float(rect.top));
		vertices[i + 2].texCoords = sf::Vector2f(float(rect.left + rect.width), float(rect.top + rect.height));
		vertices[i + 3].texCoords = sf::Vector2f(float(rect.left), float(rect.top + rect.height));
	}

	particleSize.x = float(clip.getTileSize().x);
	particleSize.y = float(clip.getTileSize().y);

	return true;
}

void dgm::ParticleEffect::reset() {
	used = 0;
}
