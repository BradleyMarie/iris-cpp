#include "frontends/pbrt/samplers/random.h"

#include <limits>

#include "iris/image_samplers/random_image_sampler.h"

namespace iris::pbrt_frontend::samplers {
namespace {

static const std::unordered_map<std::string_view, Parameter::Type>
    g_parameters = {
        {"pixelsamples", Parameter::INTEGER},
};

class RandomObjectBuilder
    : public ObjectBuilder<std::unique_ptr<ImageSampler>> {
 public:
  RandomObjectBuilder() : ObjectBuilder(g_parameters) {}

  std::unique_ptr<ImageSampler> Build(
      const std::unordered_map<std::string_view, Parameter>& parameters)
      const override;
};

std::unique_ptr<ImageSampler> RandomObjectBuilder::Build(
    const std::unordered_map<std::string_view, Parameter>& parameters) const {
  uint32_t pixel_samples = 16;

  auto pixelsamples = parameters.find("pixelsamples");
  if (pixelsamples != parameters.end()) {
    auto value = pixelsamples->second.GetIntegerValues(1).front();
    if (value < 0 || value > std::numeric_limits<uint32_t>::max()) {
      std::cerr << "ERROR: Out of range value for parameter: pixelsamples"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    pixel_samples = value;
  }

  return std::make_unique<iris::image_samplers::RandomImageSampler>(
      pixel_samples);
}

}  // namespace

const std::unique_ptr<const ObjectBuilder<std::unique_ptr<ImageSampler>>>
    g_random_builder(std::make_unique<RandomObjectBuilder>());

}  // namespace iris::pbrt_frontend::samplers