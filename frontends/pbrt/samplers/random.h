#ifndef _FRONTENDS_PBRT_SAMPLERS_RANDOM_
#define _FRONTENDS_PBRT_SAMPLERS_RANDOM_

#include <memory>

#include "frontends/pbrt/object_builder.h"
#include "iris/image_sampler.h"

namespace iris::pbrt_frontend::samplers {

extern const std::unique_ptr<const ObjectBuilder<std::unique_ptr<ImageSampler>>>
    g_random_builder;

}  // namespace iris::pbrt_frontend::samplers

#endif  // _FRONTENDS_PBRT_SAMPLERS_RANDOM_