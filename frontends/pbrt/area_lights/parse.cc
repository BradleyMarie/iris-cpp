#include "frontends/pbrt/area_lights/parse.h"

#include <cstdlib>
#include <iostream>
#include <unordered_map>

#include "frontends/pbrt/area_lights/diffuse.h"
#include "frontends/pbrt/quoted_string.h"

namespace iris::pbrt_frontend::integrators {
namespace {

static const std::unordered_map<
    std::string_view,
    const ObjectBuilder<std::pair<iris::ReferenceCounted<EmissiveMaterial>,
                                  iris::ReferenceCounted<EmissiveMaterial>>,
                        SpectrumManager&>*>
    g_area_lights = {{"diffuse", g_diffuse_builder.get()}};

}  // namespace

const ObjectBuilder<std::pair<iris::ReferenceCounted<EmissiveMaterial>,
                              iris::ReferenceCounted<EmissiveMaterial>>,
                    SpectrumManager&>&
Parse(Tokenizer& tokenizer) {
  auto type = tokenizer.Next();
  if (!type) {
    std::cerr << "ERROR: Too few parameters to directive: AreaLightSource"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = Unquote(*type);
  if (!unquoted) {
    std::cerr << "ERROR: Parameter to AreaLightSource must be a string"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  auto iter = g_area_lights.find(*unquoted);
  if (iter == g_area_lights.end()) {
    std::cerr << "ERROR: Unsupported type for directive AreaLightSource: "
              << *unquoted << std::endl;
    exit(EXIT_FAILURE);
  }

  return *iter->second;
}

}  // namespace iris::pbrt_frontend::integrators