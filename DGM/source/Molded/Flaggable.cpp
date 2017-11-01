#include <DGM\dgm.hpp>

using namespace dgm::Molded;

void Flaggable::setFlag(const unsigned int flag, const bool enabled) {
    if (enabled) {
        flags |= flag;
    }
    else if (flags & flag) {
        flags ^= flag;
    }
}

inline void Flaggable::dropAllFlags() {
    flags = 0;
}

Flaggable::Flaggable() {
    flags = 0;
}