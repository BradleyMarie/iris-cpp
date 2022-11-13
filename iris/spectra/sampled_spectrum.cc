#include "iris/spectra/sampled_spectrum.h"

#include <algorithm>
#include <cassert>
#include <cmath>

namespace iris::spectra {

SampledSpectrum::SampledSpectrum(const std::map<visual, visual>& samples) {
  assert(!samples.empty());

  for (const auto& entry : samples) {
    assert(std::isfinite(entry.first));
    assert(entry.first >= 0.0);
    assert(std::isfinite(entry.second));
    assert(entry.second >= 0.0);

    wavelengths_.push_back(entry.first);
    intensitites_.push_back(std::max(static_cast<visual>(0.0), entry.second));
  }
}

visual_t SampledSpectrum::Intensity(visual_t wavelength) const {
  if (wavelength <= wavelengths_.front()) {
    return intensitites_.front();
  }

  if (wavelength >= wavelengths_.back()) {
    return intensitites_.back();
  }

  auto iter =
      std::lower_bound(wavelengths_.begin(), wavelengths_.end(), wavelength);

  size_t next_index = iter - wavelengths_.begin();
  if (wavelengths_[next_index] == wavelength) {
    return intensitites_[next_index];
  }

  size_t index = next_index - 1;

  visual_t delta = wavelengths_[next_index] - wavelengths_[index];
  visual_t interpolation = (wavelength - wavelengths_[index]) / delta;

  visual_t result =
      intensitites_[index] +
      interpolation * (intensitites_[next_index] - intensitites_[index]);

  return std::max(static_cast<visual_t>(0.0), result);
}

}  // namespace iris::spectra