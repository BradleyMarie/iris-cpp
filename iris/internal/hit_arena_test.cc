#include "iris/internal/hit_arena.h"

#include "googletest/include/gtest/gtest.h"

TEST(HitArena, AllocateAndClear) {
  iris::internal::HitArena arena;

  auto& hit0 = arena.Allocate(nullptr, 1.0, 1, 2, nullptr, 0);
  EXPECT_EQ(nullptr, hit0.next);
  EXPECT_EQ(1.0, hit0.distance);
  EXPECT_EQ(nullptr, hit0.geometry);
  EXPECT_EQ(nullptr, hit0.model_to_world);
  EXPECT_EQ(1u, hit0.front);
  EXPECT_EQ(2u, hit0.back);
  EXPECT_EQ(nullptr, hit0.additional_data);

  uint64_t value[3] = {5, 6, 1};
  auto& hit1 = arena.Allocate(&hit0, 2.0, 3, 4, value, sizeof(value));
  EXPECT_EQ(&hit0, hit1.next);
  EXPECT_EQ(2.0, hit1.distance);
  EXPECT_EQ(nullptr, hit1.geometry);
  EXPECT_EQ(nullptr, hit1.model_to_world);
  EXPECT_EQ(3u, hit1.front);
  EXPECT_EQ(4u, hit1.back);
  ASSERT_NE(nullptr, hit1.additional_data);
  const auto* copy = static_cast<const uint64_t*>(hit1.additional_data);
  EXPECT_EQ(5u, copy[0]);
  EXPECT_EQ(6u, copy[1]);
  EXPECT_EQ(1u, copy[2]);

  arena.Clear();

  uint64_t maxval = 0xFFFFFFFFu;
  auto& hit2 = arena.Allocate(nullptr, 5.0, 6, 7, &maxval, sizeof(maxval));
  EXPECT_EQ(nullptr, hit2.next);
  EXPECT_EQ(5.0, hit2.distance);
  EXPECT_EQ(nullptr, hit2.geometry);
  EXPECT_EQ(nullptr, hit2.model_to_world);
  EXPECT_EQ(6u, hit2.front);
  EXPECT_EQ(7u, hit2.back);
  const auto* copy2 = static_cast<const uint64_t*>(hit2.additional_data);
  EXPECT_EQ(0xFFFFFFFFu, copy2[0]);
}