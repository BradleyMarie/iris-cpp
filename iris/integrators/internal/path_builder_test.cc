#include "iris/integrators/internal/path_builder.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/reflectors/mock_reflector.h"
#include "iris/spectra/mock_spectrum.h"
#include "iris/testing/spectral_allocator.h"

class PathBuilderTest : public ::testing::Test {
 public:
  PathBuilderTest()
      : reflectors_(),
        spectra_(),
        attenuations_(),
        path_builder_(reflectors_, spectra_, attenuations_) {}

  void Add(const iris::Spectrum& spectrum) {
    path_builder_.Add(&spectrum, iris::testing::GetSpectralAllocator());
  }

  void Bounce(const iris::Reflector& reflector, iris::visual_t attenuation) {
    path_builder_.Bounce(&reflector, attenuation);
  }

  const iris::Spectrum* Build() {
    return path_builder_.Build(iris::testing::GetSpectralAllocator());
  }

 private:
  std::vector<const iris::Reflector*> reflectors_;
  std::vector<const iris::Spectrum*> spectra_;
  std::vector<iris::visual_t> attenuations_;

 public:
  iris::integrators::internal::PathBuilder path_builder_;
};

TEST_F(PathBuilderTest, Empty) { EXPECT_EQ(nullptr, Build()); }

TEST_F(PathBuilderTest, NoSpectra) {
  iris::reflectors::MockReflector mock_reflector;
  Bounce(mock_reflector, 1.0);
  EXPECT_EQ(nullptr, Build());
}

TEST_F(PathBuilderTest, NoBounces) {
  iris::spectra::MockSpectrum first_spectrum;
  EXPECT_CALL(first_spectrum, Intensity(testing::_))
      .WillOnce(testing::Return(1.0));

  Add(first_spectrum);

  auto* sum = Build();
  ASSERT_NE(nullptr, sum);
  EXPECT_EQ(1.0, sum->Intensity(1.0));
}

TEST_F(PathBuilderTest, FullPath) {
  iris::spectra::MockSpectrum first_spectrum;
  EXPECT_CALL(first_spectrum, Intensity(testing::_))
      .WillOnce(testing::Return(1.0));

  iris::reflectors::MockReflector mock_reflector;
  EXPECT_CALL(mock_reflector, Reflectance(testing::_))
      .WillOnce(testing::Return(0.5));

  iris::spectra::MockSpectrum second_spectrum;
  EXPECT_CALL(second_spectrum, Intensity(testing::_))
      .WillOnce(testing::Return(2.0));

  Add(first_spectrum);
  Bounce(mock_reflector, 5.0);
  Add(second_spectrum);

  auto* sum = Build();
  ASSERT_NE(nullptr, sum);
  EXPECT_EQ(6.0, sum->Intensity(1.0));
}