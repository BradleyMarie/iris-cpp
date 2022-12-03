#ifndef _FRONTENDS_PBRT_SAMPLERS_PARSE_
#define _FRONTENDS_PBRT_SAMPLERS_PARSE_

#include <memory>

#include "frontends/pbrt/object_builder.h"
#include "iris/image_sampler.h"

namespace iris::pbrt_frontend::samplers {

const ObjectBuilder<std::unique_ptr<ImageSampler>>& Parse(Tokenizer& tokenizer);

}  // namespace iris::pbrt_frontend::samplers

#endif  // _FRONTENDS_PBRT_SAMPLERS_PARSE_