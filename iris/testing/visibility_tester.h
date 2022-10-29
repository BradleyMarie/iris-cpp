#ifndef _IRIS_TESTING_VISIBILITY_TESTER_
#define _IRIS_TESTING_VISIBILITY_TESTER_

#include "iris/visibility_tester.h"

namespace iris {
namespace testing {

VisibilityTester& GetAlwaysVisibleVisibilityTester();
VisibilityTester& GetNeverVisibleVisibilityTester();

}  // namespace testing
}  // namespace iris

#endif  // _IRIS_TESTING_VISIBILITY_TESTER_