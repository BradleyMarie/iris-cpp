#include "iris/emissive_materials/constant_emissive_material.h"

#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "iris/spectra/mock_spectrum.h"
#include "iris/testing/spectral_allocator.h"

TEST(ConstantEmissiveMaterialTest, Evaluate) {
  auto spectrum = iris::MakeReferenceCounted<iris::spectra::MockSpectrum>();
  iris::emissive_materials::ConstantEmissiveMaterial material(spectrum);
  EXPECT_EQ(spectrum.Get(),
            material.Evaluate(iris::TextureCoordinates{},
                              iris::testing::GetSpectralAllocator()));
}