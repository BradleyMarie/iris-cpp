#ifndef _IRIS_SPECTRA_SAMPLED_SPECTRUM_
#define _IRIS_SPECTRA_SAMPLED_SPECTRUM_

#include <map>
#include <vector>

#include "iris/float.h"
#include "iris/reflector.h"

namespace iris {
namespace reflectors {

class SampledReflector final : public Reflector {
 public:
  SampledReflector(const std::map<visual, visual>& samples);

  visual_t Reflectance(visual_t wavelength) const override;
  visual_t Albedo() const override;

 private:
  std::vector<visual> wavelengths_;
  std::vector<visual> intensitites_;
  visual_t albedo_;
};

}  // namespace reflectors
}  // namespace iris

#endif  // _IRIS_SPECTRA_SAMPLED_SPECTRUM_