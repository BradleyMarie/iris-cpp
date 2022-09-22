#ifndef _IRIS_BXDFS_MOCK_BXDF_
#define _IRIS_BXDFS_MOCK_BXDF_

#include <optional>

#include "googlemock/include/gmock/gmock.h"
#include "iris/bxdf.h"
#include "iris/float.h"
#include "iris/random.h"
#include "iris/reflector.h"
#include "iris/spectral_allocator.h"
#include "iris/vector.h"

namespace iris {
namespace bxdfs {

class MockBxdf final : public Bxdf {
 public:
  MockBxdf(Bxdf::Type type) noexcept : Bxdf(type) {}

  MOCK_METHOD(SampleResult, Sample,
              (const Vector&, Random&, SpectralAllocator&), (const override));

  MOCK_METHOD(const Reflector*, Reflectance,
              (const Vector&, const Vector&, SpectralAllocator&, visual_t*),
              (const override));
};

}  // namespace bxdfs
}  // namespace iris

#endif  // _IRIS_BXDFS_MOCK_BXDF_