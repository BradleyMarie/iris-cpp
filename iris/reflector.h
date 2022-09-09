#ifndef _IRIS_REFLECTOR_
#define _IRIS_REFLECTOR_

#include "iris/float.h"

namespace iris {

class Reflector {
 public:
  virtual visual Reflectance(visual wavelength) const = 0;
  virtual ~Reflector() {}
};

}  // namespace iris

#endif  // _IRIS_REFLECTOR_