#include "iris/random/mersenne_twister_random.h"

#include <cassert>
#include <limits>

namespace iris {
namespace random {

size_t MersenneTwisterRandom::NextIndex(size_t size) {
  assert(size != 0 && size <= std::numeric_limits<uint32_t>::max());
  std::uniform_int_distribution<uint32_t> distribution(0, size - 1u);
  return distribution(rng_);
}

geometric MersenneTwisterRandom::NextGeometric() {
  return std::generate_canonical<geometric,
                                 std::numeric_limits<geometric>::digits>(rng_);
}

visual MersenneTwisterRandom::NextVisual() {
  return std::generate_canonical<visual, std::numeric_limits<visual>::digits>(
      rng_);
}

std::unique_ptr<Random> MersenneTwisterRandom::Replicate() {
  return std::make_unique<MersenneTwisterRandom>(rng_());
}

}  // namespace random
}  // namespace iris