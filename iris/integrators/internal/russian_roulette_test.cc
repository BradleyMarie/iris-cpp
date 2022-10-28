#include "iris/integrators/internal/russian_roulette.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/random/mock_random.h"

using iris::Random;
using iris::integrators::internal::RussianRoulette;
using iris::random::MockRandom;

TEST(RussianRoulette, AboveThreshold) {
  MockRandom rng;
  EXPECT_CALL(rng, DiscardVisual(1)).Times(2);

  RussianRoulette roulette(1.0, 0.5);
  EXPECT_EQ(1.0, roulette.Evaluate(rng, 1.0));
  EXPECT_EQ(1.0, roulette.Evaluate(rng, 2.0));
}

TEST(RussianRoulette, AboveMaximumSuccess) {
  MockRandom rng;
  EXPECT_CALL(rng, NextVisual()).WillOnce(testing::Return(0.0));

  RussianRoulette roulette(0.75, 1.0);
  auto result = roulette.Evaluate(rng, 0.99);
  ASSERT_TRUE(result.has_value());
  EXPECT_NEAR(1.0 / 0.75, result.value(), 0.0001);
}

TEST(RussianRoulette, AboveMaximumFails) {
  MockRandom rng;
  EXPECT_CALL(rng, NextVisual()).WillOnce(testing::Return(1.0));

  RussianRoulette roulette(0.75, 1.0);
  auto result = roulette.Evaluate(rng, 0.99);
  ASSERT_FALSE(result.has_value());
}

TEST(RussianRoulette, BelowMaximumSuccess) {
  MockRandom rng;
  EXPECT_CALL(rng, NextVisual()).WillOnce(testing::Return(0.0));

  RussianRoulette roulette(0.75, 1.0);
  auto result = roulette.Evaluate(rng, 0.5);
  ASSERT_TRUE(result.has_value());
  EXPECT_NEAR(2.0, result.value(), 0.0001);
}

TEST(RussianRoulette, BelowMaximumFails) {
  MockRandom rng;
  EXPECT_CALL(rng, NextVisual()).WillOnce(testing::Return(1.0));

  RussianRoulette roulette(0.75, 1.0);
  auto result = roulette.Evaluate(rng, 0.5);
  ASSERT_FALSE(result.has_value());
}