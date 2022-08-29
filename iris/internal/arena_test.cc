#include "iris/internal/arena.h"

#include "googletest/include/gtest/gtest.h"

TEST(Arena, AllocateZero) {
  iris::internal::Arena arena;

  auto* alloc0 = arena.Allocate(0);
  EXPECT_EQ(nullptr, alloc0);
}

TEST(Arena, Allocate) {
  iris::internal::Arena arena;

  auto& alloc0 = arena.Allocate<char>('c');
  auto& alloc1 = arena.Allocate<char>('b');
  EXPECT_EQ('c', alloc0);
  EXPECT_EQ('b', alloc1);
}

TEST(Arena, AllocateAndClear) {
  iris::internal::Arena arena;

  auto& alloc0 = arena.Allocate<char>('c');
  EXPECT_EQ('c', alloc0);

  arena.Clear();

  auto& alloc1 = arena.Allocate<char>('b');
  EXPECT_EQ('b', alloc1);

  EXPECT_EQ(&alloc0, &alloc1);
}