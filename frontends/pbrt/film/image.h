#ifndef _FRONTENDS_PBRT_FILM_IMAGE_
#define _FRONTENDS_PBRT_FILM_IMAGE_

#include <memory>

#include "frontends/pbrt/film/result.h"
#include "frontends/pbrt/object_builder.h"

namespace iris::pbrt_frontend::film {

extern const std::unique_ptr<const ObjectBuilder<Result>> g_image_builder;

}  // namespace iris::pbrt_frontend::film

#endif  // _FRONTENDS_PBRT_FILM_IMAGE_