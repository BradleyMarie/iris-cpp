#ifndef _IRIS_SPECTRA_REFERENCE_COUNTABLE_REFLECTOR_
#define _IRIS_SPECTRA_REFERENCE_COUNTABLE_REFLECTOR_

#include "iris/float.h"
#include "iris/reference_countable.h"
#include "iris/reflector.h"

namespace iris {
namespace reflectors {

class ReferenceCountableReflector : public Reflector,
                                    public ReferenceCountable {
 public:
  virtual ~ReferenceCountableReflector() = default;
};

}  // namespace reflectors
}  // namespace iris

#endif  // _IRIS_SPECTRA_REFERENCE_COUNTABLE_REFLECTOR_