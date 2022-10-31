#include "iris/sampler.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/random/mock_random.h"

TEST(Sampler, NoSamples) {
  iris::random::MockRandom random;
  EXPECT_CALL(random, DiscardGeometric(2));
  { iris::Sampler sampler(random); }
}

TEST(Sampler, OneSample) {
  iris::random::MockRandom random;
  EXPECT_CALL(random, NextGeometric());
  EXPECT_CALL(random, DiscardGeometric(1));
  {
    iris::Sampler sampler(random);
    sampler.Next();
  }
}

TEST(Sampler, TwoSamples) {
  iris::random::MockRandom random;
  EXPECT_CALL(random, NextGeometric()).Times(2);
  EXPECT_CALL(random, DiscardGeometric(0));
  {
    iris::Sampler sampler(random);
    sampler.Next();
    sampler.Next();
  }
}