#include <DGM\dgm.hpp>

using dgm::ps::ParticleSystemInterface;

// *** PARTICLE SYSTEM ***
bool ParticleSystemInterface::init(const std::size_t particleCount, const dgm::Clip &clip, dgm::ps::ParticleFactoryInterface *factory) {
	if (not renderer.init(particleCount, clip)) return false;
	factory->reset();
	
	try {
		particles.resize(particleCount);

		for (unsigned i = 0; i < particles.capacity(); i++) {
			particles[i] = factory->create();
			particles[i]->init(renderer.getParticleVertices(i));
		}
	}
	catch(...) {
		return false;
	}
	
	return true;
}

dgm::ps::ParticleSystemInterface::ParticleSystemInterface() {
}

dgm::ps::ParticleSystemInterface::~ParticleSystemInterface() {
	for (unsigned i = 0; i < particles.capacity(); i++) {
		delete particles[i];
	}
}

