#ifndef _IRIS_SPECTRA_UNIFORM_SPECTRUM_
#define _IRIS_SPECTRA_UNIFORM_SPECTRUM_

#include "iris/float.h"
#include "iris/reflector.h"

namespace iris {
namespace reflectors {

class UniformReflector final : public Reflector {
 public:
  UniformReflector(visual reflectance);

  visual_t Reflectance(visual_t wavelength) const override;
  visual_t Albedo() const override;

 private:
  visual_t reflectance_;
};

}  // namespace reflectors
}  // namespace iris

#endif  // _IRIS_SPECTRA_UNIFORM_SPECTRUM_