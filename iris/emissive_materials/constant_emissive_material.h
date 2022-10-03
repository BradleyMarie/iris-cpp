#ifndef _IRIS_EMISSIVE_MATERIALS_CONSTANT_EMISSIVE_MATERIAL_
#define _IRIS_EMISSIVE_MATERIALS_CONSTANT_EMISSIVE_MATERIAL_

#include <cassert>
#include <memory>

#include "iris/emissive_material.h"
#include "iris/spectral_allocator.h"
#include "iris/texture_coordinates.h"
#include "iris/utility/spectrum_manager.h"

namespace iris {
namespace emissive_materials {

class ConstantEmissiveMaterial final : public EmissiveMaterial {
 public:
  ConstantEmissiveMaterial(
      std::shared_ptr<iris::utility::SpectrumManager> spectrum_manager,
      size_t index) noexcept
      : spectrum_manager_(std::move(spectrum_manager)),
        spectrum_(*spectrum_manager_->Get(index)) {
    assert(index != 0);
  }

  const Spectrum* Evaluate(
      const TextureCoordinates& texture_coordinates,
      SpectralAllocator& spectral_allocator) const override;

 private:
  const std::shared_ptr<iris::utility::SpectrumManager> spectrum_manager_;
  const Spectrum& spectrum_;
};

}  // namespace emissive_materials
}  // namespace iris

#endif  // _IRIS_EMISSIVE_MATERIALS_CONSTANT_EMISSIVE_MATERIAL_