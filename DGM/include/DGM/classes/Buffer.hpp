/**
 *  \file Buffer.hpp
 *  \author doomista
 */

#pragma once

#include <iostream>

namespace dgm {
	template <typename T>
	/**
	 *  \brief Static size buffer for fast maintenance of object arrays
	 *  
	 *  \details The purpose of this object is to maintain dynamic
	 *  objects like effects, projectiles, particles in easy and
	 *  efficient way. Using buffer, you define an array of object
	 *  of a static size that allows you to dynamically add and
	 *  remove object as you need (and as long as you don't exceed
	 *  defined capacity). All add/remove actions are performed in
	 *  constant time and they suppose that your objects are fully
	 *  initialized and only need to "spawn" in your logic.
	 */
	class Buffer {
	private:
		std::size_t s; ///< Size (number of curretnly maintained objects)
		std::size_t c; ///< Capacity (total number of available objects)
		T **arr;
		
		bool error(std::string errstr) {
			std::cerr << errstr << std::endl;
			return false;
		}
	
		bool allocate(std::size_t size) {
			arr = new T * [size];
			
			if (arr == NULL) return false;
			
			for (std::size_t i = 0; i < size; i++) {
				arr[i] = NULL;
				arr[i] = new T;
				
				if (arr[i] == NULL) {
					for (int p = i - 1; p >= 0; p--) delete arr[p];
					return false;
				}
			}
			
			return true;
		}
		
		void swap (size_t a, size_t b) {
			T *tmp = arr[a];
			arr[a] = arr[b];
			arr[b] = tmp;
		}
	
	public:
		/**
		 *  \brief Returns number of currently maintained objects
		 */
		std::size_t Size () const { return s; }
		
		/**
		 *  \brief Returns how many object can be maintained
		 */
		std::size_t Capacity () const { return c; }
		
		/**
		 *  \brief Adds a new object to maintained list
		 *  
		 *  \return TRUE on success, FALSE otherwise (capacity reached)
		 *  
		 *  \details If size is less than capacity then it is
		 *  incremented and new object is added to maintained list.
		 */
		bool Add () {
			if (s == c) return false;
			s++;
			return true;
		}
		
		/**
		 *  \brief Removes the selected object from maintained list
		 *  
		 *  \return TRUE on success, FALSE otherwise.
		 *  
		 *  \details Details
		 */
		bool Remove(size_t index) {
			if (s == 0) return false;

			s--;
			if (s != index) swap(index, s);

			return true;
		}
		
		/**
		 *  \brief
		 */
		T &operator[] (std::size_t index) {
			return arr[index][0];
		}
		
		/**
		 *  \brief
		 */
		const T &operator[] (std::size_t index) const {
			return arr[index][0];
		}
		
		/**
		 *  \brief get index of first object
		 *  
		 *  \return 0
		 *  
		 *  \details Basically substitute for 0. This
		 *  function tries to emulate c++ iterator syntax
		 *  but instead of returning reference to object
		 *  it only returns index to object.
		 */
		std::size_t Begin() const { return 0; }
		
		/**
		 *  \brief get index of last object + 1
		 *  
		 *  \return size()
		 *  
		 *  \details Basically substitute for size(). This
		 *  function tries to emulate c++ iterator syntax
		 *  but instead of returning reference to object
		 *  it only returns index to object.
		 */
		std::size_t End() const { return s; }
		
		/**
		 *  \brief Retrieve object located at index size()-1
		 *  
		 *  \details This function is particulary useful for
		 *  setting up newly added object.
		 *  
		 *  \return Reference to Buffer[size()-1]
		 */
		T &GetLast() {
			return arr[s-1][0];
		}
		
		/**
		 *  \brief
		 */
		const T &GetLast() const {
			return arr[s-1][0];
		}
		
		/**
		 *  \brief Initializes the buffer with 'size' objects
		 *  
		 *  \details Internal size is set to zero and internal
		 *  capacity is set to 'size'.
		 *  
		 *  \return TRUE on success, FALSE otherwise.
		 */
		bool init (std::size_t size) {
			std::string ErrHeader ("dgm::Buffer::init(...) - ");
			
			if (arr != NULL) return error(ErrHeader + "Memory is already allocated. Free it with deinit() before attemting to reallocate.");
			
			if (not allocate(size)) return error(ErrHeader + "Could not allocate memory.");
			
			c = size;
			s = 0;
			return true;
		}
		
		/**
		 *  \brief Clears the memory from all allocated items
		 *  
		 *  \details Is automatically called from destructor.
		 */
		void deinit() {
			if (arr != NULL) {
				for (unsigned int i = 0; i < c; i++) delete arr[i];

				delete[] arr;
				arr = NULL;
				s = 0;
				c = 0;
			}
		}
		
		/**
		 *  \brief Empties the buffer, but keeping the allocated memory
		 */
		void Clear() {
			s = 0;
		}

		Buffer() {}
		Buffer(unsigned long size) { init(size);  }
		~Buffer() { deinit();  }
	};
};