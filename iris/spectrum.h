#ifndef _IRIS_SPECTRUM_
#define _IRIS_SPECTRUM_

#include "iris/float.h"
#include "iris/reference_countable.h"

namespace iris {

class Spectrum : public ReferenceCountable {
 public:
  virtual visual_t Intensity(visual_t wavelength) const = 0;
  virtual ~Spectrum() {}
};

}  // namespace iris

#endif  // _IRIS_SPECTRUM_