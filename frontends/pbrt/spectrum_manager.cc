#include "frontends/pbrt/spectrum_manager.h"

#include <utility>

namespace iris::pbrt_frontend {

ReferenceCounted<Spectrum> SpectrumManager::AllocateSpectrum(
    const Color& color) {
  auto rgb = color.ConvertTo(Color::LINEAR_SRGB);
  if (rgb.r == 0.0 && rgb.g == 0.0 && rgb.b == 0.0) {
    return ReferenceCounted<Spectrum>();
  }

  std::array<visual, 3> key = {rgb.r, rgb.g, rgb.g};
  auto iter = color_spectra_.find(key);
  if (iter != color_spectra_.end()) {
    return iter->second;
  }

  auto spectrum = std::as_const(*this).AllocateSpectrum(rgb);
  color_spectra_[key] = spectrum;

  return spectrum;
}

ReferenceCounted<Reflector> SpectrumManager::AllocateReflector(
    const Color& color) {
  auto rgb = color.ConvertTo(Color::LINEAR_SRGB);
  if (rgb.r == 0.0 && rgb.g == 0.0 && rgb.b == 0.0) {
    return ReferenceCounted<Reflector>();
  }

  std::array<visual, 3> key = {rgb.r, rgb.g, rgb.g};
  auto iter = color_reflectors_.find(key);
  if (iter != color_reflectors_.end()) {
    return iter->second;
  }

  auto reflector = std::as_const(*this).AllocateReflector(rgb);
  color_reflectors_[key] = reflector;

  return reflector;
}

}  // namespace iris::pbrt_frontend