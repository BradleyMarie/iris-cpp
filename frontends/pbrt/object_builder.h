#ifndef _FRONTENDS_PBRT_OBJECT_BUILDER_
#define _FRONTENDS_PBRT_OBJECT_BUILDER_

#include <cstdlib>
#include <iostream>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

#include "frontends/pbrt/parameter.h"
#include "frontends/pbrt/parameter_list.h"

namespace iris::pbrt_frontend {

template <typename T, typename... BuildArgs>
class ObjectBuilder {
 public:
  ObjectBuilder(
      const std::unordered_map<std::string_view, Parameter::Type>& parameters)
      : parameters_(parameters) {}

  std::optional<Parameter> Parse(
      const ParameterList& parameter_list, SpectrumManager& spectrum_manager,
      TextureManager& texture_manager,
      std::unordered_set<std::string_view>& handled_args,
      bool must_succeed = true) const {
    if (handled_args.contains(parameter_list.GetName())) {
      std::cerr << "ERROR: A parameter was specified twice: "
                << parameter_list.GetName() << std::endl;
      exit(EXIT_FAILURE);
    }

    auto iter = parameters_.find(parameter_list.GetName());
    if (iter == parameters_.end()) {
      if (must_succeed) {
        std::cerr << "ERROR: Unknown parameter: " << parameter_list.GetName()
                  << std::endl;
        exit(EXIT_FAILURE);
      }

      return std::nullopt;
    }

    Parameter parameter;
    parameter.LoadFrom(parameter_list, iter->second, spectrum_manager,
                       texture_manager);

    handled_args.insert(iter->first);

    return parameter;
  }

  virtual T Build(
      const std::unordered_map<std::string_view, Parameter>& parameters,
      BuildArgs... build_args) const = 0;

 private:
  std::unordered_map<std::string_view, Parameter::Type> parameters_;
};

}  // namespace iris::pbrt_frontend

#endif  // _FRONTENDS_PBRT_OBJECT_BUILDER_