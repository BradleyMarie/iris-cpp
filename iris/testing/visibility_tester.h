#ifndef _IRIS_TESTING_VISIBILITY_TESTER_
#define _IRIS_TESTING_VISIBILITY_TESTER_

#include <functional>
#include <memory>

#include "iris/geometry.h"
#include "iris/matrix.h"
#include "iris/visibility_tester.h"

namespace iris {
namespace testing {

VisibilityTester& GetAlwaysVisibleVisibilityTester();
VisibilityTester& GetNeverVisibleVisibilityTester();

void ScopedSingleGeometryVisibilityTester(
    const Geometry& geometry, const Matrix* matrix,
    std::function<void(VisibilityTester&)> callback);

}  // namespace testing
}  // namespace iris

#endif  // _IRIS_TESTING_VISIBILITY_TESTER_