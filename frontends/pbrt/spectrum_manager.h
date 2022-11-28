#ifndef _FRONTENDS_PBRT_SPECTRUM_MANAGER_
#define _FRONTENDS_PBRT_SPECTRUM_MANAGER_

#include <map>

#include "frontends/pbrt/color.h"
#include "iris/reference_counted.h"
#include "iris/reflector.h"
#include "iris/spectrum.h"

namespace iris::pbrt_frontend {

class SpectrumManager {
 public:
  virtual ReferenceCounted<Spectrum> AllocateSpectrum(const Color& color) = 0;

  virtual ReferenceCounted<Spectrum> AllocateSpectrum(
      const std::map<visual, visual>& wavelengths) = 0;

  virtual ReferenceCounted<Reflector> AllocateReflector(const Color& color) = 0;

  virtual ReferenceCounted<Reflector> AllocateReflector(
      const std::map<visual, visual>& wavelengths) = 0;

  virtual void Clear() = 0;
};

}  // namespace iris::pbrt_frontend

#endif  // _FRONTENDS_PBRT_SPECTRUM_MANAGER_