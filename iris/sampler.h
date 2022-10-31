#ifndef _IRIS_SAMPLER_
#define _IRIS_SAMPLER_

#include <cstdint>
#include <optional>

#include "iris/float.h"
#include "iris/random.h"

namespace iris {

class Sampler {
 public:
  Sampler(Random& rng) noexcept : rng_(rng), samples_(2) {}
  ~Sampler() { rng_.DiscardGeometric(samples_); }

  size_t NextIndex(size_t max);
  geometric_t Next();

 private:
  Random& rng_;
  uint_fast8_t samples_;
  std::optional<geometric_t> next_;
};

}  // namespace iris

#endif  // _IRIS_SAMPLER_