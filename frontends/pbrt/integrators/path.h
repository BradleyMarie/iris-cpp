#ifndef _FRONTENDS_PBRT_INTEGRATORS_PATH_
#define _FRONTENDS_PBRT_INTEGRATORS_PATH_

#include <memory>

#include "frontends/pbrt/integrators/result.h"
#include "frontends/pbrt/object_builder.h"

namespace iris::pbrt_frontend::integrators {

extern const std::unique_ptr<const ObjectBuilder<Result>> g_path_builder;

}  // namespace iris::pbrt_frontend::integrators

#endif  // _FRONTENDS_PBRT_INTEGRATORS_PATH_