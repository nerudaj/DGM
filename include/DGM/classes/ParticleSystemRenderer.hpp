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
			/**
			 *  \brief Get array of four vertices for given particle
			 *  
			 *  \param[in] index  Index of particle
			 *  \return Pointer to array of four vertices
			 */
			sf::Vertex *getParticleVertices(const std::size_t index);
			
			/**
			 *  \brief Initialize the object
			 *  
			 *  \param[in]  particleCount  How many particles will renderer hold
			 *  \param[in]  clip           Clipping information for particles
			 *  \return TRUE on success
			 *  
			 *  Clipping information are only relevant if you plan on binding texture
			 *  to the object. If you only need white squares, you can pass in empty 
			 *  clip object.
			 */
			bool init(const std::size_t particleCount, const dgm::Clip &clip);
			
			/**
			 *  \brief Bind texture to the object
			 *  
			 *  \param[in]  texture  Texture that will be used for particle rendering
			 *  
			 *  Particle system does not interface directly with bound texture, it only
			 *  has access to clipping data.
			 */
			void setTexture(sf::Texture &texture);

			/**
			 *  \brief Clear texture previously bound to this object
			 */
			void unbindTexture() {
				texture = nullptr;
			}

			const dgm::Clip &getClip() const { return clip; }

			ParticleSystemRenderer() {
				unbindTexture();
			}
		};
	};
};