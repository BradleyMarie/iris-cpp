#include "iris/emissive_materials/constant_emissive_material.h"

#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "iris/spectra/mock_spectrum.h"
#include "iris/testing/emissive_material_tester.h"

TEST(ConstantEmissiveMaterialTest, Evaluate) {
  auto manager = std::make_shared<iris::utility::SpectrumManager>();
  auto index = manager->Add(std::make_unique<iris::spectra::MockSpectrum>());
  iris::emissive_materials::ConstantEmissiveMaterial material(manager, index);

  iris::testing::EmissiveMaterialTester tester;
  EXPECT_EQ(manager->Get(index),
            tester.Evaluate(material, iris::TextureCoordinates{}));
}