#include "iris/testing/bxdf_tester.h"

namespace iris {
namespace testing {

const Reflector* BxdfTester::Reflectance(const Bxdf& bxdf,
                                         const Vector& incoming,
                                         const Vector& outgoing,
                                         Bxdf::SampleSource sample_source,
                                         Bxdf::Hemisphere hemisphere) {
  SpectralAllocator allocator(arena_);
  return bxdf.Reflectance(incoming, outgoing, sample_source, hemisphere,
                          allocator);
}

}  // namespace testing
}  // namespace iris