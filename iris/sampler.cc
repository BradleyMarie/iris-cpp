#include "iris/sampler.h"

#include "absl/log/check.h"

namespace iris {

geometric_t Sampler::Next() {
  CHECK(samples_-- != 0);
  return rng_.NextGeometric();
}

}  // namespace iris