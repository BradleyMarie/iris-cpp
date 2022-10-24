#include "iris/integrators/internal/sample_direct_lighting.h"

#include "googletest/include/gtest/gtest.h"

using iris::integrators::internal::SampleDirectLighting;
using iris::integrators::internal::internal::DeltaLight;
using iris::integrators::internal::internal::FromBsdfSample;
using iris::integrators::internal::internal::FromLightSample;
using iris::integrators::internal::internal::PowerHeuristic;

TEST(PowerHeuristict, Test) {
  EXPECT_NEAR(0.5, PowerHeuristic(1.0, 1.0), 0.001);
  EXPECT_NEAR(0.2, PowerHeuristic(1.0, 2.0), 0.001);
}