#ifndef _FRONTENDS_PBRT_AREA_LIGHTS_PARSE_
#define _FRONTENDS_PBRT_AREA_LIGHTS_PARSE_

#include <utility>

#include "frontends/pbrt/object_builder.h"
#include "frontends/pbrt/tokenizer.h"
#include "iris/emissive_material.h"
#include "iris/reference_counted.h"

namespace iris::pbrt_frontend::area_lights {

const ObjectBuilder<std::pair<iris::ReferenceCounted<EmissiveMaterial>,
                              iris::ReferenceCounted<EmissiveMaterial>>,
                    SpectrumManager&>&
Parse(Tokenizer& tokenizer);

}  // namespace iris::pbrt_frontend::area_lights

#endif  // _FRONTENDS_PBRT_AREA_LIGHTS_PARSE_