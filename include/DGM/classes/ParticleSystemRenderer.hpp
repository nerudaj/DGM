#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	namespace ps {		
		class ParticleSystemRenderer : public sf::Drawable, public sf::Transformable {
		private:
			virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
			
		protected:
			sf::VertexArray vertices;
			sf::Texture *texture;
			dgm::Clip clip;
		
		public:
			sf::Vertex *getParticleVertices(const std::size_t index);
					
			bool init(const std::size_t particleCount, const dgm::Clip &clip);
			
			void setTexture(sf::Texture *texture);

			const dgm::Clip &getClip() const { return clip; }
		};
	};
};