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
  RussianRoulette(visual maximum_continue_probability,
                  visual always_continue_threshold) noexcept;

  std::optional<visual_t> Evaluate(Random& rng,
                                   visual_t desired_continue_probability) const;

 private:
  visual maximum_continue_probability_;
  visual always_continue_probability_;
};

}  // namespace internal
}  // namespace integrators
}  // namespace iris

#endif  // _IRIS_INTEGRATORS_INTERNAL_RUSSIAN_ROULETTE_