#include "iris/random/mersenne_twister_random.h"

#include <limits>
#include <set>

#include "googletest/include/gtest/gtest.h"

TEST(MersenneTwisterRandomTest, NextIndex) {
  iris::random::MersenneTwisterRandom rng;
  auto next = rng.NextIndex(10u);
  EXPECT_GE(next, 0u);
  EXPECT_LT(next, 10u);
}

TEST(MersenneTwisterRandomTest, NextGeometric) {
  iris::random::MersenneTwisterRandom rng;
  auto next = rng.NextGeometric();
  EXPECT_GE(next, 0.0);
  EXPECT_LT(next, 1.0);
}

TEST(MersenneTwisterRandomTest, NextVisual) {
  iris::random::MersenneTwisterRandom rng;
  auto next = rng.NextVisual();
  EXPECT_GE(next, 0.0);
  EXPECT_LT(next, 1.0);
}

TEST(MersenneTwisterRandomTest, Replicate) {
  iris::random::MersenneTwisterRandom rng0;
  auto rng1 = rng0.Replicate();
  EXPECT_NE(rng0.NextGeometric(), rng1->NextGeometric());
}