#include "iris/light_sample_allocator.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/internal/arena.h"

TEST(LightSampleAllocatorTest, Allocate) {
  iris::internal::Arena arena;
  iris::LightSampleAllocator allocator(arena);

  const iris::Light* light0 = reinterpret_cast<const iris::Light*>(1);
  auto& light_sample0 = allocator.Allocate(*light0, 0.5);
  EXPECT_EQ(light0, &light_sample0.light);
  EXPECT_EQ(0.5, *light_sample0.pdf);
  EXPECT_EQ(nullptr, light_sample0.next);

  const iris::Light* light1 = reinterpret_cast<const iris::Light*>(2);
  auto& light_sample1 =
      allocator.Allocate(*light1, std::nullopt, &light_sample0);
  EXPECT_EQ(light1, &light_sample1.light);
  EXPECT_FALSE(light_sample1.pdf);
  EXPECT_EQ(&light_sample0, light_sample1.next);
}