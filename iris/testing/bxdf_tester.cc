#include "iris/testing/bxdf_tester.h"

namespace iris {
namespace testing {

const Reflector* BxdfTester::Reflectance(const Bxdf& bxdf,
                                         const Vector& incoming,
                                         const Vector& outgoing,
                                         Bxdf::Type type) {
  SpectralAllocator allocator(arena_);
  return bxdf.Reflectance(incoming, outgoing, type, allocator);
}

}  // namespace testing
}  // namespace iris