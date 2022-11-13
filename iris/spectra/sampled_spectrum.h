#ifndef _IRIS_SPECTRA_SAMPLED_SPECTRUM_
#define _IRIS_SPECTRA_SAMPLED_SPECTRUM_

#include <map>
#include <vector>

#include "iris/float.h"
#include "iris/spectrum.h"

namespace iris {
namespace spectra {

class SampledSpectrum final : public Spectrum {
 public:
  SampledSpectrum(const std::map<visual, visual>& samples);

  visual_t Intensity(visual_t wavelength) const override;

 private:
  std::vector<visual> wavelengths_;
  std::vector<visual> intensitites_;
};

}  // namespace spectra
}  // namespace iris

#endif  // _IRIS_SPECTRA_SAMPLED_SPECTRUM_