#ifndef _FRONTENDS_PBRT_INTEGRATORS_PARSE_
#define _FRONTENDS_PBRT_INTEGRATORS_PARSE_

#include <memory>
#include <string_view>

#include "frontends/pbrt/integrators/result.h"
#include "frontends/pbrt/object_builder.h"

namespace iris::pbrt_frontend::integrators {

const ObjectBuilder<Result>& Parse(Tokenizer& tokenizer);

}  // namespace iris::pbrt_frontend::integrators

#endif  // _FRONTENDS_PBRT_INTEGRATORS_PARSE_