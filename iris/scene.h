#ifndef _IRIS_SCENE_
#define _IRIS_SCENE_

#include "iris/intersector.h"
#include "iris/ray.h"

namespace iris {

class Scene {
 public:
  virtual void Trace(const Ray& ray, Intersector& intersector) const = 0;
  virtual ~Scene() {}
};

}  // namespace iris

#endif  // _IRIS_SCENE_