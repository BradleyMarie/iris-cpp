#include "frontends/pbrt/area_lights/diffuse.h"

#include <unordered_map>

#include "iris/emissive_materials/constant_emissive_material.h"

namespace iris::pbrt_frontend::area_lights {
namespace {

static const Color kDefaultEmissions({1.0, 1.0, 1.0}, Color::RGB);
static const bool kDefaultTwoSided = false;
// static const int64_t kDefaultSamples = 1;

static const std::unordered_map<std::string_view, Parameter::Type>
    g_parameters = {
        {"L", Parameter::SPECTRUM},
        {"samples", Parameter::INTEGER},
        {"twosided", Parameter::BOOL},
};

class DiffuseObjectBuilder
    : public ObjectBuilder<std::pair<iris::ReferenceCounted<EmissiveMaterial>,
                                     iris::ReferenceCounted<EmissiveMaterial>>,
                           SpectrumManager&> {
 public:
  DiffuseObjectBuilder() : ObjectBuilder(g_parameters) {}

  std::pair<iris::ReferenceCounted<EmissiveMaterial>,
            iris::ReferenceCounted<EmissiveMaterial>>
  Build(const std::unordered_map<std::string_view, Parameter>& parameters,
        SpectrumManager& spectrum_manager) const override;
};

std::pair<iris::ReferenceCounted<EmissiveMaterial>,
          iris::ReferenceCounted<EmissiveMaterial>>
DiffuseObjectBuilder::Build(
    const std::unordered_map<std::string_view, Parameter>& parameters,
    SpectrumManager& spectrum_manager) const {
  iris::ReferenceCounted<Spectrum> emissions =
      spectrum_manager.AllocateSpectrum(kDefaultEmissions);
  bool two_sided = kDefaultTwoSided;

  auto l = parameters.find("L");
  if (l != parameters.end()) {
    emissions = l->second.GetSpectra(1).front();
  }

  auto samples = parameters.find("samples");
  if (samples != parameters.end()) {
    auto value = samples->second.GetIntegerValues(1).front();
    if (value <= 0 || value > std::numeric_limits<uint8_t>::max()) {
      std::cerr << "ERROR: Out of range value for parameter: samples"
                << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  auto twosided = parameters.find("twosided");
  if (twosided != parameters.end()) {
    two_sided = twosided->second.GetBoolValues(1).front();
  }

  auto front_material =
      iris::MakeReferenceCounted<emissive_materials::ConstantEmissiveMaterial>(
          std::move(emissions));

  iris::ReferenceCounted<EmissiveMaterial> back_material;
  if (two_sided) {
    back_material = front_material;
  }

  return std::make_pair(std::move(front_material), std::move(back_material));
}

}  // namespace

const std::unique_ptr<
    const ObjectBuilder<std::pair<iris::ReferenceCounted<EmissiveMaterial>,
                                  iris::ReferenceCounted<EmissiveMaterial>>,
                        SpectrumManager&>>
    g_diffuse_builder = std::make_unique<DiffuseObjectBuilder>();

}  // namespace iris::pbrt_frontend::area_lights