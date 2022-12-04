#ifndef _FRONTENDS_PBRT_BUILD_OBJECTS_
#define _FRONTENDS_PBRT_BUILD_OBJECTS_

#include <string_view>
#include <unordered_map>
#include <unordered_set>

#include "frontends/pbrt/object_builder.h"
#include "frontends/pbrt/parameter_list.h"
#include "frontends/pbrt/spectrum_manager.h"
#include "frontends/pbrt/texture_manager.h"
#include "frontends/pbrt/tokenizer.h"

namespace iris::pbrt_frontend {

template <typename T, typename... BuildArgs>
T BuildObject(const ObjectBuilder<T, BuildArgs...>& builder,
              Tokenizer& tokenizer, SpectrumManager& spectrum_manager,
              TextureManager& texture_manager, BuildArgs&&... build_args) {
  std::unordered_set<std::string_view> parameters_parsed;
  std::unordered_map<std::string_view, Parameter> parameters;

  ParameterList parameter_list;
  while (parameter_list.ParseFrom(tokenizer)) {
    auto parameter = builder
                         .Parse(parameter_list, spectrum_manager,
                                texture_manager, parameters_parsed)
                         .value();
    auto name = *parameters_parsed.find(parameter_list.GetName());
    parameters[name] = std::move(parameter);
  }

  return builder.Build(parameters, build_args...);
}

}  // namespace iris::pbrt_frontend

#endif  // _FRONTENDS_PBRT_BUILD_OBJECTS_