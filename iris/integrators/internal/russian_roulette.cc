#include "iris/integrators/internal/russian_roulette.h"

#include <cassert>
#include <cmath>

namespace iris {
namespace integrators {
namespace internal {

RussianRoulette::RussianRoulette(visual maximum_success_probability,
                                 visual success_probability_cutoff) noexcept
    : maximum_success_probability_(maximum_success_probability),
      success_probability_cutoff_(success_probability_cutoff) {
  assert(0.0 <= maximum_success_probability <= 1.0);
  assert(0.0 <= success_probability_cutoff <= 1.0);
}

std::optional<visual_t> RussianRoulette::Evaluate(
    Random& rng, visual_t desired_success_probability) const {
  desired_success_probability =
      std::max(static_cast<visual>(0.0),
               std::min(desired_success_probability, static_cast<visual>(1.0)));

  if (success_probability_cutoff_ <= desired_success_probability) {
    rng.DiscardVisual(1);
    return 1.0;
  }

  visual_t termination_cutoff =
      std::min(maximum_success_probability_, desired_success_probability);

  auto roulette_sample = rng.NextVisual();
  if (termination_cutoff < roulette_sample) {
    return std::nullopt;
  }

  return 1.0 / termination_cutoff;
}

}  // namespace internal
}  // namespace integrators
}  // namespace iris