#ifndef _IRIS_SPECTRUM_
#define _IRIS_SPECTRUM_

#include "iris/float.h"

namespace iris {

class Spectrum {
 public:
  virtual visual Intensity(visual wavelength) const = 0;
  virtual ~Spectrum() {}
};

}  // namespace iris

#endif  // _IRIS_SPECTRUM_