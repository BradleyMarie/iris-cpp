#include "iris/utility/spectrum_manager.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/spectra/mock_spectrum.h"

TEST(SpectrumManagerTest, EmptyGet) {
  iris::utility::SpectrumManager manager;
  EXPECT_EQ(nullptr, manager.Get(0));
}

TEST(SpectrumManagerTest, AddGet) {
  auto spectrum = std::make_unique<iris::spectra::MockSpectrum>();
  auto ptr = spectrum.get();

  iris::utility::SpectrumManager manager;
  auto index = manager.Add(std::move(spectrum));
  EXPECT_NE(0u, index);
  EXPECT_EQ(ptr, manager.Get(index));
}