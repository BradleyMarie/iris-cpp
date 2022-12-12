#include "frontends/pbrt/samplers/parse.h"

#include <cstdlib>
#include <iostream>

#include "frontends/pbrt/quoted_string.h"
#include "frontends/pbrt/samplers/random.h"
#include "frontends/pbrt/samplers/stratified.h"

namespace iris::pbrt_frontend::samplers {
namespace {

static const std::unordered_map<
    std::string_view, const ObjectBuilder<std::unique_ptr<ImageSampler>>*>
    g_samplers = {{"random", g_random_builder.get()},
                  {"stratified", g_stratified_builder.get()}};

}  // namespace

const ObjectBuilder<std::unique_ptr<ImageSampler>>& Parse(
    Tokenizer& tokenizer) {
  auto type = tokenizer.Next();
  if (!type) {
    std::cerr << "ERROR: Too few parameters to directive: Sampler" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = Unquote(*type);
  if (!unquoted) {
    std::cerr << "ERROR: Parameter to Sampler must be a string" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto iter = g_samplers.find(*unquoted);
  if (iter == g_samplers.end()) {
    std::cerr << "ERROR: Unsupported type for directive Sampler: " << *unquoted
              << std::endl;
    exit(EXIT_FAILURE);
  }

  return *iter->second;
}

}  // namespace iris::pbrt_frontend::samplers