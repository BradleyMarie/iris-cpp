#ifndef _IRIS_BXDFS_MOCK_BXDF_
#define _IRIS_BXDFS_MOCK_BXDF_

#include <optional>

#include "googlemock/include/gmock/gmock.h"
#include "iris/bxdf.h"
#include "iris/float.h"
#include "iris/reflector.h"
#include "iris/sampler.h"
#include "iris/spectral_allocator.h"
#include "iris/vector.h"

namespace iris {
namespace bxdfs {

class MockBxdf final : public Bxdf {
 public:
  MOCK_METHOD(Vector, Sample, (const Vector&, Sampler&), (const override));
  MOCK_METHOD(std::optional<visual_t>, Pdf,
              (const Vector&, const Vector&, SampleSource), (const override));
  MOCK_METHOD(const Reflector*, Reflectance,
              (const Vector&, const Vector&, SampleSource, Hemisphere,
               SpectralAllocator&),
              (const override));
};

}  // namespace bxdfs
}  // namespace iris

#endif  // _IRIS_BXDFS_MOCK_BXDF_