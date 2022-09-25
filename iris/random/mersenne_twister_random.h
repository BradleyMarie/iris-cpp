#ifndef _IRIS_RANDOM_MERSENNE_TWISTER_RANDOM_
#define _IRIS_RANDOM_MERSENNE_TWISTER_RANDOM_

#include <memory>
#include <random>

#include "iris/float.h"
#include "iris/random.h"

namespace iris {
namespace random {

class MersenneTwisterRandom final : public Random {
 public:
  MersenneTwisterRandom(
      std::mt19937::result_type default_seed = std::mt19937::default_seed)
      : rng_(default_seed) {}

  size_t NextIndex(size_t size) override;
  geometric NextGeometric() override;
  visual NextVisual() override;

  std::unique_ptr<Random> Replicate() override;

 private:
  std::mt19937 rng_;
};

}  // namespace random
}  // namespace iris

#endif  // _IRIS_RANDOM_MERSENNE_TWISTER_RANDOM_