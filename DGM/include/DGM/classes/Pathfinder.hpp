#pragma once

#include <DGM\dgm.hpp>
#include <mutex>

namespace dgm {
	namespace ai {
		class Pathfinder {
		protected:
			struct LookupField {
				enum class Flags { Solid = 1, FastForward = 2 };
				
				int flags;
				int value;
				
				bool isSolid() const { return flags & int(Flags::Solid); }
				bool isFastForward() const { return flags & int(Flags::FastForward); }
				void setSolid(bool yes) {
					int mask = 0xF ^ int(Flags::Solid);
					flags = (flags & mask) | (yes ? int(Flags::Solid) : 0);
				}
				void setFastForward(bool yes) {
					int mask = 0xF ^ int(Flags::FastForward);
					flags = (flags & mask) | (yes ? int(Flags::FastForward) : 0);
				}
				
				LookupField() { flags = 0; value = 0; }
			};
			
			struct Target {
				sf::Vector2i position;
				int distance;
				
				Target() {
					position = sf::Vector2i(0, 0);
					distance = 0;
				}
				Target(const sf::Vector2i &position, const int distance) {
					Target::position = position;
					Target::distance = distance;
				}
				Target(const int positionX, const int positionY, const int distance) {
					position = sf::Vector2i(positionX, positionY);
					Target::distance = distance;
				}
			};
			
			typedef std::vector<LookupField> LookupArray;
			typedef std::vector<Target> TargetArray;
			std::vector<LookupArray> lookupArrays;
			std::vector<TargetArray> targetArrays;
			
			int progress; ///< Progress of current computation
			int activeLookup; ///< Index of currently used LookupArray
			sf::Vector2i lookupSize;
			int syncLookupArray; ///< Whether (and which) lookup array is out of sync with solid fields
			
			void swapLookups();
			void recomputeLookup(); ///< Launched by thread
			int at(int pos) { return lookupArrays[activeLookup][pos].value; }
			
			// Recompute Lookup auxiliary methods
			void clearLookup(LookupArray &lookup);
			void buildLookup(LookupArray &lookup, TargetArray &targets);
			void markFastForwards(LookupArray &lookup);
		
		public:
			/**
			 *  \brief Empty the cache for targets
			 *  
			 *  \details All previous calls to addTarget() will be forgotten.
			 *  If recompute() will be called, no lookup will be build (because
			 *  there is no target). If recomputation is in progress, clearing
			 *  targets will take effect the next time recompute() is called.
			 *  
			 *  \see addTarget()
			 *  \see recompute()
			 *  \see getComputationProgress()
			 */
			void clearTargets() {
				targetArrays[activeLookup].clear();
			}
		
			/**
			 *  \brief Adds a new target for computing path to
			 *  
			 *  \param [in] position Coordinates of target tile
			 *  \param [in] distance Maximum distance from target where lookup should be computed
			 *  
			 *  \details Zero distance (default value) means infinite distance. Lookup
			 *  will be computed for the whole map. Calling this function while computation is
			 *  active will plan this target for the next recompute() call.
			 */
			void addTarget(const sf::Vector2i &position, const int distance = 0) {
				targetArrays[activeLookup].push_back(Target(position, distance));
			}
			
			/**
			 *  \brief Adds a new target for computing path to
			 *  
			 *  \param [in] positionX X coordinate of target tile
			 *  \param [in] positionY Y coordinate of target tile
			 *  \param [in] distance Maximum distance from target where lookup should be computed
			 *  
			 *  \details Zero distance (default value) means infinite distance. Lookup
			 *  will be computed for the whole map.
			 */
			void addTarget(const int positionX, const int positionY, const int distance = 0) {
				targetArrays[activeLookup].push_back(Target(positionX, positionY, distance));
			}
			
			/**
			 *  \brief Recompute the lookup with current set of targets
			 *  
			 *  \returns TRUE if recomputation started succesfully.
			 *  
			 *  \details Recompute launches a second thread where new lookup
			 *  is computed. Until then, the nextStep uses old lookup field.
			 *  When the new lookup is finished, lookups are swapped and
			 *  nextStep will use the new one. You can query recompute progress
			 *  with getComputationProgress(). Recomputation will not start if
			 *  previous one did not end.
			 *  
			 *  \see addTarget()
			 *  \see nextStep()
			 *  \see getComputationProgress()
			 */
			bool recompute();
			
			/**
			 *  \brief Get coordinates of a neighboring tile which is closer to a target
			 *  
			 *  \param [in] position Coordinates of current tile
			 *  \return Coordinates of new tile
			 *  
			 *  \details A (-1, -1) is returned in case a lookup is not defined for
			 *  current tile. In that case, your AI actor should not move (or retain
			 *  its default behaviour). (-1, -1) can only be returned if your actor
			 *  is in location separated from where target is or when you're using
			 *  distance based lookup.
			 */
			sf::Vector2i nextStep(const sf::Vector2i &position);
			
			/**
			 *  \brief Get coordinates of a neighboring tile which is closer to a target
			 *  
			 *  \param [in] positionX X coordinate of current tile
			 *  \param [in] positionY Y coordinate of current tile
			 *  \return Coordinates of new tile
			 *  
			 *  \details A (-1, -1) is returned in case a lookup is not defined for
			 *  current tile. In that case, your AI actor should not move (or retain
			 *  its default behaviour). (-1, -1) can only be returned if your actor
			 *  is in location separated from where target is or when you're using
			 *  distance based lookup.
			 */
			sf::Vector2i nextStep(const int positionX, const int positionY) {
				return nextStep(sf::Vector2i(positionX, positionY));
			}
			
			/**
			 *  \brief Get progress of current recomputation attempt
			 *  
			 *  \return A number from 0-100. 100 means recomputation
			 *  finished.
			 */
			int getComputationProgress() const { return progress; }
			
			/**
			 *  \brief Change state of a tile (blocking/noblocking)
			 *  
			 *  \param [in] position Coordinates of a tile
			 *  \param [in] blocking Whether the tile is solid or walk-through
			 *  
			 *  \details If position is out-of-bounds, assertion will fail.
			 *  Change of the tile will be reflected in the next recompute() call.
			 */
			void changeTileValue(const sf::Vector2i &position, bool blocking);
			
			/**
			 *  \brief Change state of a tile (blocking/noblocking)
			 *  
			 *  \param [in] positionX X coordinate of a tile
			 *  \param [in] positionY Y coordinate of a tile
			 *  \param [in] blocking Whether the tile is solid or walk-through
			 *  
			 *  \details If position is out-of-bounds, assertion will fail.
			 *  Change of the tile will be reflected in the next recompute() call.
			 */
			void changeTileValue(const int positionX, const int positionY, bool blocking);
			
			/**
			 *  \brief Initialize pathfinder from level mesh data
			 *  
			 *  \param [in] mesh Initialized mesh data
			 *  \return TRUE on success.
			 *  
			 *  \details Only call this method if computation is not
			 *  in progress. Calling this method will also wipe all
			 *  currently set targets.
			 *  
			 *  \see getComputationProgress()
			 */
			bool init(const dgm::Mesh &mesh);
			
			Pathfinder();
			~Pathfinder();
		};
	};
};