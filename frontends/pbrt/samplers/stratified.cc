#include "frontends/pbrt/samplers/stratified.h"

#include <limits>

#include "iris/image_samplers/stratified_image_sampler.h"

namespace iris::pbrt_frontend::samplers {
namespace {

static const std::unordered_map<std::string_view, Parameter::Type>
    g_parameters = {
        {"jitter", Parameter::BOOL},
        {"xsamples", Parameter::INTEGER},
        {"ysamples", Parameter::INTEGER},
};

class StratifiedObjectBuilder
    : public ObjectBuilder<std::unique_ptr<ImageSampler>> {
 public:
  StratifiedObjectBuilder() : ObjectBuilder(g_parameters) {}

  std::unique_ptr<ImageSampler> Build(
      const std::unordered_map<std::string_view, Parameter>& parameters)
      const override;
};

std::unique_ptr<ImageSampler> StratifiedObjectBuilder::Build(
    const std::unordered_map<std::string_view, Parameter>& parameters) const {
  bool jittered = true;
  uint16_t x_samples = 2;
  uint16_t y_samples = 2;

  auto jitter = parameters.find("jitter");
  if (jitter != parameters.end()) {
    jittered = jitter->second.GetBoolValues(1).front();
  }

  auto xsamples = parameters.find("xsamples");
  if (xsamples != parameters.end()) {
    auto value = xsamples->second.GetIntegerValues(1).front();
    if (value < 0 || value > std::numeric_limits<uint16_t>::max()) {
      std::cerr << "ERROR: Out of range value for parameter: xsamples"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    x_samples = value;
  }

  auto ysamples = parameters.find("ysamples");
  if (ysamples != parameters.end()) {
    auto value = ysamples->second.GetIntegerValues(1).front();
    if (value < 0 || value > std::numeric_limits<uint16_t>::max()) {
      std::cerr << "ERROR: Out of range value for parameter: ysamples"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    y_samples = value;
  }

  return std::make_unique<iris::image_samplers::StratifiedImageSampler>(
      x_samples, y_samples, jittered);
}

}  // namespace

const std::unique_ptr<const ObjectBuilder<std::unique_ptr<ImageSampler>>>
    g_stratified_builder(std::make_unique<StratifiedObjectBuilder>());

}  // namespace iris::pbrt_frontend::samplers