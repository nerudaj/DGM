#pragma once

namespace dgm {
    namespace Molded {
        /**
         *  \brief Simple class for making objects that can have flags assigned to them
         */
        class Flaggable {
        private:
            unsigned int flags;
            
        public:
            /**
             *  \brief Enable/disable flag
             *  
             *  \param [in] flag Flag value
             *  \param [in] enabled Whether flag will be enabled
             */
            void setFlag(const unsigned int flag, const bool enabled);
            
            /**
             *  \brief Test whether flag is set
             *  
             *  \param [in] flag Flag value
             *  \return TRUE if was set
             */
            unsigned int getFlag(const unsigned int flag) const { return flags & flag; }
            
            /**
             *  \brief Unsets all flags
             */
            void dropAllFlags();
            
            Flaggable();
		};
    };
};