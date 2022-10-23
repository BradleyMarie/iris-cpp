#include "iris/emissive_materials/constant_emissive_material.h"

#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "iris/spectra/mock_spectrum.h"
#include "iris/testing/emissive_material_tester.h"

TEST(ConstantEmissiveMaterialTest, Evaluate) {
  auto spectrum = iris::MakeReferenceCounted<iris::spectra::MockSpectrum>();
  iris::emissive_materials::ConstantEmissiveMaterial material(spectrum);

  iris::testing::EmissiveMaterialTester tester;
  EXPECT_EQ(spectrum.Get(),
            tester.Evaluate(material, iris::TextureCoordinates{}));
}