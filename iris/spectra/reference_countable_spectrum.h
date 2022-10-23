#ifndef _IRIS_SPECTRA_REFERENCE_COUNTABLE_SPECTRUM_
#define _IRIS_SPECTRA_REFERENCE_COUNTABLE_SPECTRUM_

#include "iris/float.h"
#include "iris/reference_countable.h"
#include "iris/spectrum.h"

namespace iris {
namespace spectra {

class ReferenceCountableSpectrum : public Spectrum, public ReferenceCountable {
 public:
  virtual ~ReferenceCountableSpectrum() = default;
};

}  // namespace spectra
}  // namespace iris

#endif  // _IRIS_SPECTRA_REFERENCE_COUNTABLE_SPECTRUM_