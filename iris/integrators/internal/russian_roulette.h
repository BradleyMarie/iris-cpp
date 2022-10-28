#ifndef _IRIS_INTEGRATORS_INTERNAL_RUSSIAN_ROULETTE_
#define _IRIS_INTEGRATORS_INTERNAL_RUSSIAN_ROULETTE_

#include <optional>

#include "iris/float.h"
#include "iris/random.h"

namespace iris {
namespace integrators {
namespace internal {

class RussianRoulette {
 public:
  RussianRoulette(visual maximum_success_probability,
                  visual success_probability_cutoff) noexcept;

  std::optional<visual_t> Evaluate(Random& rng,
                                   visual_t desired_success_probability) const;

 private:
  visual maximum_success_probability_;
  visual success_probability_cutoff_;
};

}  // namespace internal
}  // namespace integrators
}  // namespace iris

#endif  // _IRIS_INTEGRATORS_INTERNAL_RUSSIAN_ROULETTE_