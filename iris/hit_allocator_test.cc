#include "iris/hit_allocator.h"

#include <array>

#include "googletest/include/gtest/gtest.h"
#include "iris/internal/hit_arena.h"

TEST(HitArena, Allocate) {
  auto ray = iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));

  auto arena = iris::internal::HitArena();
  iris::HitAllocator allocator(ray, arena);

  auto& hit0 = allocator.Allocate(nullptr, 1.0, 1, 2);
  EXPECT_EQ(nullptr, hit0.next);
  EXPECT_EQ(1.0, hit0.distance);

  auto& full_hit0 = static_cast<iris::internal::Hit&>(hit0);
  EXPECT_EQ(nullptr, full_hit0.geometry);
  EXPECT_EQ(nullptr, full_hit0.model_to_world);
  EXPECT_EQ(1u, full_hit0.front);
  EXPECT_EQ(2u, full_hit0.back);
  EXPECT_EQ(nullptr, full_hit0.additional_data);

  std::array<double, 3> array = {1.0, 2.0, 3.0};
  auto& hit1 = allocator.Allocate(&hit0, 2.0, 3, 4, array);
  EXPECT_EQ(&hit0, hit1.next);
  EXPECT_EQ(2.0, hit1.distance);

  auto& full_hit1 = static_cast<iris::internal::Hit&>(hit1);
  EXPECT_EQ(nullptr, full_hit1.geometry);
  EXPECT_EQ(nullptr, full_hit1.model_to_world);
  EXPECT_EQ(3u, full_hit1.front);
  EXPECT_EQ(4u, full_hit1.back);
  ASSERT_NE(nullptr, full_hit1.additional_data);
  auto* array_copy =
      static_cast<const std::array<double, 3>*>(full_hit1.additional_data);
  EXPECT_EQ(1.0, (*array_copy)[0]);
  EXPECT_EQ(2.0, (*array_copy)[1]);
  EXPECT_EQ(3.0, (*array_copy)[2]);
}