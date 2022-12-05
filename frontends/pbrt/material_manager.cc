#include "frontends/pbrt/material_manager.h"

#include <cstdlib>
#include <iostream>

namespace iris::pbrt_frontend {

ReferenceCounted<Material> MaterialManager::Get(std::string_view name) const {
  temp_key_ = name;

  auto iter = materials_.find(temp_key_);
  if (iter == materials_.end()) {
    std::cerr << "ERROR: No material defined with name: " << name << std::endl;
    exit(EXIT_FAILURE);
  }

  return iter->second;
}

void MaterialManager::Put(std::string_view name,
                          ReferenceCounted<Material> texture) {
  temp_key_ = name;
  materials_[temp_key_] = texture;
}

void MaterialManager::Clear() { materials_.clear(); }

}  // namespace iris::pbrt_frontend