#ifndef _IRIS_LIGHT_SCENE_
#define _IRIS_LIGHT_SCENE_

#include "iris/light.h"
#include "iris/light_sample.h"
#include "iris/light_sample_allocator.h"
#include "iris/point.h"
#include "iris/random.h"

namespace iris {

class LightScene {
 public:
  class Builder {
   public:
    void Add(std::unique_ptr<Light> light);
    std::unique_ptr<LightScene> Build(const Scene& scene);

   private:
    virtual std::unique_ptr<LightScene> Build(
        std::vector<std::unique_ptr<Light>> lights) = 0;

    std::vector<std::unique_ptr<Light>> lights_;
  };

  virtual LightSample* Sample(const Point& hit_point, Random& rng,
                              LightSampleAllocator& allocator) const = 0;
  virtual ~LightScene() {}
};

}  // namespace iris

#endif  // _IRIS_LIGHT_SCENE_