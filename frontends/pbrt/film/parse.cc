#include "frontends/pbrt/film/parse.h"

#include <cstdlib>
#include <iostream>

#include "frontends/pbrt/film/image.h"
#include "frontends/pbrt/quoted_string.h"

namespace iris::pbrt_frontend::film {
namespace {

static const std::unordered_map<std::string_view, const ObjectBuilder<Result>*>
    g_film = {{"image", g_image_builder.get()}};

}  // namespace

const ObjectBuilder<Result>& Parse(Tokenizer& tokenizer) {
  auto type = tokenizer.Next();
  if (!type) {
    std::cerr << "ERROR: Too few parameters to directive: Film" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = Unquote(*type);
  if (!unquoted) {
    std::cerr << "ERROR: Parameter to Film must be a string" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto iter = g_film.find(*unquoted);
  if (iter == g_film.end()) {
    std::cerr << "ERROR: Unsupported type for directive Film: " << *unquoted
              << std::endl;
    exit(EXIT_FAILURE);
  }

  return *iter->second;
}

}  // namespace iris::pbrt_frontend::film