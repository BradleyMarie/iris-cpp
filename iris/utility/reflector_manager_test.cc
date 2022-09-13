#include "iris/utility/reflector_manager.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/reflectors/mock_reflector.h"

TEST(ReflectorManagerTest, EmptyGet) {
  iris::utility::ReflectorManager manager;
  EXPECT_EQ(nullptr, manager.Get(0));
}

TEST(ReflectorManagerTest, AddGet) {
  auto reflector = std::make_unique<iris::reflectors::MockReflector>();
  auto ptr = reflector.get();

  iris::utility::ReflectorManager manager;
  auto index = manager.Add(std::move(reflector));
  EXPECT_NE(0u, index);
  EXPECT_EQ(ptr, manager.Get(index));
}