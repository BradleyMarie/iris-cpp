#include "frontends/pbrt/spectrum_managers/test_spectrum_manager.h"

#include "iris/reflectors/mock_reflector.h"
#include "iris/spectra/mock_spectrum.h"

namespace iris::pbrt_frontend::spectrum_managers {

ReferenceCounted<Spectrum> TestSpectrumManager::AllocateSpectrum(
    const Color& color) {
  return iris::MakeReferenceCounted<iris::spectra::MockSpectrum>();
}

ReferenceCounted<Spectrum> TestSpectrumManager::AllocateSpectrum(
    const std::map<visual, visual>& wavelengths) {
  return iris::MakeReferenceCounted<iris::spectra::MockSpectrum>();
}

ReferenceCounted<Reflector> TestSpectrumManager::AllocateReflector(
    const Color& color) {
  return iris::MakeReferenceCounted<iris::reflectors::MockReflector>();
}

ReferenceCounted<Reflector> TestSpectrumManager::AllocateReflector(
    const std::map<visual, visual>& wavelengths) {
  return iris::MakeReferenceCounted<iris::reflectors::MockReflector>();
}

void TestSpectrumManager::Clear() {}

}  // namespace iris::pbrt_frontend::spectrum_managers