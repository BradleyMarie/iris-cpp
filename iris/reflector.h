#ifndef _IRIS_REFLECTOR_
#define _IRIS_REFLECTOR_

#include "iris/float.h"
#include "iris/reference_countable.h"

namespace iris {

class Reflector : public ReferenceCountable {
 public:
  virtual visual_t Reflectance(visual_t wavelength) const = 0;
  virtual visual_t Albedo() const = 0;
  virtual ~Reflector() {}
};

}  // namespace iris

#endif  // _IRIS_REFLECTOR_