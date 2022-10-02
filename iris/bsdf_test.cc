#include "iris/bsdf.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/bxdfs/mock_bxdf.h"
#include "iris/internal/arena.h"
#include "iris/random/mock_random.h"
#include "iris/reflectors/mock_reflector.h"

iris::random::MockRandom g_rng;
iris::reflectors::MockReflector g_reflector;

TEST(BsdfTest, SampleNoPdf) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(bxdf, Sample(iris::Vector(1.0, 0.0, 0.0), testing::_))
      .WillOnce(testing::Return(iris::Vector(1.0, 0.0, 0.0)));
  EXPECT_CALL(bxdf,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::BXDF))
      .WillOnce(testing::Return(std::optional<iris::visual_t>(0.0)));

  iris::Bsdf bsdf(bxdf, iris::Vector(0.0, 0.0, 1.0),
                  iris::Vector(0.0, 0.0, 1.0));
  auto result = bsdf.Sample(iris::Vector(1.0, 0.0, 0.0), g_rng, allocator);
  EXPECT_FALSE(result);
}

TEST(BsdfTest, SampleNoReflector) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(bxdf, Sample(iris::Vector(1.0, 0.0, 0.0), testing::_))
      .WillOnce(testing::Return(iris::Vector(1.0, 0.0, 0.0)));
  EXPECT_CALL(bxdf,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::BXDF))
      .WillOnce(testing::Return(std::optional<iris::visual_t>(0.5)));
  EXPECT_CALL(bxdf, Reflectance(iris::Vector(1.0, 0.0, 0.0),
                                iris::Vector(1.0, 0.0, 0.0),
                                iris::Bxdf::SampleSource::BXDF,
                                iris::Bxdf::Hemisphere::BTDF, testing::_))
      .WillOnce(testing::Return(nullptr));

  iris::Bsdf bsdf(bxdf, iris::Vector(0.0, 0.0, 1.0),
                  iris::Vector(0.0, 0.0, 1.0));
  auto result = bsdf.Sample(iris::Vector(1.0, 0.0, 0.0), g_rng, allocator);
  EXPECT_FALSE(result);
}

TEST(BsdfTest, SampleBtdf) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(bxdf, Sample(iris::Vector(1.0, 0.0, 0.0), testing::_))
      .WillOnce(testing::Return(iris::Vector(1.0, 0.0, 0.0)));
  EXPECT_CALL(bxdf,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::BXDF))
      .WillOnce(testing::Return(std::optional<iris::visual_t>(0.5)));
  EXPECT_CALL(bxdf, Reflectance(iris::Vector(1.0, 0.0, 0.0),
                                iris::Vector(1.0, 0.0, 0.0),
                                iris::Bxdf::SampleSource::BXDF,
                                iris::Bxdf::Hemisphere::BTDF, testing::_))
      .WillOnce(testing::Return(&g_reflector));

  iris::Bsdf bsdf(bxdf, iris::Vector(0.0, 0.0, 1.0),
                  iris::Vector(0.0, 0.0, 1.0), true);
  auto result = bsdf.Sample(iris::Vector(1.0, 0.0, 0.0), g_rng, allocator);
  EXPECT_TRUE(result);
  EXPECT_EQ(&g_reflector, &result->reflector);
  EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0), result->direction);
  EXPECT_EQ(0.5, result->pdf.value());
}

TEST(BsdfTest, SampleBrdf) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(bxdf, Sample(iris::Vector(0.0, 0.0, 1.0), testing::_))
      .WillOnce(testing::Return(iris::Vector(0.0, 0.0, -1.0)));
  EXPECT_CALL(bxdf,
              Pdf(iris::Vector(0.0, 0.0, 1.0), iris::Vector(0.0, 0.0, -1.0),
                  iris::Bxdf::SampleSource::BXDF))
      .WillOnce(testing::Return(std::optional<iris::visual_t>(0.5)));
  EXPECT_CALL(bxdf, Reflectance(iris::Vector(0.0, 0.0, 1.0),
                                iris::Vector(0.0, 0.0, -1.0),
                                iris::Bxdf::SampleSource::BXDF,
                                iris::Bxdf::Hemisphere::BRDF, testing::_))
      .WillOnce(testing::Return(&g_reflector));

  iris::Bsdf bsdf(bxdf, iris::Vector(0.0, 0.0, 1.0),
                  iris::Vector(0.0, 0.0, 1.0), true);
  auto result = bsdf.Sample(iris::Vector(0.0, 0.0, 1.0), g_rng, allocator);
  EXPECT_TRUE(result);
  EXPECT_EQ(&g_reflector, &result->reflector);
  EXPECT_EQ(iris::Vector(0.0, 0.0, -1.0), result->direction);
  EXPECT_EQ(0.5, result->pdf.value());
}

TEST(BsdfTest, ReflectanceNoPdf) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(bxdf,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::LIGHT))
      .WillOnce(testing::Return(0.0));

  iris::Bsdf bsdf(bxdf, iris::Vector(0.0, 0.0, 1.0),
                  iris::Vector(0.0, 0.0, 1.0), true);
  auto result = bsdf.Reflectance(iris::Vector(1.0, 0.0, 0.0),
                                 iris::Vector(1.0, 0.0, 0.0), allocator);
  EXPECT_FALSE(result);
}

TEST(BsdfTest, ReflectanceNoReflector) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(bxdf,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::LIGHT))
      .WillOnce(testing::Return(0.5));
  EXPECT_CALL(bxdf, Reflectance(iris::Vector(1.0, 0.0, 0.0),
                                iris::Vector(1.0, 0.0, 0.0),
                                iris::Bxdf::SampleSource::LIGHT,
                                iris::Bxdf::Hemisphere::BTDF, testing::_))
      .WillOnce(testing::Return(nullptr));

  iris::Bsdf bsdf(bxdf, iris::Vector(0.0, 0.0, 1.0),
                  iris::Vector(0.0, 0.0, 1.0), true);
  auto result = bsdf.Reflectance(iris::Vector(1.0, 0.0, 0.0),
                                 iris::Vector(1.0, 0.0, 0.0), allocator);
  EXPECT_FALSE(result);
}

TEST(BsdfTest, ReflectanceBtdf) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(bxdf,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::LIGHT))
      .WillOnce(testing::Return(0.5));
  EXPECT_CALL(bxdf, Reflectance(iris::Vector(1.0, 0.0, 0.0),
                                iris::Vector(1.0, 0.0, 0.0),
                                iris::Bxdf::SampleSource::LIGHT,
                                iris::Bxdf::Hemisphere::BTDF, testing::_))
      .WillOnce(testing::Return(&g_reflector));

  iris::Bsdf bsdf(bxdf, iris::Vector(0.0, 0.0, 1.0),
                  iris::Vector(0.0, 0.0, 1.0), true);
  auto result = bsdf.Reflectance(iris::Vector(1.0, 0.0, 0.0),
                                 iris::Vector(1.0, 0.0, 0.0), allocator);
  EXPECT_TRUE(result);
  EXPECT_EQ(&g_reflector, &result->reflector);
  EXPECT_EQ(0.5, result->pdf);
}

TEST(BsdfTest, ReflectanceBrdf) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(bxdf,
              Pdf(iris::Vector(0.0, 0.0, 1.0), iris::Vector(0.0, 0.0, -1.0),
                  iris::Bxdf::SampleSource::LIGHT))
      .WillOnce(testing::Return(0.5));
  EXPECT_CALL(bxdf, Reflectance(iris::Vector(0.0, 0.0, 1.0),
                                iris::Vector(0.0, 0.0, -1.0),
                                iris::Bxdf::SampleSource::LIGHT,
                                iris::Bxdf::Hemisphere::BRDF, testing::_))
      .WillOnce(testing::Return(&g_reflector));

  iris::Bsdf bsdf(bxdf, iris::Vector(0.0, 0.0, 1.0),
                  iris::Vector(0.0, 0.0, 1.0), true);
  auto result = bsdf.Reflectance(iris::Vector(0.0, 0.0, 1.0),
                                 iris::Vector(0.0, 0.0, -1.0), allocator);
  EXPECT_TRUE(result);
  EXPECT_EQ(&g_reflector, &result->reflector);
  EXPECT_EQ(0.5, result->pdf);
}

TEST(BsdfTest, Normalize) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(bxdf, Sample(iris::Vector(1.0, 0.0, 0.0), testing::_))
      .WillOnce(testing::Return(iris::Vector(1.0, 0.0, 0.0)));
  EXPECT_CALL(bxdf,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::BXDF))
      .WillOnce(testing::Return(std::optional<iris::visual_t>(0.5)));
  EXPECT_CALL(bxdf, Reflectance(iris::Vector(1.0, 0.0, 0.0),
                                iris::Vector(1.0, 0.0, 0.0),
                                iris::Bxdf::SampleSource::BXDF,
                                iris::Bxdf::Hemisphere::BTDF, testing::_))
      .WillOnce(testing::Return(&g_reflector));

  iris::Bsdf bsdf(bxdf, iris::Vector(0.0, 0.0, 2.0),
                  iris::Vector(0.0, 0.0, 2.0), true);
  auto result = bsdf.Sample(iris::Vector(1.0, 0.0, 0.0), g_rng, allocator);
  EXPECT_TRUE(result);
  EXPECT_EQ(&g_reflector, &result->reflector);
  EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0), result->direction);
  EXPECT_EQ(0.5, result->pdf.value());
}