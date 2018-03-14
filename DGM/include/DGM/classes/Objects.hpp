/**
 *  \file 			Objects.hpp
 *  \author			doomista
 *  \description	Declarations of Basic dmg logic objects
 */

#pragma once

namespace dgm {
	/**
	 *  \brief Abstract class with interface
	 *  common to all objects
	 */
	class Object {
	public:
		virtual const sf::Vector2f &GetPosition() const =0;
		
		virtual void SetPosition(const float x, const float y) =0;
		
		virtual void SetPosition(const sf::Vector2f &position) =0;
		
		virtual void Move(const float x, const float y) =0;
		
		virtual void Move(const sf::Vector2f &position) =0;
	};
	
	/**
	 *  \brief Logic class for circles
	 *  
	 *  \details This class is basis for most of
	 *  your characters and collidable objects in
	 *  top-down games. Circle is defined by position
	 *  of its center and radius.
	 */
	class Circle : public Object {
	protected:
		sf::Vector2f position; ///< Position of the center
		float radius; ///< Radius of circle
		
	public:
		/**
		 *  \brief Returns position of circle
		 */
		const sf::Vector2f &GetPosition() const;
		
		/**
		 *  \brief Returns radius of circle
		 */
		const float GetRadius() const;
		
		/**
		 *  \brief Sets position of circle by setting new XY coordinates
		 */
		void SetPosition(const float x, const float y);
		
		/**
		 *  \brief Sets new position of circle with SFML vector
		 */
		void SetPosition(const sf::Vector2f &position);
		
		/**
		 *  \brief Sets radius
		 */
		void SetRadius(const float radius);
		
		/**
		 *  \brief Moves object
		 */
		void Move(const float x, const float y);
		
		/**
		 *  \brief Moves object
		 */
		void Move(const sf::Vector2f &forward);
		
		Circle();
		Circle(const float x, const float y);
		Circle(const sf::Vector2f &position);
		~Circle();
	};
	
	/**
	 *  \brief Logic class for rectangle
	 *  
	 *  \details Rectangle is defined by position
	 *  of its top left corner and its dimesions.
	 */
	class Rect : public Object {
	protected:
		sf::Vector2f position; ///< Position of the topleft corner
		sf::Vector2f size; ///< Dimensions of the rectangle
		
	public:
		/**
		 *  \brief Returns position of topleft vertex of rectangle
		 */
		const sf::Vector2f &GetPosition() const;
		
		/**
		 *  \brief Returns dimensions of rectangle
		 */
		const sf::Vector2f &GetSize() const;
		
		/**
		 *  \brief Set position of top-left corner
		 */
		void SetPosition(const float x, const float y);
		
		/**
		 *  \brief Set position of top-left corner
		 */
		void SetPosition(const sf::Vector2f &position);
		
		/**
		 *  \brief Moves object
		 */
		void Move(const float x, const float y);
		
		/**
		 *  \brief Moves object
		 */
		void Move(const sf::Vector2f &forward);
		
		/**
		 *  \brief Set dimensions of rectangle
		 */
		void SetSize(const float width, const float height);
		
		/**
		 *  \brief Set dimensions of rectangle
		 */
		void SetSize(const sf::Vector2f &size);
		
		Rect();
		Rect(const float x, const float y, const float width, const float height);
		Rect(const sf::Vector2f &position, const sf::Vector2f &size);
		~Rect();
	};
	
	/**
	 *  \brief Logic class for representing level geometry or per-pixel collision hitboxes
	 *  
	 *  Mesh is basically an integer array where in DGM logic each cell with value
	 *  <= 0 is considered to be 'air' and any cell with value > 0 is impassable.
	 *  If this object is send to DGM collision functions, it will be compared
	 *  to the other object based on set voxelSize and dataSize. If your mesh
	 *  represents a whole level, then voxelSize is size of a single block (64x64px for example).
	 *  If mesh represents a per-pixel hitbox, then voxelSize should be 1 (each cell is single pixel).
	 *  Access cell at [x, y] by y * dataSize.x + x.
	 */
	class Mesh : public Object {
	private:
		/**
		 *  \brief Frees memory held by *data if necessary
		 */
		void Free();
		
		/**
		 *  \brief Allocates memory to *data based on dataSize
		 */
		bool Alloc();
		
	protected:
		int *data;	///< Array for holding collision data
		sf::Vector2f position;	///< Position of top-left corner
		sf::Vector2i dataSize; ///< dataSize.x * dataSize.y is size of data array
		sf::Vector2i voxelSize; ///< How big rectangle does single cell of data represents
		
	public:
		/**
		 *  \brief Read and Write access to *data
		 *  
		 *  Access cell at [x, y] by y * dataSize.x + x.
		 */
		int &operator[] (std::size_t index);
		
		/**
		 *  \brief Read-only access to *data
		 *  
		 *  Access cell at [x, y] by y * dataSize.x + x.
		 */
		const int &operator[] (std::size_t index) const;
	
		/**
		 *  \brief Get position of top-left corner
		 */
		const sf::Vector2f &GetPosition() const;
		
		/**
		 *  \brief Get dimensions of *data array
		 */
		const sf::Vector2i &GetDataSize() const;
		
		/**
		 *  \brief Get dimensions of single voxel
		 */
		const sf::Vector2i &GetVoxelSize() const;
	
		/**
		 *  \brief Set position of top-left corner
		 */
		void SetPosition(const float x, const float y);
		
		/**
		 *  \brief Set position of top-left corner
		 */
		void SetPosition(const sf::Vector2f &position);
		
		/**
		 *  \brief Set dimensions of single voxel
		 */
		void SetVoxelSize(const int width, const int height);
		
		/**
		 *  \brief Set dimensions of single voxel
		 */
		void SetVoxelSize(const sf::Vector2i &size);
		
		/**
		 *  \brief Set dimensions of data array
		 *  
		 *  \details This will allocate memory for *data.
		 *  Any data stored here prior to this call will be lost.
		 *  
		 *  \returns TRUE on success, FALSE on malloc failure
		 */
		bool SetDataSize(const int width, const int height);
		
		/**
		 *  \brief Set dimensions of data array
		 *  
		 *  \details This will allocate memory for *data.
		 *  Any data stored here prior to this call will be lost.
		 *  
		 *  \returns TRUE on success, FALSE on malloc failure
		 */
		bool SetDataSize(const sf::Vector2i &size);
		
		/**
		 *  \brief Moves object
		 */
		void Move(const float x, const float y);
		
		/**
		 *  \brief Moves object
		 */
		void Move(const sf::Vector2f &forward);
		
		Mesh();
		~Mesh();
	};
}