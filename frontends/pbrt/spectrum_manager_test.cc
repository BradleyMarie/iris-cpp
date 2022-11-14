#include "frontends/pbrt/spectrum_manager.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/reflectors/mock_reflector.h"
#include "iris/spectra/mock_spectrum.h"

class TestSpectrumManager final : public iris::pbrt_frontend::SpectrumManager {
 public:
  iris::ReferenceCounted<iris::Spectrum> AllocateSpectrum(
      const std::map<iris::visual, iris::visual> wavelengths) const override {
    EXPECT_FALSE(true);
    return iris::ReferenceCounted<iris::Spectrum>();
  }

  iris::ReferenceCounted<iris::Reflector> AllocateReflector(
      const std::map<iris::visual, iris::visual> wavelengths) const override {
    EXPECT_FALSE(true);
    return iris::ReferenceCounted<iris::Reflector>();
  }

 protected:
  iris::ReferenceCounted<iris::Spectrum> AllocateSpectrum(
      const iris::Color& color) const override {
    return iris::MakeReferenceCounted<iris::spectra::MockSpectrum>();
  }

  iris::ReferenceCounted<iris::Reflector> AllocateReflector(
      const iris::Color& color) const override {
    return iris::MakeReferenceCounted<iris::reflectors::MockReflector>();
  }
};

TEST(SpectrumManager, ZeroReflector) {
  iris::Color black(0.0, 0.0, 0.0, iris::Color::LINEAR_SRGB);
  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::SpectrumManager& base = spectrum_manager;
  EXPECT_EQ(nullptr, base.AllocateReflector(black).Get());
}

TEST(SpectrumManager, Reflector) {
  iris::Color red(1.0, 0.0, 0.0, iris::Color::LINEAR_SRGB);
  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::SpectrumManager& base = spectrum_manager;
  auto reflector = base.AllocateReflector(red).Get();
  EXPECT_TRUE(reflector);
  EXPECT_EQ(reflector, base.AllocateReflector(red).Get());
}

TEST(SpectrumManager, ZeroSpectrum) {
  iris::Color black(0.0, 0.0, 0.0, iris::Color::LINEAR_SRGB);
  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::SpectrumManager& base = spectrum_manager;
  EXPECT_EQ(nullptr, base.AllocateSpectrum(black).Get());
}

TEST(SpectrumManager, Spectrum) {
  iris::Color red(1.0, 0.0, 0.0, iris::Color::LINEAR_SRGB);
  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::SpectrumManager& base = spectrum_manager;
  auto spectrum = base.AllocateSpectrum(red).Get();
  EXPECT_TRUE(spectrum);
  EXPECT_EQ(spectrum, base.AllocateSpectrum(red).Get());
}