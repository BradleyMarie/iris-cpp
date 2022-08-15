#ifndef _IRIS_HIT_
#define _IRIS_HIT_

#include "iris/float.h"

namespace iris {
namespace internal {
class Hit;
}  // namespace internal

struct Hit {
  Hit* next;
  const geometric_t distance;

 private:
  Hit(Hit* next, geometric_t distance) : next(next), distance(distance) {}

  friend internal::Hit;
};

}  // namespace iris

#endif  // _IRIS_HIT_