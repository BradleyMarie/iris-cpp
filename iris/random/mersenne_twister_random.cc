#include "iris/random/mersenne_twister_random.h"

namespace iris {
namespace random {

Random::result_type MersenneTwisterRandom::operator()() { return rng_(); }

void MersenneTwisterRandom::discard(unsigned long long z) {
  return rng_.discard(z);
}

std::unique_ptr<Random> MersenneTwisterRandom::Replicate() {
  return std::make_unique<MersenneTwisterRandom>(rng_());
}

}  // namespace random
}  // namespace iris