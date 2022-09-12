#include "iris/random/mersenne_twister_random.h"

#include <limits>
#include <set>

#include "googletest/include/gtest/gtest.h"

TEST(MersenneTwisterRandomTest, MinMax) {
  iris::random::MersenneTwisterRandom rng;
  EXPECT_LE(rng.min(), rng.max());
}

TEST(MersenneTwisterRandomTest, Invoke) {
  iris::random::MersenneTwisterRandom rng;
  auto next = rng();
  EXPECT_GE(next, rng.min());
  EXPECT_LT(next, rng.max());
}

TEST(MersenneTwisterRandomTest, Discard) {
  iris::random::MersenneTwisterRandom rng0;
  rng0();
  rng0();

  iris::random::MersenneTwisterRandom rng1;
  rng1.discard(2);

  EXPECT_EQ(rng0(), rng1());
}

TEST(MersenneTwisterRandomTest, Replicate) {
  iris::random::MersenneTwisterRandom rng0;
  auto rng1 = rng0.Replicate();
  EXPECT_NE(rng0(), (*rng1)());
}