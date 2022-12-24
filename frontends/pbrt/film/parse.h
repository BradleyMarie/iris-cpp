#ifndef _FRONTENDS_PBRT_FILM_PARSE_
#define _FRONTENDS_PBRT_FILM_PARSE_

#include "frontends/pbrt/film/result.h"
#include "frontends/pbrt/object_builder.h"

namespace iris::pbrt_frontend::film {

const ObjectBuilder<Result>& Parse(Tokenizer& tokenizer);

}  // namespace iris::pbrt_frontend::film

#endif  // _FRONTENDS_PBRT_FILM_PARSE_