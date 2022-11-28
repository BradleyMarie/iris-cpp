#ifndef _FRONTENDS_PBRT_INTEGRATORS_PIXEL_BOUNDS_
#define _FRONTENDS_PBRT_INTEGRATORS_PIXEL_BOUNDS_

#include <array>

#include "frontends/pbrt/parameter.h"

namespace iris::pbrt_frontend::integrators {

std::array<size_t, 4> ParsePixelBounds(const Parameter& parameter);

}  // namespace iris::pbrt_frontend::integrators

#endif  // _FRONTENDS_PBRT_INTEGRATORS_PIXEL_BOUNDS_