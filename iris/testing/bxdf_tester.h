#ifndef _IRIS_TESTING_BXDF_TESTER_
#define _IRIS_TESTING_BXDF_TESTER_

#include "iris/bxdf.h"
#include "iris/internal/arena.h"
#include "iris/reflector.h"
#include "iris/vector.h"

namespace iris {
namespace testing {

class BxdfTester final {
 public:
  const Reflector* Reflectance(const Bxdf& bxdf, const Vector& incoming,
                               const Vector& outgoing,
                               Bxdf::SampleSource sample_source,
                               Bxdf::Hemisphere hemisphere);

 private:
  internal::Arena arena_;
};

}  // namespace testing
}  // namespace iris

#endif  // _IRIS_TESTING_BXDF_TESTER_