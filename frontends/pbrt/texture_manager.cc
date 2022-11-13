#include "frontends/pbrt/texture_manager.h"

#include <cstdlib>
#include <iostream>

namespace iris::pbrt_frontend {

ReferenceCounted<textures::ValueTexture2D<visual>>
TextureManager::GetFloatTexture(std::string_view name) const {
  temp_key_ = name;
  auto iter = float_textures_.find(temp_key_);
  if (iter == float_textures_.end()) {
    std::cerr << "ERROR: No float texture defined with name: " << name
              << std::endl;
    exit(EXIT_FAILURE);
  }
  return iter->second;
}

ReferenceCounted<textures::PointerTexture2D<Reflector, SpectralAllocator>>
TextureManager::GetReflectorTexture(std::string_view name) const {
  temp_key_ = name;
  auto iter = reflector_textures_.find(temp_key_);
  if (iter == reflector_textures_.end()) {
    std::cerr << "ERROR: No spectrum texture defined with name: " << name
              << std::endl;
    exit(EXIT_FAILURE);
  }
  return iter->second;
}

void TextureManager::Put(
    std::string_view name,
    ReferenceCounted<textures::ValueTexture2D<visual>> texture) {
  temp_key_ = name;
  float_textures_[temp_key_] = texture;
}

void TextureManager::Put(
    std::string_view name,
    ReferenceCounted<textures::PointerTexture2D<Reflector, SpectralAllocator>>
        texture) {
  temp_key_ = name;
  reflector_textures_[temp_key_] = texture;
}

}  // namespace iris::pbrt_frontend