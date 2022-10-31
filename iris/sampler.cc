#include "iris/sampler.h"

#include <cmath>

#include "absl/log/check.h"

namespace iris {

size_t Sampler::NextIndex(size_t max) {
  auto scaled_sample = Next() * static_cast<geometric_t>(max);
  geometric_t floor = std::floor(scaled_sample);
  next_ = scaled_sample - floor;
  return floor;
}

geometric_t Sampler::Next() {
  if (next_) {
    auto value = *next_;
    next_.reset();
    return value;
  }

  CHECK(samples_-- != 0);
  return rng_.NextGeometric();
}

}  // namespace iris