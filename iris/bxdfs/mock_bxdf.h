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
  MOCK_METHOD(Vector, Sample, (const Vector&, Random&), (const override));
  MOCK_METHOD(visual_t, DiffusePdf, (const Vector&, const Vector&),
              (const override));
  MOCK_METHOD(std::optional<visual_t>, SamplePdf,
              (const Vector&, const Vector&), (const override));
  MOCK_METHOD(const Reflector*, Reflectance,
              (const Vector&, const Vector&, Type, SpectralAllocator&),
              (const override));
};

}  // namespace bxdfs
}  // namespace iris

#endif  // _IRIS_BXDFS_MOCK_BXDF_