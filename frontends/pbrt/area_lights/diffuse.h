#ifndef _FRONTENDS_PBRT_AREA_LIGHTS_DIFFUSE_
#define _FRONTENDS_PBRT_AREA_LIGHTS_DIFFUSE_

#include <memory>
#include <utility>

#include "frontends/pbrt/object_builder.h"
#include "iris/emissive_material.h"
#include "iris/reference_counted.h"

namespace iris::pbrt_frontend::integrators {

extern const std::unique_ptr<
    const ObjectBuilder<std::pair<iris::ReferenceCounted<EmissiveMaterial>,
                                  iris::ReferenceCounted<EmissiveMaterial>>,
                        SpectrumManager&>>
    g_diffuse_builder;

}  // namespace iris::pbrt_frontend::integrators

#endif  // _FRONTENDS_PBRT_AREA_LIGHTS_DIFFUSE_