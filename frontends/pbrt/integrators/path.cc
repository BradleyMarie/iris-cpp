#include "frontends/pbrt/integrators/path.h"

#include <limits>

#include "frontends/pbrt/integrators/light_scenes.h"
#include "frontends/pbrt/integrators/pixel_bounds.h"
#include "iris/integrators/path_integrator.h"

namespace iris::pbrt_frontend::integrators {
namespace {

static const std::unordered_map<std::string_view, Parameter::Type>
    g_parameters = {
        {"lightsamplestrategy", Parameter::STRING},
        {"maxdepth", Parameter::INTEGER},
        {"pixelbounds", Parameter::INTEGER},
        {"rrthreshold", Parameter::FLOAT},
};

class PathObjectBuilder : public ObjectBuilder<Result> {
 public:
  PathObjectBuilder() : ObjectBuilder(g_parameters) {}

  Result Build(const std::unordered_map<std::string_view, Parameter>&
                   parameters) const override;
};

Result PathObjectBuilder::Build(
    const std::unordered_map<std::string_view, Parameter>& parameters) const {
  std::string_view sample_strategy = "uniform";  // TODO: This should be spatial
  std::optional<std::array<size_t, 4>> pixel_bounds;
  visual maximum_path_continue_probability = 0.95;
  visual always_continue_threshold = 1.0;
  uint8_t min_bounces = 3;
  uint8_t max_bounces = 5;

  auto maxdepth = parameters.find("maxdepth");
  if (maxdepth != parameters.end()) {
    auto value = maxdepth->second.GetIntegerValues(1).front();
    if (value < 0 || value > std::numeric_limits<uint8_t>::max()) {
      std::cerr << "ERROR: Out of range value for parameter: maxdepth"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    max_bounces = value;
  }

  auto rrthreshold = parameters.find("rrthreshold");
  if (rrthreshold != parameters.end()) {
    auto value = rrthreshold->second.GetFloatValues(1).front();
    if (value < 0.0 || value > 1.0) {
      std::cerr << "ERROR: Out of range value for parameter: rrthreshold"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    always_continue_threshold = value;
  }

  auto integrator = std::make_unique<iris::integrators::PathIntegrator>(
      maximum_path_continue_probability, always_continue_threshold, min_bounces,
      max_bounces);

  auto lightsamplestrategy = parameters.find("lightsamplestrategy");
  if (lightsamplestrategy != parameters.end()) {
    sample_strategy = lightsamplestrategy->second.GetStringValues(1).front();
  }

  auto light_scene_builder = ParseLightScene(sample_strategy);

  auto pixelbounds = parameters.find("pixelbounds");
  if (pixelbounds != parameters.end()) {
    pixel_bounds = ParsePixelBounds(pixelbounds->second);
  }

  return Result{std::move(integrator), std::move(light_scene_builder),
                pixel_bounds};
}

}  // namespace

const std::unique_ptr<const ObjectBuilder<Result>> g_path_builder(
    std::make_unique<PathObjectBuilder>());

}  // namespace iris::pbrt_frontend::integrators