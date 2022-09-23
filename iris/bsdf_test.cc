#include "iris/bsdf.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/bxdfs/mock_bxdf.h"
#include "iris/internal/arena.h"
#include "iris/random/mock_random.h"
#include "iris/reflectors/mock_reflector.h"

iris::random::MockRandom g_rng;
iris::reflectors::MockReflector g_reflector;

TEST(BsdfTest, Normalize) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(bxdf, Sample(iris::Vector(1.0, 0.0, 0.0), testing::_, testing::_))
      .WillOnce(testing::Return(iris::Bxdf::SampleResult{
          g_reflector, iris::Vector(1.0, 0.0, 0.0), 0.5}));

  iris::Bsdf bsdf(bxdf, iris::Vector(0.0, 0.0, 2.0),
                  iris::Vector(0.0, 0.0, 2.0), true);
  auto result = bsdf.Sample(iris::Vector(1.0, 0.0, 0.0), g_rng, allocator);
  EXPECT_EQ(&g_reflector, &result.reflector);
  EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0), result.direction);
  EXPECT_EQ(0.5, result.pdf.value());
}

TEST(BsdfTest, Sample) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(bxdf, Sample(iris::Vector(0.0, 0.0, 1.0), testing::_, testing::_))
      .WillOnce(testing::Return(iris::Bxdf::SampleResult{
          g_reflector, iris::Vector(0.0, 0.0, 1.0), 0.5}));

  iris::Bsdf bsdf(bxdf, iris::Vector(0.0, 1.0, 0.0),
                  iris::Vector(0.0, 1.0, 0.0));
  auto result = bsdf.Sample(iris::Vector(0.0, 1.0, 0.0), g_rng, allocator);
  EXPECT_EQ(&g_reflector, &result.reflector);
  EXPECT_EQ(iris::Vector(0.0, 1.0, 0.0), result.direction);
  EXPECT_EQ(0.5, result.pdf.value());
}

TEST(BsdfTest, ReflectionBrdf) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(bxdf, Reflectance(iris::Vector(0.0, 0.0, 1.0),
                                iris::Vector(0.0, 0.0, -1.0), iris::Bxdf::BRDF,
                                testing::_, testing::_))
      .WillOnce(testing::Return(&g_reflector));

  iris::Bsdf bsdf(bxdf, iris::Vector(0.0, 1.0, 0.0),
                  iris::Vector(0.0, 1.0, 0.0));
  auto result = bsdf.Reflectance(iris::Vector(0.0, 1.0, 0.0),
                                 iris::Vector(0.0, -1.0, 0.0), allocator);
  EXPECT_EQ(&g_reflector, result);
}

TEST(BsdfTest, ReflectionBtdf) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(bxdf, Reflectance(iris::Vector(0.0, 0.0, 1.0),
                                iris::Vector(0.0, 0.0, 1.0), iris::Bxdf::BTDF,
                                testing::_, testing::_))
      .WillOnce(testing::Return(&g_reflector));

  iris::Bsdf bsdf(bxdf, iris::Vector(0.0, 1.0, 0.0),
                  iris::Vector(0.0, 1.0, 0.0));
  auto result = bsdf.Reflectance(iris::Vector(0.0, 1.0, 0.0),
                                 iris::Vector(0.0, 1.0, 0.0), allocator);
  EXPECT_EQ(&g_reflector, result);
}