#include "iris/random.h"

#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"

class TestMockRandom final : public iris::Random {
 public:
  MOCK_METHOD(size_t, NextIndex, (size_t), (override));
  MOCK_METHOD(iris::geometric, NextGeometric, (), (override));
  MOCK_METHOD(iris::visual, NextVisual, (), (override));
  MOCK_METHOD(std::unique_ptr<Random>, Replicate, (), (override));
};

TEST(RandomTest, DiscardIndex) {
  TestMockRandom rng;
  EXPECT_CALL(rng, NextIndex(128u))
      .Times(10)
      .WillRepeatedly(testing::Return(1));
  rng.DiscardIndex(10u);
}

TEST(RandomTest, DiscardGeometric) {
  TestMockRandom rng;
  EXPECT_CALL(rng, NextGeometric())
      .Times(10)
      .WillRepeatedly(testing::Return(1.0));
  rng.DiscardGeometric(10u);
}

TEST(RandomTest, DiscardVisual) {
  TestMockRandom rng;
  EXPECT_CALL(rng, NextVisual()).Times(10).WillRepeatedly(testing::Return(1.0));
  rng.DiscardVisual(10u);
}