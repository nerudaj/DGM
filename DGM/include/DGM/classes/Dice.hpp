#pragma once

namespace dgm {
	class Dice {
	public:
		/**
		 *  \brief Roll n dices with s sides
		 *  
		 *  \param [in] sides Number of sides on each dice
		 *  \param [in] dices Number of dices
		 *  \return Sum of thrown values
		 */
		static int roll(int sides, int dies = 0);
	};
}