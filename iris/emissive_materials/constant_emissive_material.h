#ifndef _IRIS_EMISSIVE_MATERIALS_CONSTANT_EMISSIVE_MATERIAL_
#define _IRIS_EMISSIVE_MATERIALS_CONSTANT_EMISSIVE_MATERIAL_

#include <cassert>

#include "iris/emissive_material.h"
#include "iris/reference_counted.h"
#include "iris/spectra/reference_countable_spectrum.h"
#include "iris/spectral_allocator.h"
#include "iris/texture_coordinates.h"

namespace iris {
namespace emissive_materials {

class ConstantEmissiveMaterial final : public EmissiveMaterial {
 public:
  ConstantEmissiveMaterial(
      iris::ReferenceCounted<iris::spectra::ReferenceCountableSpectrum>
          spectrum) noexcept
      : spectrum_(std::move(spectrum)) {
    assert(spectrum_);
  }

  const Spectrum* Evaluate(
      const TextureCoordinates& texture_coordinates,
      SpectralAllocator& spectral_allocator) const override;

 private:
  const iris::ReferenceCounted<iris::spectra::ReferenceCountableSpectrum>
      spectrum_;
};

}  // namespace emissive_materials
}  // namespace iris

#endif  // _IRIS_EMISSIVE_MATERIALS_CONSTANT_EMISSIVE_MATERIAL_