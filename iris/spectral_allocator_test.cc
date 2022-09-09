#include "iris/spectral_allocator.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/internal/arena.h"

class TestSpectrum : public iris::Spectrum {
 public:
  TestSpectrum(iris::visual_t wavelength, iris::visual_t intensity)
      : wavelength_(wavelength), intensity_(intensity) {}

  iris::visual_t Intensity(iris::visual_t wavelength) const override {
    EXPECT_EQ(wavelength_, wavelength);
    return intensity_;
  }

 private:
  iris::visual_t wavelength_;
  iris::visual_t intensity_;
};

class TestReflector : public iris::Reflector {
 public:
  TestReflector(iris::visual_t wavelength, iris::visual_t reflectance)
      : wavelength_(wavelength), reflectance_(reflectance) {}

  iris::visual_t Reflectance(iris::visual_t wavelength) const override {
    EXPECT_EQ(wavelength_, wavelength);
    return reflectance_;
  }

 private:
  iris::visual_t wavelength_;
  iris::visual_t reflectance_;
};

TEST(SpectralAllocator, SpectrumAddNullptr) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  TestSpectrum spectrum(0.0, 0.0);
  EXPECT_EQ(nullptr,
            allocator.Add(static_cast<const iris::Spectrum*>(nullptr),
                          static_cast<const iris::Spectrum*>(nullptr)));
  EXPECT_EQ(&spectrum, allocator.Add(&spectrum, nullptr));
  EXPECT_EQ(&spectrum, allocator.Add(nullptr, &spectrum));
}

TEST(SpectralAllocator, SpectrumAdd) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  TestSpectrum spectrum0(1.0, 1.0);
  TestSpectrum spectrum1(1.0, 2.0);
  EXPECT_EQ(3.0, allocator.Add(&spectrum0, &spectrum1)->Intensity(1.0));
}

TEST(SpectralAllocator, SpectrumScaleNullptr) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  TestSpectrum spectrum(0.0, 0.0);
  EXPECT_EQ(nullptr,
            allocator.Scale(static_cast<const iris::Spectrum*>(nullptr), 0.0));
  EXPECT_EQ(nullptr, allocator.Scale(&spectrum, 0.0));
}

TEST(SpectralAllocator, SpectrumScaleIdentity) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  TestSpectrum spectrum(0.0, 0.0);
  EXPECT_EQ(&spectrum, allocator.Scale(&spectrum, 1.0));
}

TEST(SpectralAllocator, SpectrumScale) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  TestSpectrum spectrum(1.0, 0.5);
  EXPECT_EQ(0.25, allocator.Scale(&spectrum, 0.5)->Intensity(1.0));
}

TEST(SpectralAllocator, SpectrumReflectNullptr) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  TestSpectrum spectrum(0.0, 0.0);
  TestReflector reflector(0.0, 0.0);
  EXPECT_EQ(nullptr, allocator.Reflect(nullptr, nullptr));
  EXPECT_EQ(nullptr, allocator.Reflect(&spectrum, nullptr));
  EXPECT_EQ(nullptr, allocator.Reflect(nullptr, &reflector));
}

TEST(SpectralAllocator, SpectrumReflect) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  TestSpectrum spectrum(1.0, 2.0);
  TestReflector reflector(1.0, 0.25);
  EXPECT_EQ(0.5, allocator.Reflect(&spectrum, &reflector)->Intensity(1.0));
}

TEST(SpectralAllocator, ReflectorAddNullptr) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  TestReflector reflector(0.0, 1.0);
  EXPECT_EQ(nullptr,
            allocator.Add(static_cast<const iris::Reflector*>(nullptr),
                          static_cast<const iris::Reflector*>(nullptr)));
  EXPECT_EQ(&reflector, allocator.Add(&reflector, nullptr));
  EXPECT_EQ(&reflector, allocator.Add(nullptr, &reflector));
}

TEST(SpectralAllocator, ReflectorAdd) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  TestReflector reflector0(1.0, 0.125);
  TestReflector reflector1(1.0, 0.250);
  EXPECT_EQ(0.375, allocator.Add(&reflector0, &reflector1)->Reflectance(1.0));
}

TEST(SpectralAllocator, ReflectorScaleNullptr) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  TestReflector reflector(0.0, 1.0);
  EXPECT_EQ(nullptr,
            allocator.Scale(static_cast<const iris::Reflector*>(nullptr), 0.0));
  EXPECT_EQ(nullptr, allocator.Scale(&reflector, 0.0));
}

TEST(SpectralAllocator, ReflectorScaleIdentity) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  TestReflector reflector(0.0, 1.0);
  EXPECT_EQ(&reflector, allocator.Scale(&reflector, 1.0));
}

TEST(SpectralAllocator, ReflectorScale) {
  iris::internal::Arena arena;
  iris::SpectralAllocator allocator(arena);

  TestReflector reflector(1.0, 0.5);
  EXPECT_EQ(0.25, allocator.Scale(&reflector, 0.5)->Reflectance(1.0));
}