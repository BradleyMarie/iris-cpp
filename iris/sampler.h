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

  Sampler(Sampler&& from)
      : rng_(from.rng_), samples_(from.samples_), next_(from.next_) {
    from.samples_ = 0;
    from.next_.reset();
  }

  Sampler(const Sampler&) = delete;
  Sampler& operator=(const Sampler&) = delete;
  Sampler& operator=(Sampler&& from) = delete;

  ~Sampler() {
    if (samples_ != 0) {
      rng_.DiscardGeometric(samples_);
    }
  }

  size_t NextIndex(size_t max);
  geometric_t Next();

 private:
  Random& rng_;
  uint_fast8_t samples_;
  std::optional<geometric_t> next_;
};

}  // namespace iris

#endif  // _IRIS_SAMPLER_