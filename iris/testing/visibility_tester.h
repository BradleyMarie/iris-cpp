#ifndef _IRIS_TESTING_VISIBILITY_TESTER_
#define _IRIS_TESTING_VISIBILITY_TESTER_

#include <memory>

#include "iris/scene.h"
#include "iris/visibility_tester.h"

namespace iris {
namespace testing {

VisibilityTester& GetAlwaysVisibleVisibilityTester();
VisibilityTester& GetNeverVisibleVisibilityTester();

std::unique_ptr<VisibilityTester> GetVisibilityTester(const Scene& scene);

}  // namespace testing
}  // namespace iris

#endif  // _IRIS_TESTING_VISIBILITY_TESTER_