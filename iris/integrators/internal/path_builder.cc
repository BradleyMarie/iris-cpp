#include "iris/integrators/internal/path_builder.h"

#include <cassert>
#include <cmath>

namespace iris {
namespace integrators {
namespace internal {

PathBuilder::PathBuilder(std::vector<const Reflector*>& reflectors,
                         std::vector<const Spectrum*>& spectra,
                         std::vector<visual_t>& attenuations) noexcept
    : reflectors_(reflectors), spectra_(spectra), attenuations_(attenuations) {
  reflectors_.clear();
  spectra_.clear();
  spectra_.push_back(nullptr);
  attenuations_.clear();
}

void PathBuilder::Add(const Spectrum* spectrum, SpectralAllocator& allocator) {
  spectra_.back() = allocator.Add(spectra_.back(), spectrum);
}

void PathBuilder::Bounce(const Reflector* reflector, visual_t attenuation) {
  assert(reflector);
  assert(std::isfinite(attenuation) && attenuation >= 0.0);

  spectra_.push_back(nullptr);
  reflectors_.push_back(reflector);
  attenuations_.push_back(attenuation);
}

const Spectrum* PathBuilder::Build(SpectralAllocator& allocator) const {
  const Spectrum* sum = spectra_.back();
  for (size_t i = 0; i < reflectors_.size(); i++) {
    size_t index = reflectors_.size() - i - 1;
    sum = allocator.Scale(sum, attenuations_[index]);
    sum = allocator.Reflect(sum, reflectors_[index]);
    sum = allocator.Add(sum, spectra_[index]);
  }
  return sum;
}

}  // namespace internal
}  // namespace integrators
}  // namespace iris