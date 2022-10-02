#include "iris/bxdfs/composite_bxdf.h"

#define _USE_MATH_CONSTANTS
#include <cmath>

#include "googletest/include/gtest/gtest.h"
#include "iris/bxdfs/mock_bxdf.h"
#include "iris/random/mock_random.h"
#include "iris/reflectors/mock_reflector.h"
#include "iris/testing/bxdf_tester.h"

TEST(CompositeBxdfTest, Sample) {
  iris::random::MockRandom rng;
  EXPECT_CALL(rng, NextIndex(2)).Times(1).WillOnce(testing::Return(1));

  iris::bxdfs::MockBxdf bxdf0;
  iris::bxdfs::MockBxdf bxdf1;
  EXPECT_CALL(bxdf1, Sample(iris::Vector(1.0, 0.0, 0.0), testing::_))
      .WillOnce(testing::Return(iris::Vector(1.0, 0.0, 0.0)));

  auto composite = iris::bxdfs::MakeComposite(bxdf0, bxdf1);
  EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0),
            composite.Sample(iris::Vector(1.0, 0.0, 0.0), rng));
}

TEST(CompositeBxdfTest, PdfOneBxdf) {
  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(bxdf,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(-1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::BXDF))
      .WillOnce(testing::Return(std::nullopt));

  auto composite = iris::bxdfs::MakeComposite(bxdf);
  EXPECT_EQ(std::nullopt, composite.Pdf(iris::Vector(1.0, 0.0, 0.0),
                                        iris::Vector(-1.0, 0.0, 0.0),
                                        iris::Bxdf::SampleSource::BXDF));
}

TEST(CompositeBxdfTest, PdfTwoBxdfsEmpty) {
  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(bxdf,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(-1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::BXDF))
      .WillRepeatedly(testing::Return(std::nullopt));

  auto composite = iris::bxdfs::MakeComposite(bxdf, bxdf);
  EXPECT_EQ(std::nullopt, composite.Pdf(iris::Vector(1.0, 0.0, 0.0),
                                        iris::Vector(-1.0, 0.0, 0.0),
                                        iris::Bxdf::SampleSource::BXDF));
}

TEST(CompositeBxdfTest, PdfTwoBxdfs) {
  iris::bxdfs::MockBxdf bxdf0;
  EXPECT_CALL(bxdf0,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(-1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::BXDF))
      .WillOnce(testing::Return(std::nullopt));
  iris::bxdfs::MockBxdf bxdf1;
  EXPECT_CALL(bxdf1,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(-1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::BXDF))
      .WillOnce(testing::Return(2.0));

  auto composite = iris::bxdfs::MakeComposite(bxdf0, bxdf1);
  EXPECT_EQ(1.5, composite.Pdf(iris::Vector(1.0, 0.0, 0.0),
                               iris::Vector(-1.0, 0.0, 0.0),
                               iris::Bxdf::SampleSource::BXDF));
}

TEST(CompositeBxdfTest, LightPdfTwoBxdfs) {
  iris::bxdfs::MockBxdf bxdf0;
  EXPECT_CALL(bxdf0,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(-1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::LIGHT))
      .WillOnce(testing::Return(0.0));
  iris::bxdfs::MockBxdf bxdf1;
  EXPECT_CALL(bxdf1,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(-1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::LIGHT))
      .WillOnce(testing::Return(2.0));

  auto composite = iris::bxdfs::MakeComposite(bxdf0, bxdf1);
  EXPECT_EQ(1.0, composite.Pdf(iris::Vector(1.0, 0.0, 0.0),
                               iris::Vector(-1.0, 0.0, 0.0),
                               iris::Bxdf::SampleSource::LIGHT));
}

TEST(CompositeBxdfTest, LightPdfTwoBxdfsMissingOne) {
  iris::bxdfs::MockBxdf bxdf0;
  EXPECT_CALL(bxdf0,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(-1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::LIGHT))
      .WillOnce(testing::Return(1.0));
  iris::bxdfs::MockBxdf bxdf1;
  EXPECT_CALL(bxdf1,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(-1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::LIGHT))
      .WillOnce(testing::Return(std::nullopt));

  auto composite = iris::bxdfs::MakeComposite(bxdf0, bxdf1);
  EXPECT_EQ(0.5, composite.Pdf(iris::Vector(1.0, 0.0, 0.0),
                               iris::Vector(-1.0, 0.0, 0.0),
                               iris::Bxdf::SampleSource::LIGHT));
}

TEST(CompositeBxdfTest, LightPdfTwoBxdfsMissingBoth) {
  iris::bxdfs::MockBxdf bxdf0;
  EXPECT_CALL(bxdf0,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(-1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::LIGHT))
      .WillOnce(testing::Return(std::nullopt));
  iris::bxdfs::MockBxdf bxdf1;
  EXPECT_CALL(bxdf1,
              Pdf(iris::Vector(1.0, 0.0, 0.0), iris::Vector(-1.0, 0.0, 0.0),
                  iris::Bxdf::SampleSource::LIGHT))
      .WillOnce(testing::Return(std::nullopt));

  auto composite = iris::bxdfs::MakeComposite(bxdf0, bxdf1);
  EXPECT_FALSE(composite.Pdf(iris::Vector(1.0, 0.0, 0.0),
                             iris::Vector(-1.0, 0.0, 0.0),
                             iris::Bxdf::SampleSource::LIGHT));
}

TEST(CompositeBxdfTest, Reflectance) {
  iris::testing::BxdfTester tester;

  iris::reflectors::MockReflector reflector0;
  EXPECT_CALL(reflector0, Reflectance(1.0)).WillOnce(testing::Return(0.25));
  iris::bxdfs::MockBxdf bxdf0;
  EXPECT_CALL(bxdf0, Reflectance(iris::Vector(1.0, 0.0, 0.0),
                                 iris::Vector(-1.0, 0.0, 0.0),
                                 iris::Bxdf::SampleSource::LIGHT,
                                 iris::Bxdf::Hemisphere::BRDF, testing::_))
      .WillOnce(testing::Return(&reflector0));
  iris::reflectors::MockReflector reflector1;
  EXPECT_CALL(reflector1, Reflectance(1.0)).WillOnce(testing::Return(0.5));
  iris::bxdfs::MockBxdf bxdf1;
  EXPECT_CALL(bxdf1, Reflectance(iris::Vector(1.0, 0.0, 0.0),
                                 iris::Vector(-1.0, 0.0, 0.0),
                                 iris::Bxdf::SampleSource::LIGHT,
                                 iris::Bxdf::Hemisphere::BRDF, testing::_))
      .WillOnce(testing::Return(&reflector1));

  auto composite = iris::bxdfs::MakeComposite(bxdf0, bxdf1);
  auto* result = tester.Reflectance(
      composite, iris::Vector(1.0, 0.0, 0.0), iris::Vector(-1.0, 0.0, 0.0),
      iris::Bxdf::SampleSource::LIGHT, iris::Bxdf::Hemisphere::BRDF);
  EXPECT_EQ(0.75, result->Reflectance(1.0));
}