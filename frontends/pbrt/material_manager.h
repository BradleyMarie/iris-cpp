#ifndef _FRONTENDS_PBRT_MATERIAL_MANAGER_
#define _FRONTENDS_PBRT_MATERIAL_MANAGER_

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include "frontends/pbrt/object_builder.h"
#include "iris/material.h"
#include "iris/reference_counted.h"

namespace iris::pbrt_frontend {

class MaterialManager {
 public:
  std::shared_ptr<ObjectBuilder<iris::ReferenceCounted<Material>>> Get(
      std::string_view name) const;
  void Put(
      std::string_view name,
      std::shared_ptr<ObjectBuilder<iris::ReferenceCounted<Material>>> texture);
  void Clear();

 private:
  std::unordered_map<
      std::string,
      std::shared_ptr<ObjectBuilder<iris::ReferenceCounted<Material>>>>
      materials_;
  mutable std::string temp_key_;
};

}  // namespace iris::pbrt_frontend

#endif  // _FRONTENDS_PBRT_MATERIAL_MANAGER_