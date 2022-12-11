#ifndef _FRONTENDS_PBRT_MATERIALS_MATTE_
#define _FRONTENDS_PBRT_MATERIALS_MATTE_

#include <memory>

#include "frontends/pbrt/object_builder.h"
#include "frontends/pbrt/texture_manager.h"
#include "iris/material.h"
#include "iris/reference_counted.h"

namespace iris::pbrt_frontend::materials {

extern const std::unique_ptr<
    const ObjectBuilder<std::shared_ptr<ObjectBuilder<
                            iris::ReferenceCounted<Material>, TextureManager&>>,
                        TextureManager&>>
    g_matte_builder;

extern const std::shared_ptr<
    ObjectBuilder<iris::ReferenceCounted<Material>, TextureManager&>>
    g_default;

}  // namespace iris::pbrt_frontend::materials

#endif  // _FRONTENDS_PBRT_MATERIALS_MATTE_