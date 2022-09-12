#ifndef _IRIS_RANDOM_MERSENNE_TWISTER_RANDOM_
#define _IRIS_RANDOM_MERSENNE_TWISTER_RANDOM_

#include <memory>
#include <random>

#include "iris/random.h"

namespace iris {
namespace random {

class MersenneTwisterRandom : public Random {
 public:
  MersenneTwisterRandom(
      Random::result_type default_seed = std::mt19937::default_seed)
      : rng_(default_seed) {}

  result_type min() const override;
  result_type max() const override;
  result_type operator()() override;
  void discard(unsigned long long z) override;

  std::unique_ptr<Random> Replicate() override;

 private:
  std::mt19937 rng_;
};

}  // namespace random
}  // namespace iris

#endif  // _IRIS_RANDOM_MERSENNE_TWISTER_RANDOM_