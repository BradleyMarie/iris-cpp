#ifndef _IRIS_BSDFS_MOCK_BSDF_
#define _IRIS_BSDFS_MOCK_BSDF_

#include <optional>

#include "googlemock/include/gmock/gmock.h"
#include "iris/bsdf.h"
#include "iris/float.h"
#include "iris/random.h"
#include "iris/reflector.h"
#include "iris/spectral_allocator.h"
#include "iris/vector.h"

namespace iris {
namespace bsdfs {

class MockBsdf final : public Bsdf {
 public:
  MOCK_METHOD(std::optional<Sample>, SampleAll,
              (const Vector&, const Vector&, const Vector&, Random&,
               SpectralAllocator&),
              (const override));

  MOCK_METHOD(std::optional<Sample>, SampleDiffuse,
              (const Vector&, const Vector&, const Vector&, Random&,
               SpectralAllocator&),
              (const override));

  MOCK_METHOD(const Reflector*, ReflectanceAll,
              (const Vector&, const Vector&, const Vector&, const Vector&, bool,
               SpectralAllocator&, visual_t*),
              (const override));

  MOCK_METHOD(const Reflector*, ReflectanceDiffuse,
              (const Vector&, const Vector&, const Vector&, const Vector&, bool,
               SpectralAllocator&, visual_t*),
              (const override));
};

}  // namespace bsdfs
}  // namespace iris

#endif  // _IRIS_BSDFS_MOCK_BSDF_