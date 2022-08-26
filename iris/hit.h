#ifndef _IRIS_HIT_
#define _IRIS_HIT_

#include "iris/float.h"

namespace iris {

struct Hit {
  Hit* next;
  const geometric_t distance;
};

}  // namespace iris

#endif  // _IRIS_HIT_