#include "frontends/pbrt/integrators/parse.h"

#include <cstdlib>
#include <iostream>

#include "frontends/pbrt/integrators/path.h"
#include "frontends/pbrt/quoted_string.h"

namespace iris::pbrt_frontend::integrators {
namespace {

static const std::unordered_map<std::string_view, const ObjectBuilder<Result>*>
    g_integrators = {{"path", g_path_builder.get()}};

}  // namespace

const ObjectBuilder<Result>& Parse(Tokenizer& tokenizer) {
  auto type = tokenizer.Next();
  if (!type) {
    std::cerr << "ERROR: Too few parameters to directive Integrator"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = Unquote(*type);
  if (!unquoted) {
    std::cerr << "ERROR: Parameter to Integrator must be a string" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto iter = g_integrators.find(*unquoted);
  if (iter == g_integrators.end()) {
    std::cerr << "ERROR: Unsupported type for directive Integrator: "
              << *unquoted << std::endl;
    exit(EXIT_FAILURE);
  }

  return *iter->second;
}

}  // namespace iris::pbrt_frontend::integrators