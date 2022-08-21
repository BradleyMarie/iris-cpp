#include "iris/internal/arena.h"

#include "googletest/include/gtest/gtest.h"

TEST(Arena, AllocateZero) {
  auto arena = iris::internal::Arena();

  auto* alloc0 = arena.Allocate(0);
  EXPECT_EQ(nullptr, alloc0);
}

TEST(Arena, Allocate) {
  auto arena = iris::internal::Arena();

  auto* alloc0 = arena.Allocate(1);
  EXPECT_NE(nullptr, alloc0);

  auto* alloc1 = arena.Allocate(1);
  EXPECT_NE(nullptr, alloc1);

  EXPECT_NE(alloc0, alloc1);
}

TEST(Arena, AllocateAndClear) {
  auto arena = iris::internal::Arena();

  auto* alloc0 = arena.Allocate(1);
  EXPECT_NE(nullptr, alloc0);

  arena.Clear();

  auto* alloc1 = arena.Allocate(1);
  EXPECT_NE(nullptr, alloc1);

  EXPECT_EQ(alloc0, alloc1);
}