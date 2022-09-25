#include "iris/random.h"

namespace iris {

void Random::DiscardIndex(size_t num_to_discard) {
  for (size_t i = 0; i < num_to_discard; i++) {
    NextIndex(128);
  }
}

void Random::DiscardGeometric(size_t num_to_discard) {
  for (size_t i = 0; i < num_to_discard; i++) {
    NextGeometric();
  }
}

void Random::DiscardVisual(size_t num_to_discard) {
  for (size_t i = 0; i < num_to_discard; i++) {
    NextVisual();
  }
}

}  // namespace iris