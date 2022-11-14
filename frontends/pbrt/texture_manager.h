#ifndef _FRONTENDS_PBRT_TEXTURE_MANAGER_
#define _FRONTENDS_PBRT_TEXTURE_MANAGER_

#include <string>
#include <string_view>
#include <unordered_map>

#include "iris/reference_counted.h"
#include "iris/reflector.h"
#include "iris/spectral_allocator.h"
#include "iris/textures/texture2d.h"

namespace iris::pbrt_frontend {

class TextureManager {
 public:
  ReferenceCounted<textures::ValueTexture2D<visual>>
  AllocateUniformFloatTexture(visual value);

  ReferenceCounted<textures::PointerTexture2D<Reflector, SpectralAllocator>>
  AllocateUniformReflectorTexture(iris::ReferenceCounted<Reflector> reflector);

  ReferenceCounted<textures::ValueTexture2D<visual>> GetFloatTexture(
      std::string_view name) const;

  ReferenceCounted<textures::PointerTexture2D<Reflector, SpectralAllocator>>
  GetReflectorTexture(std::string_view name) const;

  void Put(std::string_view name,
           ReferenceCounted<textures::ValueTexture2D<visual>> texture);

  void Put(
      std::string_view name,
      ReferenceCounted<textures::PointerTexture2D<Reflector, SpectralAllocator>>
          texture);

 private:
  std::unordered_map<visual, ReferenceCounted<textures::ValueTexture2D<visual>>>
      uniform_float_textures_;

  std::unordered_map<const Reflector*,
                     ReferenceCounted<textures::PointerTexture2D<
                         Reflector, SpectralAllocator>>>
      uniform_reflector_textures_;

  std::unordered_map<std::string,
                     ReferenceCounted<textures::ValueTexture2D<visual>>>
      float_textures_;

  std::unordered_map<std::string, ReferenceCounted<textures::PointerTexture2D<
                                      Reflector, SpectralAllocator>>>
      reflector_textures_;

  mutable std::string temp_key_;
};

}  // namespace iris::pbrt_frontend

#endif  // _FRONTENDS_PBRT_TEXTURE_MANAGER_