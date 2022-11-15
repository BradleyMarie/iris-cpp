#ifndef _FRONTENDS_PBRT_SPECTRUM_MANAGER_
#define _FRONTENDS_PBRT_SPECTRUM_MANAGER_

#include <map>

#include "iris/color.h"
#include "iris/reference_counted.h"
#include "iris/reflector.h"
#include "iris/spectrum.h"

namespace iris::pbrt_frontend {

class SpectrumManager {
 public:
  virtual ReferenceCounted<Spectrum> AllocateSpectrum(
      const std::map<visual, visual>& wavelengths) const = 0;
  ReferenceCounted<Spectrum> AllocateSpectrum(const Color& color);

  virtual ReferenceCounted<Reflector> AllocateReflector(
      const std::map<visual, visual>& wavelengths) const = 0;
  ReferenceCounted<Reflector> AllocateReflector(const Color& color);

 protected:
  virtual ReferenceCounted<Spectrum> AllocateSpectrum(
      const Color& color) const = 0;
  virtual ReferenceCounted<Reflector> AllocateReflector(
      const Color& color) const = 0;

 private:
  std::map<std::array<visual, 3>, ReferenceCounted<Spectrum>> color_spectra_;
  std::map<std::array<visual, 3>, ReferenceCounted<Reflector>>
      color_reflectors_;
};

}  // namespace iris::pbrt_frontend

#endif  // _FRONTENDS_PBRT_SPECTRUM_MANAGER_