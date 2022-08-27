#ifndef _IRIS_LIGHT_
#define _IRIS_LIGHT_

#include <optional>

#include "iris/float.h"
#include "iris/point.h"
#include "iris/random.h"
#include "iris/ray.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"
#include "iris/vector.h"
#include "iris/visibility_tester.h"

namespace iris {

class Light {
 public:
  struct SampleResult {
    const Spectrum& emission;
    Vector to_light;
    visual_t pdf;
  };

  virtual std::optional<SampleResult> Sample(
      const Point& hit_point, const Vector& surface_normal, Random& rng,
      VisibilityTester& tester, SpectralAllocator& allocator) const = 0;

  virtual const Spectrum* Emission(const Ray& to_light,
                                   VisibilityTester& tester,
                                   SpectralAllocator& allocator,
                                   visual_t* pdf = nullptr) const = 0;

  virtual ~Light() {}
};

}  // namespace iris

#endif  // _IRIS_LIGHT_